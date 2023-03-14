/***************************************************************************************
* Разработать приложение, реализующее чтение и запись в файл без отображения в память. *
***************************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

int main() 
{
    char buff[21];
    char message[21] = "Printed without mmap";
    clock_t start, end;
    double cpu_time_used;
    struct stat sb;

    start = clock();
    int fd = open("/home/user/Desktop/file", O_RDWR, S_IRUSR | S_IWUSR);

    if (read(fd, buff, sizeof(buff)) < 0)
        perror("couldn't read file ");
    write(fd, message, sizeof(message));

    end = clock();
    if (fstat(fd, &sb) < 0)
        perror("couldn't get file size ");
	printf("Read from file: %s\n", buff);
	printf("Print to file: %s\n", message);

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time = %f\n", cpu_time_used);
    close(fd);
	return 0;
}