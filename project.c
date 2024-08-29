#include<stdio.h>
#include<stdlib.h>
void createFile(const char* filename){//Passing the filename as string
    FILE *file=fopen(filename,"w");//This line will try to open the file with write access
    //if file will not exist it will create a new file
    if(file==NULL){
        printf("Error while creating file: %s\n",filename);
        return;
    }
    printf("File created: %s\n",filename);
    fclose(file);// Closes the file after creation.
}
