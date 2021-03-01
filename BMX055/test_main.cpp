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

	double accl[3] = {1.0, 2.0, 3.0};
	double gyro[3] = {-1.0, -2.0, -3.0};
	double temp = 0.1234;
	
	while(1) {
		imu.readData();
		imu.getData(accl, gyro, &temp);
		printf("%f,%f,%f,%f,%f,%f,%f\n", 
			accl[0], accl[1], accl[2], gyro[0], gyro[1], gyro[2], temp);
		sleep_ms(100);
	}
	return 0;
}
