#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<dirent.h>
#include<fcntl.h>

int main(void)
{
   int fptr,oldstdout;
   char msg[] = "Тестовый пример";
   /* создание файла */
   fptr = open("DUMMY.FIL",O_CREAT|O_RDWR, S_IREAD|S_IWRITE);
   if(fptr)
   {
      /* создать дополнительный дескриптор для stdout */
      oldstdout = dup(STDOUT_FILENO);
      /* перенаправить стандартный вывод в файл, путем
         дублирования его дескриптора */
      dup2(fptr,STDOUT_FILENO);
      /* закрыть файл */
      close(fptr);
      /* было перенаправлено в DUMMY.FIL */
      write(STDOUT_FILENO,msg,strlen(msg));
      /* восстановить исходный дескриптор stdout */
      dup2(oldstdout,STDOUT_FILENO);
      /* закрыть второй дескриптор stdout */
      close(oldstdout);
      printf("This is console");
   }
   else
   {
      printf("Ошибка при открытии файла. \n");
   }
   return 0;
}