#include"bmx055.hpp"

namespace bmx055
{

bmx055::bmx055(i2c_inst_t *ch, int delay_ms) {
	i2c_ch = ch;
	measurment_interval = delay_ms/1000.0;
	ma_id = 0;
	imuInitialize();
	warmingUp();
}

bmx055::bmx055() {}

void bmx055::Initialize(i2c_inst_t *ch, int delay_ms) {
	i2c_ch = ch;
	measurment_interval = delay_ms/1000.0;
	ma_id = 0;
	imuInitialize();
	warmingUp();
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

void bmx055::readAccl() {
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

void bmx055::readGyro() {
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
	uint8_t data[1];
	uint8_t sub_addr[1] = {0x08};
	int ack;
	int temp;
	
	ack = i2c_write_blocking(i2c_ch, BMX055_ACCL_ADDRESS, sub_addr, 1, true);
	// Read 6 bytes of data
	if(ack == 1) i2c_read_blocking(i2c_ch, BMX055_ACCL_ADDRESS, data, 1, false);
	// Convert the data to 8-bits
	if(data[0] > 128)	temp = data[0] - 256;
	else				temp = data[0];
	chip_temp = temp*0.5 + 23;
}

void bmx055::warmingUp() {
	for (int i=0; i<3; i++) {
		gyro_delta_integra[i] = 0.0;
		gyro_offset[i] = 0.0;
		gyro_moving_average[i] = 0.0;
		angle[i] = 0.0;
	}

	sleep_ms(2000);
	for (int i=0; i<400; i++) {
		readData();
		for (int j=0; j<3; j++) {
			gyro_delta[j] = gyro[j] - gyro_past[j];
			gyro_delta_integra[j] += gyro_delta[j];
			gyro_offset[j] += gyro_delta_integra[j];
			gyro_past[j] = gyro[j];
		}
		sleep_ms(measurment_interval);
	}

	for (int i=0; i<3; i++) {
		gyro_delta_integra[i] -= gyro_offset[i] / 400;
	}

	for (ma_id=0; ma_id<5; ma_id++) {
		readData();
		for (int j=0; j<3; j++) {
			gyro_delta[j] = gyro[j] - gyro_past[j];
			gyro_delta_integra[j] += gyro_delta[j];
			gyro_moving_average_list[j][ma_id] = gyro_delta_integra[j];
			gyro_moving_average[j] += gyro_delta_integra[j];
			gyro_past[j] = gyro[j];
		}
		sleep_ms(measurment_interval);
	}
	for (int i=0; i<3; i++) {
		gyro_moving_average[i] /= 5.0;
	}
	ma_id = 0;
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

void bmx055::getAnglData(double data[3]) {
	data[0] = angle[0];
	data[1] = angle[1];
	data[2] = angle[2];
}

void bmx055::getData(double accl_data[3], double gyro_data[3], double *temp) {
	getAcclData(accl_data);
	getGyroData(gyro_data);
	getTempData(temp);
}

void bmx055::readDataCallback() {
	readData();
	for (int i=0; i<3; i++) {
		// delta & integara
		gyro_delta[i] = gyro[i] - gyro_past[i];
		gyro_delta_integra[i] += gyro_delta[i];
		// moving average
		gyro_moving_average[i] += (gyro_delta_integra[i] - gyro_moving_average_list[i][ma_id]) / 5.0;
		gyro_moving_average_list[i][ma_id] = gyro_delta_integra[i];
		// culc ahgle
		angle[i] += gyro_delta_integra[i]*measurment_interval;
		//angle[i] += gyro_moving_average[i]*measurment_interval;
		gyro_past[i] = gyro[i];
	}
	(ma_id < 4) ? ma_id++ : ma_id = 0;
	printf("%f,%f,%f,%f,%f,%f,%f,%d\n", 
		angle[0], angle[1], angle[2], gyro_delta_integra[0], gyro_delta_integra[1], gyro_delta_integra[2], chip_temp, ma_id);
}

}//end namespace bmx055

