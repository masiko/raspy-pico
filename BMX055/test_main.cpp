#include<stdio.h>
//#include"aqm1602.hpp"
#include"bmx055.hpp"

#define INTERVAL 50
bmx055::bmx055 imu;


bool timer_callback(struct repeating_timer *t) {
	//printf("timer_callback\n");
	imu.readDataCallback();
	return true;
}

int main() {
	// Enable UART so we can print status output
	stdio_init_all();
	// I2C setting
	i2c_init(i2c0, 10000);
	gpio_set_function(0, GPIO_FUNC_I2C);
	gpio_set_function(1, GPIO_FUNC_I2C);
	gpio_pull_up(0);
	gpio_pull_up(1);
	
	// IMU setting	
	imu.Initialize(i2c0, INTERVAL);
	
	// Timer setting
	struct repeating_timer timer;
	add_repeating_timer_ms(INTERVAL, timer_callback, NULL, &timer);
	
	while(1) {}

	return 0;
}
