#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    int fd = 0, ret = 0;

    fd = open("/dev/randevice", O_RDONLY);
    if(fd < 0){
        printf("False to open randevice\n");
        return -1;
    }
    printf("fd: %d\n", fd);
    read(fd, &ret, sizeof(ret));
    printf("Random number from randevice: %d\n", ret);
    close(fd);
    return 0;

}