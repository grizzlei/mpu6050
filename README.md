## simple mpu6050 reader for orange pi and raspberry pi

this program will read gyro and accelerometer sensor data from mpu6050 imu and forward it to serial port, so you can work with the data on your development pc.
in `main.c` mpu6050 is initialized with ±500 °/s gyro, ±4g accelerometer configurations.

following format is what you will receive from serial port, which are respectively accelerometer's x, y, z and gyro's x, y, z values:
```
-11224,6704,-88,-84,-99,-7
```
#### quick start with libimu
you can use `libimu` (https://github.com/grizzlei/libimu) to process the data you get from sensors. for a quick start, you can compile and use `demo.c` on your development pc.

#### configuring pi
first, enable `i2c-0` and `uart-1` interfaces from underlying operating system's tool. if you have to use a different configuration, update the code accordingly.

for armbian, use `armbian-config`

#### wiring
```
[mpu6050] -- i2c --> [orange/raspberry pi board] -- uart --> [your development pc]
╔═══════════════╦═══════════════╗    
║ orangepi pins ║  mpu6050 pins ║
╠═══════════════╬═══════════════╣
║ 1 [-][a]      ║  [a] - vcc    ║ 
║   [b][-]      ║  [d] - gnd    ║ 
║   [c][-]      ║  [c] - scl    ║ 
║   [-][e]      ║  [b] - sda    ║
║   [-][f]      ║  [-]          ║ 
║   [-][-]      ║  [-]          ║ 
║   [-][g]      ║  [-]          ║
║   [-][-]      ║  [-]          ║
║   [-][-]      ╠═══════════════╣
║   [-][-]      ║  usb-ttl pins ║
║   [-][-]      ╠═══════════════╣
║   [-][-]      ║ [e][f][g][-]  ║
║   [d][-] 26   ║ rx tx gnd vcc ║
╚═══════════════╩═══════════════╝
```

usb-ttl wiring is not necessary if you don't want gyro and accelerometer sensor data over serial.

#### dependencies

`apt install libi2c-dev`

#### build and install 

! program will start as systemd service if you choose to install

```
git pull https://github.com/grizzlei/mpu6050.git
cd mpu6050
make
(optional) sudo make install
```
* * *

[hasan karaman (whoami)](https://hasankaraman.dev/whoami) - [https://hasankaraman.dev](https://hasankaraman.dev) - [hk@hasankaraman.dev](mailto:hk@hasankaraman.dev) - 2022
