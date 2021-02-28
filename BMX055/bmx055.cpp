#include"bmx055.hpp"

namespace bmx055
{

bmx055::bmx055(i2c_inst_t *ch) {
	i2c_ch = ch;
	imuInitialize();
}

void bmx055::imuInitialize() {
	uint8_t val[2];
//----------------------------------------------------------------//
	val[0] = 0x0F;	// Select PMU_Range register
	val[1] = 0x03;	// Range = +/- 2g
	i2c_write_blocking(i2c_ch, BMX055_ACCL_ADDRESS, val, 2, false);
	sleep_us(100);
//----------------------------------------------------------------//
	val[0] = 0x10;	// Select PMU_BW register
	val[1] = 0x08;	// Bandwidth = 7.81 Hz
	i2c_write_blocking(i2c_ch, BMX055_ACCL_ADDRESS, val, 2, false);
	sleep_us(100);
//----------------------------------------------------------------//
	val[0] = 0x11;	// Select PMU_LPW register
	val[1] = 0x03;	// Normal mode, Sleep duration = 0.5ms
	i2c_write_blocking(i2c_ch, BMX055_ACCL_ADDRESS, val, 2, false);
	sleep_us(100);
//----------------------------------------------------------------//
	val[0] = 0x0F;	// Select Range register
	val[1] = 0x04;	// Range = +/- 125 degree/s
	i2c_write_blocking(i2c_ch, BMX055_GYRO_ADDRESS, val, 2, false);
	sleep_us(100);
//----------------------------------------------------------------//
	val[0] = 0x010;	// Select BandWidth register
	val[1] = 0x07;	// ODR = 100 Hz
	i2c_write_blocking(i2c_ch, BMX055_GYRO_ADDRESS, val, 2, false);
	sleep_us(100);
//----------------------------------------------------------------//
	val[0] = 0x11;	// Select LPW1 register
	val[1] = 0x00;	// Normal mode, Sleep duration = 2ms
	i2c_write_blocking(i2c_ch, BMX055_GYRO_ADDRESS, val, 2, false);
	sleep_us(100);
//----------------------------------------------------------------//
	val[0] = 0x4B;	// Select Mag register
	val[1] = 0x83;	// Soft reset
	i2c_write_blocking(i2c_ch, BMX055_MAG_ADDRESS, val, 2, false);
	sleep_us(100);
//----------------------------------------------------------------//
	val[0] = 0x4B;	// Select Mag register
	val[1] = 0x01;	// Soft reset
	i2c_write_blocking(i2c_ch, BMX055_MAG_ADDRESS, val, 2, false);
	sleep_us(100);
//----------------------------------------------------------------//
	val[0] = 0x4C;	// Select Mag register
	val[1] = 0x00;	// Normal mode, ODR = 10 Hz
	i2c_write_blocking(i2c_ch, BMX055_MAG_ADDRESS, val, 2, false);
	sleep_us(100);
//----------------------------------------------------------------//
	val[0] = 0x4E;	// Select Mag register
	val[1] = 0x84;	// X, Y, Z-Axis enabled
	i2c_write_blocking(i2c_ch, BMX055_MAG_ADDRESS, val, 2, false);
	sleep_us(100);
//----------------------------------------------------------------//
	val[0] = 0x51;	// Select Mag register
	val[1] = 0x04;	// No. of Repetitions for X-Y Axis = 9
	i2c_write_blocking(i2c_ch, BMX055_MAG_ADDRESS, val, 2, false);
	sleep_us(100);
//----------------------------------------------------------------//
	val[0] = 0x52;	// Select Mag register
	val[1] = 0x16;	// No. of Repetitions for X-Y Axis = 15
	i2c_write_blocking(i2c_ch, BMX055_MAG_ADDRESS, val, 2, false);
	sleep_us(100);

	accl_resolution = 0.0098;
	gyro_resolution = 0.0038;
}

void readAccl() {
	uint8_t data[6];
	uint8_t sub_addr[1] = {0x02};
	int ax;
	int ay;
	int az;
	int ack;
	
	ack = i2c_write_blocking(i2c_ch, BMX055_ACCL_ADDRESS, sub_addr, 1, true);
	// Read 6 bytes of data
	// xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
	if(ack == 1) i2c_read_blocking(i2c_ch, BMX055_ACCL_ADDRESS, data, 6, false);
	// Convert the data to 12-bits
	ax = ((data[1]<<8) | data[0])>>4;
	if (ax > 2047)  ax -= 4096;
	ay = ((data[3]<<8) | data[2])>>4;
	if (ay > 2047)  ay -= 4096;
	az = ((data[5]<<8) | data[4])>>4;
	if (az > 2047)  az -= 4096;
	accl[0] = ax*accl_resolution;   // renge +-2g 
	accl[1] = ay*accl_resolution;   // renge +-2g
	accl[2] = az*accl_resolution;   // renge +-2g
}

void readGyro() {
	uint8_t data[6];
	uint8_t sub_addr[1] = {0x02};
	int gx;
	int gy;
	int gz;
	int ack;
	
	ack = i2c_write_blocking(i2c_ch, BMX055_GYRO_ADDRESS, sub_addr, 1, true);
	// Read 6 bytes of data
	if(ack == 1) i2c_read_blocking(i2c_ch, BMX055_GYRO_ADDRESS, data, 6, false);
	// Convert the data to 12-bits
	gx = (data[1]<<8) | data[0];
	if (gx > 32767)  gx -= 65536;
	gy = (data[3]<<8) | data[2];
	if (gy > 32767)  gy -= 65536;
	gz = (data[5]<<8) | data[4];
	if (gz > 32767)  gz -= 65536;

	gyro[0] = gx*gyro_resolution;   // renge +-125 deg/s
	gyro[1] = gy*gyro_resolution;   // renge +-125 deg/s
	gyro[2] = gz*gyro_resolution;   // renge +-125 deg/s
}

void bmx055::readTemp() {
	int8_t data[1];
	uint8_t sub_addr[1] = {0x08};
	int ack;
	
	ack = i2c_write_blocking(i2c_ch, BMX055_ACCL_ADDRESS, sub_addr, 1, true);
	// Read 6 bytes of data
	if(ack == 1) i2c_read_blocking(i2c_ch, BMX055_ACCL_ADDRESS, data, 1, false);
	// Convert the data to 8-bits
	chip_temp = data[0]*0.5 + 23;

}

void bmx055::readData() {
	readAccl();
	readGyro();
	readTemp();
}

void bmx055::getAcclData(double data[3]) {
	data[0] = accl[0];
	data[1] = accl[1];
	data[2] = accl[2];
}

void bmx055::getGyroData(double data[3]) {
	data[0] = gyro[0];
	data[1] = gyro[1];
	data[2] = gyro[2];
}

void bmx055::getTempData(double *temp) {
	*temp = chip_temp;
}

void bmx055::getData(double accl_data[3], double gyro_data[3] double *temp) {
	getAcclData(accl_data);
	getGyroData(gyro_data);
	getTempData(temp);
}

}//end namespace bmx055

