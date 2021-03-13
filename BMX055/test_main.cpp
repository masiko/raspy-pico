#include<stdio.h>
//#include"aqm1602.hpp"
#include"bmx055.hpp"

int main() {
	// Enable UART so we can print status output
    stdio_init_all();
	// I2C setting
	i2c_init(i2c0, 10000);
	gpio_set_function(0, GPIO_FUNC_I2C);
	gpio_set_function(1, GPIO_FUNC_I2C);
	gpio_pull_up(0);
	gpio_pull_up(1);
	// LCD setting	
	bmx055::bmx055 imu(i2c0);

	double accl[3];
	double gyro[3];
	double temp;
	double gyro_past[3] = {0.0,0.0,0.0};
	double gyro_delta[3];
	double gyro_delta_integra[3] = {0.0,0.0,0.0};
	double gyro_offset[3];
	double angle[3] = {0.0,0.0,0.0};
	
	sleep_ms(5000);
		
	for (int i=0; i<200; i++) {
		imu.readData();
		imu.getData(accl, gyro, &temp);
		for (int j=0; j<3; j++) {
			gyro_delta[j] = gyro[j] - gyro_past[j];
			gyro_delta_integra[j] += gyro_delta[j];
			gyro_offset[j] += gyro_delta_integra[j];
			gyro_past[j] = gyro[j];
		}
		sleep_ms(100);
	}

	for (int i=0; i<3; i++) {
		gyro_delta_integra[i] += -gyro_offset[i] / 200;
	}

	while(1) {
		imu.readData();
		imu.getData(accl, gyro, &temp);
		for (int i=0; i<3; i++) {
			gyro_delta[i] = gyro[i] - gyro_past[i];
			gyro_delta_integra[i] += gyro_delta[i];
			angle[i] += gyro_delta_integra[i]*0.1;
			gyro_past[i] = gyro[i];
		}
		printf("%f,%f,%f,%f,%f,%f,%f\n", 
			angle[0], angle[1], angle[2], gyro[0], gyro[1], gyro[2], temp);
		/*printf("%f,%f,%f,%f,%f,%f,%f\n", 
			accl[0], accl[1], accl[2], gyro[0], gyro[1], gyro[2], temp);*/
		sleep_ms(100);
	}
	return 0;
}
