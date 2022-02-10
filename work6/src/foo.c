#include "foo.h"

int foo_write_on_file()
{
    int fd;
    char *buf = malloc(sizeof(char) * 256);
    strcpy(buf, "string");
    fd = open("file.txt", O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR | S_IXUSR);
    if (fd == -1)
    {
        fd = open("file.txt", O_WRONLY | O_TRUNC, 00700);
        if (fd == -1)
        {
            return -1;
        }
    }
    if (write(fd, buf, strlen(buf)) == -1)
    {
        return -2;
    }
    free(buf);
    close(fd);
    return 0;
}

int foo_read_on_file()
{
    ssize_t ret;
    char ch;
    char *buf = malloc(sizeof(char) * 256);
    int fd = open("file.txt", O_RDONLY);
    if (fd == -1)
    {
        return -1;
    }
    int i = 0;
    while ((ret = read(fd, &ch, 1)) > 0)
    {
        buf[i] = ch;
        i++;
    }
    if (ret == -1)
    {
        return -2;
    }
    buf[i] = '\0';
    printf("%s\n", buf);
    free(buf);
    close(fd);
    return 0;
}

int foo_reverse_read_on_file()
{
    int fd;
    ssize_t ret;
    long position_last;
    char ch;
    int i;
    char *buf = malloc(sizeof(char) * 256);
    fd = open("file.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("File opening error");
        return -1;
    }
    // переместились в конец, узнали позицию последнего символа
    position_last = lseek(fd, 0, SEEK_END);
    if (position_last == -1)
    {
        return -3;
    }
    // вернулись в начало файла
    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        return -3;
    }
    i = position_last - 1;
    while ((ret = read(fd, &ch, 1)) > 0)
    {
        buf[i] = ch;
        i--;
    }
    if (ret == -1)
    {
        return -2;
    }
    buf[position_last] = '\0';
    printf("%s\n", buf);
    free(buf);
    close(fd);
    return 0;
}