#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<fcntl.h>
#include<dirent.h>
#include<unistd.h>
#include<stdlib.h>

#define MaxLenName 256

typedef struct _TDirInfo
{
  int AmountOfFiles;
  int TotalFileSize;
  int TotalFileCount;
  int Error;
  char DirName[MaxLenName];
  char *MaxSizeFileName;
  DIR *DirStream;
}TDirInfo, *TPDirInfo;

int OutputAllFiles(char* Dir);

int main ()
{
    char Dir[MaxLenName];
    if (getcwd(Dir, MaxLenName) == NULL){
        printf("Cannot get current directory name");
        return 1;
    };
    OutputAllFiles(Dir);

    OutputAllFiles("/");
    return 0;
};

int OutputAllFiles(char *Dir)
{
    TPDirInfo DirInfo;
    DirInfo = calloc(1, sizeof(TDirInfo));
    DIR* DirStream;
    if ((DirInfo->DirStream = opendir(Dir)) == NULL){
        printf("Cannot open directory \"%s\"", Dir);
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
    while ((PDirStruct = readdir(DirInfo->DirStream)) != NULL){
        printf("\"%s\"\n", PDirStruct->d_name);
        i++;
    };
    if (i != count){
        printf("Cannot read file \n");
        return 2;
    };
    free (PDirStruct);
    if (closedir(DirInfo->DirStream) != 0){
        printf("Cannot close directory \"%s\"", Dir);
        return 3;
    };
    return 0;
};