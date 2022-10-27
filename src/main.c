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