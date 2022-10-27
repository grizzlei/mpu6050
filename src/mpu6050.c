#include "mpu6050.h"


////////////////////////////////////////////


mpu6050_t *mpu6050_init(const char *dev)
{
	mpu6050_t *m = (mpu6050_t *)malloc(sizeof(mpu6050_t));

	m->addr = 0x68;
	strncpy(m->device, dev, 63);

	if ((m->fd = open(m->device, O_RDWR)) < 0)
	{
		prerr("cannot open %s. (%s)", m->device, strerror(errno));
		return NULL;
	}

	if (sysguard(ioctl(m->fd, I2C_SLAVE, m->addr), "ioctl failed.") < 0)
	{
		return NULL;
	}

	i2c_smbus_write_byte_data(m->fd, 0x6B, 0x00);

	return m;
}


////////////////////////////////////////////


int mpu6050_cleanup(mpu6050_t *m)
{
	int res;
	if ((res = sysguard(close(m->fd), "cannot close.")) == 0)
	{
		free(m);
		return res;
	}
	return res;
}


////////////////////////////////////////////


int16_t mpu6050_read_word(mpu6050_t *m, uint16_t addr)
{
	uint8_t h = i2c_smbus_read_byte_data(m->fd, addr);
	uint8_t l = i2c_smbus_read_byte_data(m->fd, addr + 1);
	return ((int16_t)h << 8) | l;
}


////////////////////////////////////////////


void mpu6050_get_gyro_raw(mpu6050_t *m, int16_t *gx, int16_t *gy, int16_t *gz)
{
	*gx = mpu6050_read_word(m, 0x43);
	*gy = mpu6050_read_word(m, 0x45);
	*gz = mpu6050_read_word(m, 0x47);
}


////////////////////////////////////////////


void mpu6050_get_accl_raw(mpu6050_t *m, int16_t *ax, int16_t *ay, int16_t *az)
{
	*ax = mpu6050_read_word(m, 0x3B);
	*ay = mpu6050_read_word(m, 0x3D);
	*az = mpu6050_read_word(m, 0x3F);
}


////////////////////////////////////////////


void mpu6050_set_config(mpu6050_t * m, uint8_t config)
{
	i2c_smbus_write_byte_data(m->fd, 0x1A, config);
}


////////////////////////////////////////////


uint8_t mpu6050_get_config(mpu6050_t * m)
{
	return (uint8_t)i2c_smbus_read_byte_data(m->fd, 0x1A);
}


////////////////////////////////////////////


void mpu6050_set_gyro_config(mpu6050_t * m, uint8_t config)
{
	i2c_smbus_write_byte_data(m->fd, 0x1B, config);
}


////////////////////////////////////////////


uint8_t mpu6050_get_gyro_config(mpu6050_t * m)
{
	return (uint8_t)i2c_smbus_read_byte_data(m->fd, 0x1B);
}


////////////////////////////////////////////


void mpu6050_set_accl_config(mpu6050_t * m, uint8_t config)
{
	i2c_smbus_write_byte_data(m->fd, 0x1C, config);
}


////////////////////////////////////////////


uint8_t mpu6050_get_accl_config(mpu6050_t * m)
{
	return (uint8_t)i2c_smbus_read_byte_data(m->fd, 0x1C);
}


////////////////////////////////////////////