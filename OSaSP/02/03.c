#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<fcntl.h>

#define SMB_STOP 6

int main (int argc, char*argv[])
{
    if (argc < 2){
        fprintf(stderr, "Not enough actual parametrs");
        fprintf(stderr, "./{FileName} {Dest}\nFileName - Name of exec file\nDest - Name of File to write in\n");
        return 1;
    };
    int Fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (Fd < 0){
        fprintf(stderr, "Cannot open file \"%s\"\n", argv[1]);
        return 2;
    }
    else{
        FILE *Fp = fdopen(Fd, "w");
        int c; 
        while ((c = getc(stdin)) != SMB_STOP){
            if (putc(c, Fp) == EOF){
                perror("Cannot write charachter to file\n");
                return 3;
            };
        };
        if (fclose(Fp) != 0){ 
            perror("Cannot close file\n");
            return 4;
        };
    };
    return 0;
};