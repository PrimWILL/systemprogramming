#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "serial.h"
#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h>

struct termios tty_back;
bool restore = false;

void configure_interface(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty_back) < 0) {
        perror("tcgetattr");
        exit(1);
    }

    if (tcgetattr(fd, &tty) < 0) {
        perror("tcgetattr");
        exit(1);
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);   
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         
    tty.c_cflag &= ~PARENB;    
    tty.c_cflag &= ~CSTOPB;     
    tty.c_cflag &= ~CRTSCTS;    

    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("tcsetattr");
        exit(1);
    }

    restore=true;
}

void set_blocking(int fd, int mcount)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        perror("tcgetattr");
        exit(1);
    }

    tty.c_cc[VMIN] = mcount ? 1 : 0;
    tty.c_cc[VTIME] = 5;        /* half second timer */

    if (tcsetattr(fd, TCSANOW, &tty) < 0) {
        perror("tcsetattr");
        exit(1);
    }
}

void restore_ser(int fd)
{
    if (restore)
    {
        if (tcsetattr(fd, TCSANOW, &tty_back) < 0) {
            perror("reset tcsetattr");
            exit(1);
        }
    }
}
