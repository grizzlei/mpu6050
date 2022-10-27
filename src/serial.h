#ifndef SERIAL_H
#define SERIAL_H

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <termios.h>

#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif


////////////////////////////////////////////


typedef struct Serial
{
    // file descriptor for serial device.
    int fd;
    // baud rate of the serial device. possible values are 4800, 9600, 19200, 38400, 57600 and 115200.
    int32_t baudrate;
    // configurations to set and revert when we are done with serial port
    struct termios tiosold, tiosnew;
    // serial device path
    char device[64];
}
serial_t;


////////////////////////////////////////////


// creates a serial_t object and initializes it
serial_t * serial_init(const char * dev, int baudrate);


////////////////////////////////////////////


// destroys serial_t object
int serial_cleanup(serial_t * s);


////////////////////////////////////////////


// send one byte of data
int serial_write_char(serial_t * s, char c);


////////////////////////////////////////////


#ifdef __cplusplus
}
#endif

#endif