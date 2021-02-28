#include<stdio.h>
//#include"aqm1602.hpp"
#include"bmx055.hpp"

int main() {
	// Enable UART so we can print status output
    stdio_init_all();
	// LCD setting	
	bmx055::bmx055 imu(i2c0);

	double accl[3];
	double gyro[3];
	double temp;
	
	while(1) {
		imu.getData(accl, gyro, &temp);
		printf("%f,%f,%f,%f,%f,%f,%f", 
			accl[0], accl[1], accl[2], gyro[0], gyro[1], gyro[2], temp);
	}
	//output -99~+100
	return 0;
}
