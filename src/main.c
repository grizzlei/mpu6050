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

#include <signal.h>

#include "serial.h"
#include "mpu6050.h"


////////////////////////////////////////////
/// DEFINES


#define BUFLEN 0x100 // 256


////////////////////////////////////////////
/// GLOBAL DECLARATIONS

// quit application
int terminate = 0;

// signal handler
void signal_handler(int signum);


////////////////////////////////////////////


int main(int argc, char **argv)
{
	UNUSED(argc); UNUSED(argv);
	signal(SIGINT, signal_handler);

	mpu6050_t * m = mpu6050_init("/dev/i2c-0");

	mpu6050_set_gyro_config(m, mpu6050_get_gyro_config(m) | MPU6050_GYRO_CONF_FS_500);
	mpu6050_set_accl_config(m, mpu6050_get_accl_config(m) | MPU6050_ACCL_CONF_FS_4G);

	serial_t * s = serial_init("/dev/ttyS1", 115200);

	int16_t gx = 0, gy = 0, gz = 0;
	int16_t ax = 0, ay = 0, az = 0;

	while (!terminate)
	{
		mpu6050_get_gyro_raw(m, &gx, &gy, &gz);
		mpu6050_get_accl_raw(m, &ax, &ay, &az);

		////////////////////////////////////////////
		/// sending values over serial port. disable if you dont need

		char buffer[BUFLEN] = {0x00};
		char eol = 0x0A;
		int index = 0;
		snprintf(buffer, BUFLEN - 1, "%d,%d,%d,%d,%d,%d\r\n", ax, ay, az, gx, gy, gz);

		for(int sent = 0; !terminate; sent = serial_write_char(s, buffer[index++]))
		{
			if(sent == 1)
			{
				if(index == BUFLEN)
					break;

				if(buffer[index -1] == eol)
					break;
			}
			else if(sent == -1)
				goto break_out;
		}


		/// end of sending values over serial port
		////////////////////////////////////////////

		usleep(1000);
	}

	break_out:

	mpu6050_cleanup(m);
	serial_cleanup(s);

	prwar("bye!");

	return 0;
}


////////////////////////////////////////////


void signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		terminate = 1;
		prwar("keyboard interrupt received. attempting to quit gracefully.");
	}
}


////////////////////////////////////////////