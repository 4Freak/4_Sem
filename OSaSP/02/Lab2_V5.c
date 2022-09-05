#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dirent.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LEN_NAME 255

typedef struct _TDirInfo
{
  int AmountOfFiles;
  int TotalFileSize;
  int TotalFileCount;
  int Error;
  char DirName[MAX_LEN_NAME];
  char MaxSizeFileName[MAX_LEN_NAME];
  DIR *DirStream;
}TDirInfo, *TPDirInfo;

void CreatePathname(char *Pathname, char *StartDir, char *Filename);
TPDirInfo GetDirInfo(TPDirInfo DirInfo);
int OutputDirInfo(TPDirInfo DirInfo, FILE *Fp);


int main(int argc, char * argv[])
{

  if (argc < 3){
    fprintf(stderr, "Not enough actual parametrs\n");
    fprintf(stderr, "./{FileName} {StartDir} {OutFile}\nFileName - Name of file\nStartDir - Start directory\nOutFile - Outuput file\n");
    return 1;
  };

  int Fd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT ,0644);
  if (Fd < 0){
    fprintf(stderr, "Cannot open file \"%s\"\n", argv[1]);
    return 2;
  };
  
  FILE* Fp = fdopen(Fd, "w");

  TPDirInfo DirInfo;
  DIR *MainDirStream;
  DirInfo = calloc(1, sizeof(TDirInfo));
  if (((MainDirStream) = opendir(argv[1])) == NULL){
    fprintf(stderr,"Cannot open directory \"%s\"", argv[1]);
    return 3;
  };
      
  strcpy(DirInfo->DirName, argv[1]);
  DirInfo->DirStream = MainDirStream;
  GetDirInfo(DirInfo);
  OutputDirInfo(DirInfo, Fp);

  struct dirent* PDirStruct;
  while ((PDirStruct = readdir(MainDirStream)) != NULL){
    if (PDirStruct->d_type == 4 && //DT_DIR == 4
        PDirStruct->d_name[0] != '.'){ 
      CreatePathname(DirInfo->DirName, argv[1], PDirStruct->d_name);
      if ((DirInfo->DirStream = opendir(DirInfo->DirName)) == NULL){
        fprintf(stderr,"Cannot open directory \"%s\"", argv[1]);
        return 3;
      };
      GetDirInfo(DirInfo); 
      OutputDirInfo(DirInfo, Fp);
    };
  };
  
  free(DirInfo);
  if (fclose(Fp) != 0){
    fprintf(stderr,"Cannot close file \"%s\"\n", argv[2]);
    return 5;
  }
  return 0;
};

TPDirInfo GetDirInfo(TPDirInfo PDirInfo)
{
  PDirInfo->Error = 0;
  PDirInfo->TotalFileCount = 0;
  PDirInfo->MaxSizeFileName[0] = '\0';

  struct dirent* PDirStruct;
  struct stat Buff;
  char Pathname[MAX_LEN_NAME];
  int MaxFileSize = 0;
  PDirInfo->TotalFileSize = 0;
  while ((PDirStruct = readdir(PDirInfo->DirStream)) != NULL){
    if (PDirStruct->d_type == 8){ // DT_REG == 8
      CreatePathname(Pathname, PDirInfo->DirName, PDirStruct->d_name);
      stat(Pathname, &Buff);
      PDirInfo->TotalFileSize += Buff.st_size;
      PDirInfo->TotalFileCount++;
      if (MaxFileSize < Buff.st_size){
        MaxFileSize = Buff.st_size;
        strcpy(PDirInfo->MaxSizeFileName, PDirStruct->d_name);
      };
    };
  };
  rewinddir(PDirInfo->DirStream);
  return PDirInfo;
};

void CreatePathname(char *Pathname, char *StartDir, char *FileName)
{
  strcpy(Pathname, StartDir);
  if (StartDir[strlen(StartDir)-1] != '/')
    strcat(Pathname, "/");
  strcat(Pathname, FileName);
};

int OutputDirInfo(TPDirInfo DirInfo, FILE *Fp)
{
  printf("Directory: \"%s\"\n",  DirInfo->DirName);
  fprintf(Fp, "Directory: \"%s\"\n",  DirInfo->DirName);
 
  printf("File count: \"%d\"\n", DirInfo->TotalFileCount);
  fprintf(Fp, "File count: \"%d\"\n", DirInfo->TotalFileCount);
 
  printf("Max file name: \"%s\"\n", DirInfo->MaxSizeFileName);
  fprintf(Fp, "Max file name: \"%s\"\n", DirInfo->MaxSizeFileName);
  return 0;
};