#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define GPIO_BASE           0x200000
#define GPFSEL1_OFFSET      0x04        // GPIO function select register (for pins 0-9)
#define GPSET0_OFFSET       0x1c        // GPIO output set register
#define GPCLR0_OFFSET       0x28        // GPIO output clear register

int main()
{
    unsigned int *gpio;
    int mem_fd;
    if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC) ) < 0) {
        perror("open");
        return 1;
    }
    if((gpio = mmap(NULL, 4096, PROT_READ | PROT_WRITE, NULL, mem_fd, GPIO_BASE)) == NULL) {
        perror("mmap");
        close(mem_fd);
        return 1;
    }

    *(gpio + GPFSEL1_OFFSET) |= (1 << 3);
    *(gpio + GPFSEL1_OFFSET) &= 0xffffffcf;

    *(gpio + GPSET0_OFFSET) = 0xffffffff;

    munmap(gpio, 4096);
    close(mem_fd);
    return 0;
}