#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;
    char ch;

    if ( (fd = open("/dev/LED", O_RDWR | O_SYNC) ) < 0) {
        perror("open");
        return 1;
    }

    printf("Turn on LED - 1\nTurn off LED - 0\nExit - 3\n\n");

    scanf(" %c", &ch);
    while(ch != '3')
    {
        if (write(fd, &ch, 1) < 0)
        {
            perror("write");
        }
        scanf(" %c", &ch);
    }
    printf("Bye\n");

    close(fd);
    return 0;
}