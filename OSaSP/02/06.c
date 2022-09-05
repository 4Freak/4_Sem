#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<fcntl.h>
#include<dirent.h>
#include<unistd.h>
#include<stdlib.h>

#define MaxLenName 256

int OutputAllFiles(char* Dir);

int main ()
{

    OutputAllFiles(".");

    OutputAllFiles("/");
    return 0;
};

int OutputAllFiles(char *Dir)
{
    DIR* DirStream;
    if ((DirStream = opendir(Dir)) == NULL){
        fprintf(stderr, "Cannot open directory \"%s\"", Dir);
        return 1;
    };

    printf("Files in directory: \"%s\"\n", Dir);
    struct dirent* PDirStruct, **namelist;
    int count = scandir(Dir, &namelist,NULL, NULL);
    int i = 0;  
    for(; i < count; i++)
        free(namelist[i]);
    free(namelist);
    i = 0;
    while ((PDirStruct = readdir(DirStream)) != NULL){
        printf("\"%s\"\n", PDirStruct->d_name);
        i++;
    };
    if (i != count){
        fprintf(stderr, "Cannot read file \n");
        return 2;
    };
    free(PDirStruct);
    if (closedir(DirStream) != 0){
        fprintf(stderr, "Cannot close directory \"%s\"", Dir);
        return 3;
    };
    return 0;
};