#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

int main (int argc, char *argv[]) {
  
  //Error Message
  if(argc != 4) {
    
    printf("usage: tokenring number_process probability block_seconds\n");
    
    return EXIT_FAILURE;	
  }
  
  
  char *p = (char*)malloc(50 * sizeof(char));
  for(int i = 1; i <= atoi(argv[1]); i++) {
    if(i == atoi(argv[1])){
      sprintf(p, "pipe%dto1", i);
    }else{
      sprintf(p, "pipe%dto%d", i, i+1);
    }
    
    if((mkfifo(p, 0666)) < 0) {
      fprintf(stderr, "%s: mkfifo error: %s", argv[0], strerror(errno));
      return EXIT_FAILURE;
    }
  }
  free(p);
  
  int val = 0;
  int probability = 100*atof(argv[2]);
  
  pid_t pids[atoi(argv[1])];
  char *write_pipe = (char*)malloc(50 * sizeof(char));
  char *read_pipe = (char*)malloc(50 * sizeof(char));
  for(int i = 1; i <= atoi(argv[1]); i++) {
    if((pids[i-1] = fork()) < 0) {
      fprintf(stderr, "%s: fork error: %s\n", argv[0], strerror(errno));
      return EXIT_FAILURE;
    } else if(pids[i-1] == 0) {
      if(i == atoi(argv[1])) {
	sprintf(write_pipe, "pipe%dto1", i);
	sprintf(read_pipe, "pipe%dto%d", i-1, i);
      } else if(i == 1) {
	sprintf(write_pipe, "pipe%dto%d", i, i+1);
	sprintf(read_pipe, "pipe%dto1", atoi(argv[1]));
      } else {
	sprintf(write_pipe, "pipe%dto%d", i, i+1);
	sprintf(read_pipe, "pipe%dto%d", i-1, i);
      }
      
      srandom(time(NULL) - i * 2);
      
      //Store pipes into an array
      int fd[2];
      
      if(i == 1) {
	//Find error opening the pipe
	if((fd[i] = open(write_pipe, O_WRONLY)) < 0) {
	  fprintf(stderr, "%s: pipe opening error: %s\n", argv[0], strerror(errno));
	  return EXIT_FAILURE;
	}
	
	val++;
	
	//Find error writing the pipe
	if(write(fd[i], &val, sizeof(int)) < 0) {
	  fprintf(stderr, "%s: write error: %s\n", argv[0], strerror(errno));
	  return EXIT_FAILURE;
	}
	
	close(fd[1]);
      }
      
      for(;;) {
	
	if((fd[0] = open(read_pipe, O_RDONLY)) < 0) {
	  fprintf(stderr, "%s: pipe opening error: %s\n", argv[0], strerror(errno));
	  return EXIT_FAILURE;
	}
	
	if(read(fd[0], &val, sizeof(int)) < 0) {
	  fprintf(stderr, "%s: read error: %s\n", argv[0], strerror(errno));
	  return EXIT_FAILURE;
	}
	
	close(fd[0]);
	
	//val variable increments in 1 (+1) every time program pass throw a process
	val++;
	
	//Creates a ramdom number between 1 and 100 to test if program should wait the t seconds
	int rand = random() % 100;
	if(rand <= probability) {
	  printf("[p%d] lock on token (val = %d)\n", i, val);
	  sleep(atoi(argv[3]));
	  printf("[p%d] unlock token\n", i);
	}
	
	if((fd[1] = open(write_pipe, O_WRONLY)) < 0) {
	  fprintf(stderr, "%s: pipe opening error: %s\n", argv[0], strerror(errno));
	  return EXIT_FAILURE;
	}
        
	if(write(fd[1], &val, sizeof(int)) < 0) {
	  fprintf(stderr, "%s: write error: %s\n", argv[0], strerror(errno));
	  return EXIT_FAILURE;
	}
	
	close(fd[1]);
      }
      
      return 0;
    }
  }
  
  for(int i = 0; i < atoi(argv[1]); i++) {
    if(waitpid(pids[i], NULL, 0) < 0) {
      fprintf(stderr, "%s: waitpid error: %s\n", argv[0], strerror(errno));
      return EXIT_FAILURE;
    }
  }
  
  return 0;
}
