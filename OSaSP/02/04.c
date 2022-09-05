#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>

int main (int argc, char*argv[])
{

    if (argc < 3){
        fprintf(stderr, "Not enough actual parametrs");
        fprintf(stderr, "./{FileName} {Src} {NumOfLines}\nFileName - Name of exec file\nSrc - Source\nNunOfLines - Number of lines to output\n"); 
        return 1;
    };
    int Fd = open(argv[1], O_RDONLY);
    if (Fd < 0){
        fprintf(stderr, "Cannot open file \"%s\"\n", argv[1]);
        return 2;
    }
    else{
        FILE *Fp = fdopen(Fd, "rb");
        fseek(Fp, 0, SEEK_SET);
        int LineCount;
        char* endptr;
        errno = 0;
        LineCount = strtol(argv[2], &endptr , 10);
	    if (errno == ERANGE){ 
            fprintf(stderr, "Incorrect parament \"%s\"\nParametn must be number of lines to print\n",argv[2]); 
            return 3;
        }
        if (endptr == argv[2]){
            fprintf(stderr, "No digits where found in paramer\n");
            return 3;
        }
        else{
            char c;
            int i = 0;
            while ((c = fgetc(Fp)) != EOF){
                if (c == '\n'){
                    i++;
                    if (i == LineCount && LineCount != 0){
                        getchar();
                        i = 0;
                    };
                };
                fputc(c, stdout);
            };
            printf("\n");
        };
        if (fclose(Fp) != 0){ 
            perror("Cannot close file\n");
            return 4;    
        }   
        else{
            return 0;
        };
    };
};

