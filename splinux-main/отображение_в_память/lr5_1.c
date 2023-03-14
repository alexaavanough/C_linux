/*******************************************************************************************************************
* Разработать приложение, реализующее отображение файла в память. Демонстрация чтения содержимого файла из памяти. *
* Демонстрация записи в файл через память.																		   *
*******************************************************************************************************************/

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#import <string.h>
#include <time.h>

int main() 
{
    int fd = open("/home/user/Desktop/file", O_RDWR, S_IRUSR | S_IWUSR);

    struct stat sb;
    if (fstat(fd, &sb) < 0)
        perror("couldn't get file size ");
	
    char message[18] = "Printed with mmap";

    clock_t start, end;
    double cpu_time_used;

    printf("Read from file: ");
    start = clock();

    char *file_in_memory = mmap(NULL, 18, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	/***********************************************************************************
	void * mmap(void *addr, size_t len, int prot , int flags, int fd, off_t offset);
	отражает len байтов
	начиная со смещения offset файла, определенного файловым описателем fd, в память
	начиная с адреса addr
	prot описывает желаемый режим защиты памяти
	***********************************************************************************/


    printf("%s", file_in_memory);
    memcpy(file_in_memory, message, sizeof(message));
    end = clock();
    printf("\nPrint to file: %s\n", message);
	
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time = %f\n", cpu_time_used);

    munmap(file_in_memory, sb.st_size);
    close(fd);
	return 0;
}