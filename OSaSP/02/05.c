#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

int ShareFileAccess(int FdSrc, int FdDest);

int main (int argc, char*argv[])
{

    if (argc < 3){
        fprintf(stderr, "Not enough actual parametrs\n");
        fprintf(stderr, "./{FileName} {Src} {Dest}\nFileName - Name of file\nSrc - Source file\nDest - Destination file"); 
        return 1;
    };
    int FdSrc = open(argv[1], O_RDONLY);
    if (FdSrc < 0){
        fprintf(stderr, "Cannot open file \"%s\"", argv[2]);
        return 1;
    }
    int FdDest = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (FdDest < 0){
        fprintf(stderr, "Cannot open file \"%s\"", argv[3]);
        return 2;
    };
    FILE *FpSrc = fdopen(FdSrc, "rb");
    FILE *FpDest = fdopen(FdDest, "wb");
    
    char c;
    while ((c = getc(FpSrc)) != EOF)
        fputc(c, FpDest);

    if (ShareFileAccess(FdSrc, FdDest)){
        fprintf(stderr, "Cannot share file acces\n");
        return 5;
    }

    if (fclose(FpSrc) != 0){
        fprintf(stderr, "Cannot close file \"%s\"\n", argv[1]);
        return 6;
    };  
    if (fclose(FpDest) != 0){
        fprintf(stderr, "Cannot close file \"%s\"\n", argv[2]);
        return 7;
    };  
    return 0;
}


int ShareFileAccess(int FdSrc, int FdDest)
{
    struct stat buf;

	if (fstat(FdSrc,&buf)==-1) {
		perror("Cannot get source file characteric");
		return 1;
	}
	else if (fchmod(FdDest,buf.st_mode)==-1) {
		perror("Cannot change mod of destination file");
		return 2;
	}
    return 0;
};