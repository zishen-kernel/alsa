#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int load_file(const char *path, void **buf)
{
    int fd;
    int res;
    struct stat st;

    fd = open(path, O_RDONLY);
    if(fd < 0)
    {
        printf("open file error\n");
        return 0;
    }

    res = fstat(fd, &st);
    if(res != 0)
    {
        printf("fstat syscall error\n");
        return 0;
    }

    *buf = malloc(st.st_size);
    if(*buf == NULL)
    {
        printf("malloc error\n");
        return 0;
    }

    res = read(fd, *buf, st.st_size);
    if(res != st.st_size)
    {
        printf("read system call error\n");
        return 0;
    }
    
    return st.st_size;
}

int _main(void)
{
    void *buf;
    int *c_buf;
    int s;
    s = load_file("geisha.pcm", &buf);
    c_buf = buf;
    printf("%d %p %x\n", s, buf, c_buf[0]);
}
