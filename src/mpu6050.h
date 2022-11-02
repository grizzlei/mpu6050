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

#ifndef MPU6050_H
#define MPU6050_H

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif


////////////////////////////////////////////


#define MPU6050_CONF_DEFAULT        0x00

#define MPU6050_GYRO_CONF_FS_250    0x00
#define MPU6050_GYRO_CONF_FS_500    0x08
#define MPU6050_GYRO_CONF_FS_1000   0x10
#define MPU6050_GYRO_CONF_FS_2000   0x18

#define MPU6050_ACCL_CONF_FS_2G     0x00
#define MPU6050_ACCL_CONF_FS_4G     0x08
#define MPU6050_ACCL_CONF_FS_8G     0x10
#define MPU6050_ACCL_CONF_FS_16G    0x18


////////////////////////////////////////////


typedef struct MPU6050
{
    // file descriptor for i2c device.
    int fd;
    // i2c address to read from / write to. 0x68 is mpu6050 specific.
    int addr;
    // i2c device path
    char device[64];
} mpu6050_t;


////////////////////////////////////////////


// creates a mpu6050_t object and initializes it
mpu6050_t * mpu6050_init(const char * dev);


////////////////////////////////////////////


// destroys mpu6050_t object
int mpu6050_cleanup(mpu6050_t * m);


////////////////////////////////////////////


// reads two bytes from address and address + 1 and returns one 16 bit integer
int16_t mpu6050_read_word(mpu6050_t * m, uint16_t addr);


////////////////////////////////////////////


// set gx, gy, gz to most recent values from gyro sensor
void mpu6050_get_gyro_raw(mpu6050_t * m, int16_t *gx, int16_t *gy, int16_t *gz);


////////////////////////////////////////////


// set ax, ay, az to most recent values from accelerometer
void mpu6050_get_accl_raw(mpu6050_t * m, int16_t *ax, int16_t *ay, int16_t *az);


////////////////////////////////////////////


void mpu6050_set_config(mpu6050_t * m, uint8_t config);


////////////////////////////////////////////


uint8_t mpu6050_get_config(mpu6050_t * m);


////////////////////////////////////////////


void mpu6050_set_gyro_config(mpu6050_t * m, uint8_t config);


////////////////////////////////////////////


uint8_t mpu6050_get_gyro_config(mpu6050_t * m);


////////////////////////////////////////////


void mpu6050_set_accl_config(mpu6050_t * m, uint8_t config);


////////////////////////////////////////////


uint8_t mpu6050_get_accl_config(mpu6050_t * m);


////////////////////////////////////////////


#ifdef __cplusplus
}
#endif

#endif