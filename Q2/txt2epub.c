#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#pragma GCC diagnostic ignored "-Wformat-truncation"

int main(){
  
    DIR *directory;
    long unsigned int fileSize;
    char temp[128];
    struct dirent *t;

    //Command to zip the files after converting them
    char c[128] = "zip ebooks.zip";

    //Error if directory cant be read
    directory = opendir(".");
    if (directory == NULL){
        perror("Unable to read directory");
        return EXIT_FAILURE;
    }

    while ((t = readdir(directory))){

        fileSize = strlen(t->d_name);

	//Confirm if the arguments are '.txt'
        if (t->d_name[fileSize - 4] == '.' && t->d_name[fileSize - 3] == 't' && t->d_name[fileSize - 2] == 'x' && t->d_name[fileSize - 1] == 't'){

	  //Convert the arguments to '.epub' using pandoc
	  char fileName[fileSize];
	  snprintf(fileName, fileSize - 3, "%s", t->d_name);
	  char ebook[128];
	  char buffer[128];
	  snprintf(buffer, sizeof(buffer), "pandoc %s.txt -o %s.epub --quiet", fileName, fileName);
	  printf("Converting %s.txt...\n", fileName);
          
	  system(buffer);
	  
	  snprintf(ebook, sizeof(ebook), "%s.epub", fileName);
	  snprintf(temp, sizeof(temp), "%s %s --quiet", c, ebook);
	  
	  strcpy(c, temp);
        }
    }
    closedir(directory);
    
    //Execute command to zip the created '.epub' files
    system(c);
    
    return 0;
}
