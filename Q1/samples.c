#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

  //Error Message
  if(argc != 4){
    printf("usage: samples file numberfrags maxfragsize\n");
    return 0;
  }

  //Open the file
  FILE* fp = fopen(argv[1], "r");
  
  int n = atoi(argv[2]);
  int m = atoi(argv[3]);

  // Check if file exists
  if (fp == NULL) {
    printf("Could not open file %s\n", argv[1]);
    return 0;
  }

  //Count number of characters in the file
  int count = 0;
  for (char c = getc(fp); c != EOF; c = getc(fp)){
        count = count + 1;
  }
  
  //Printing n results
  for(int i = 0; i < n; i++){
    //Generate random number between the first and the last - (m-1) character
    int r = rand() % (count-m-1);
    
    printf(">");

    //Using 'fseek' to move the file position r characters ahead
    fseek(fp,r,SEEK_SET);

    //Printing the resulting file after r characters
    int ch;
    for(int i = 0; i<m; i++){
      ch=fgetc(fp);
      putchar(ch);
    }
    
    printf("<\n");
    }
  
  // Close the file
  fclose(fp);
  
  return 0;
}
