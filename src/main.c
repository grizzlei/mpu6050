#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include <i2c/smbus.h>

////////////////////////////////////////////
/// DEFINES


#define BUFLEN 0x100 // 256

#define GYRO_CONFIG 0x00  // 0b00000000
#define ACCEL_CONFIG 0x00 // 0b00000000

#define prdbg(x, ...) (printf("\x1b[33mdebug:: " x "\x1b[0m\n", ##__VA_ARGS__))
#define prerr(x, ...) (printf("\x1b[31merror:: " x "\x1b[0m\n", ##__VA_ARGS__))
#define prwar(x, ...) (printf("\x1b[36mwarng:: " x "\x1b[0m\n", ##__VA_ARGS__))

////////////////////////////////////////////
/// GLOBAL DECLARATIONS


// quit application
int terminate = 0;

// file descriptor for i2c device.
int fd_i2c;

// file descriptor for serial device
int fd_serial;

// configurations to set and revert when we are done with serial port
struct termios tiosold, tiosnew;

// i2c address to read from / write to. 0x68 is mpu6050 specific.
uint8_t addr = 0x68;

// i2c device path
const char *fname_i2c0 = "/dev/i2c-0";

// serial device path
const char *fname_serial = "/dev/ttyS1";

// serial baud rate
const int32_t serial_baud = B115200;


////////////////////////////////////////////
/// FUNCTIONS


// wraps system calls with an error to show a message on error (when res is -1).
int sysguard(int res, char *msg);

// reads two bytes from address and address + 1 and returns one 16 bit integer
int16_t mpu6050_read_word(uint16_t addr);

// set gx, gy, gz to most recent values from gyro sensor
void mpu6050_get_gyro_raw(int16_t *gx, int16_t *gy, int16_t *gz);

// set ax, ay, az to most recent values from accelerometer
void mpu6050_get_accl_raw(int16_t *ax, int16_t *ay, int16_t *az);

// init serial port
void serial_port_init();

// cleanup serial port
void serial_port_cleanup();

// signal handler
void signal_handler(int signum);

// get time in seconds up to nano seconds
double get_time_sec();


////////////////////////////////////////////


int main(int argc, char **argv)
{

	signal(SIGINT, signal_handler);

	if ((fd_i2c = open(fname_i2c0, O_RDWR)) < 0)
	{
		prerr("cannot open %s. (%s)", fname_i2c0, strerror(errno));
		return -1;
	}

	if (sysguard(ioctl(fd_i2c, I2C_SLAVE, addr), "ioctl failed.") < 0)
	{
		return -1;
	}

	i2c_smbus_write_byte_data(fd_i2c, 0x6b, 0x00);

	int16_t gx = 0, gy = 0, gz = 0;
	int16_t ax = 0, ay = 0, az = 0;

	serial_port_init();

	while (!terminate)
	{
		mpu6050_get_gyro_raw(&gx, &gy, &gz);
		mpu6050_get_accl_raw(&ax, &ay, &az);

		// do whatever you like with these values.
		// in this case I forward them over serial port

		////////////////////////////////////////////
		/// sending values over serial port. disable if you dont need

		char buffer[BUFLEN] = {0x00};
		int index = 0;
		char eol = 0x0A;
		snprintf(buffer, BUFLEN - 1, "%d,%d,%d,%d,%d,%d\r\n", ax, ay, az, gx, gy, gz);

		for(int sent = 0; !terminate; sent = sysguard(write(fd_serial, &buffer[index++], 1), "cannot write."))
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

	serial_port_cleanup();

	printf("bye!");

	return 0;
}


////////////////////////////////////////////


int sysguard(int res, char *msg)
{
	if (res == -1)
	{
		prerr("%s (%s)", msg, strerror(errno));
	}
	return res;
}


////////////////////////////////////////////


int16_t mpu6050_read_word(uint16_t addr)
{
	uint8_t h = i2c_smbus_read_byte_data(fd_i2c, addr);
	uint8_t l = i2c_smbus_read_byte_data(fd_i2c, addr + 1);
	return ((int16_t)h << 8) | l;
}


////////////////////////////////////////////


void mpu6050_get_gyro_raw(int16_t *gx, int16_t *gy, int16_t *gz)
{
	*gx = mpu6050_read_word(0x43);
	*gy = mpu6050_read_word(0x45);
	*gz = mpu6050_read_word(0x47);
}


////////////////////////////////////////////


void mpu6050_get_accl_raw(int16_t *ax, int16_t *ay, int16_t *az)
{
	*ax = mpu6050_read_word(0x3B);
	*ay = mpu6050_read_word(0x3D);
	*az = mpu6050_read_word(0x3F);
}


////////////////////////////////////////////


void serial_port_init()
{
	if ((fd_serial = open(fname_serial, O_RDWR | O_NOCTTY)) < 0)
	{
		prerr("cannot open %s. (%s)", fname_serial, strerror(errno));
		exit(-1);
	}

	tcgetattr(fd_serial, &tiosold);
	bzero(&tiosnew, sizeof(tiosnew));

	tiosnew.c_cflag |= serial_baud | CRTSCTS | CS8 | CLOCAL | CREAD;
	tiosnew.c_iflag |= IGNPAR | ICRNL;
	tiosnew.c_oflag = 0;
	tiosnew.c_cc[VTIME] = 0;
	tiosnew.c_cc[VMIN] = 0;

	tcflush(fd_serial, TCIOFLUSH);
	tcsetattr(fd_serial, TCSANOW, &tiosnew);
}


////////////////////////////////////////////


void serial_port_cleanup()
{
	tcsetattr(fd_serial, TCSANOW, &tiosold);
	close(fd_serial);
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


double get_time_sec()
{
	struct timespec now = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &now);
	return (double)now.tv_sec + 1e-9 * now.tv_nsec;
}


////////////////////////////////////////////