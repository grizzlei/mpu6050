#include "serial.h"

////////////////////////////////////////////


serial_t *serial_init(const char *dev, int baudrate)
{
	serial_t *s = (serial_t *)malloc(sizeof(serial_t));

	strncpy(s->device, dev, 63);

	if ((s->fd = open(s->device, O_RDWR | O_NOCTTY)) < 0)
	{
		prerr("cannot open %s. (%s)", s->device, strerror(errno));
		return NULL;
	}

	switch (baudrate)
	{
	case 4800:
		s->baudrate = B4800;
		break;
	case 9600:
		s->baudrate = B9600;
		break;
	case 19200:
		s->baudrate = B19200;
		break;
	case 38400:
		s->baudrate = B38400;
		break;
	case 57600:
		s->baudrate = B57600;
		break;
	case 115200:
		s->baudrate = B115200;
		break;
	default:
		prwar("baudrate not recognized, default value 115200 will be used.");
		s->baudrate = B115200;
		break;
	}

	tcgetattr(s->fd, &s->tiosold);
	bzero(&s->tiosnew, sizeof(s->tiosnew));

	s->tiosnew.c_cflag |= s->baudrate | CRTSCTS | CS8 | CLOCAL | CREAD;
	s->tiosnew.c_iflag |= IGNPAR | ICRNL;
	s->tiosnew.c_oflag = 0;
	s->tiosnew.c_cc[VTIME] = 0;
	s->tiosnew.c_cc[VMIN] = 0;

	tcflush(s->fd, TCIOFLUSH);
	tcsetattr(s->fd, TCSANOW, &s->tiosnew);

	return s;
}


////////////////////////////////////////////


int serial_cleanup(serial_t *s)
{
	int res;
	tcsetattr(s->fd, TCSANOW, &s->tiosold);
	if ((res = sysguard(close(s->fd), "cannot close.")) == 0)
	{
		free(s);
		return res;
	}
	return res;
}


////////////////////////////////////////////


int serial_write_char(serial_t *s, char c)
{
	return sysguard(write(s->fd, &c, 1), "cannot write.");
}


////////////////////////////////////////////