/**
 * MIT License
 * Copyright (c) 2022 Hasan Karaman (github.com/grizzlei)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */

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