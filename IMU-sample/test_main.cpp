#include<stdio.h>
#include"AQM1602-ST7032/aqm1602.hpp"
#include"BMX055/bmx055.hpp"

#define INTERVAL 50
bmx055::bmx055 imu;
aqm1602::aqm1602 lcd;

bool imu_callback(struct repeating_timer *t) {
	// read data
	imu.readDataCallback();
	printf("a");	
	return true;
}

bool lcd_callback(struct repeating_timer *t) {
//	lcd.displaySign('+', 0, 0);
//	lcd.displaySign('+', 0, 14);
	static double angle_data[3];
	int angle = 0;
	char sign;
	uint8_t lcd_input[5];

	imu.getAnglData(angle_data);
	
	for (int i=0; i<3; i++) {
//		printf("%d( ",i);
		printf("+-");
		angle = (int)angle_data[i];
		lcd.cvtNum2LcdInput(angle, &sign, lcd_input);
		lcd.displaySign(sign, 0, i*5);
		for (int j=0; j<4; j++) {
			printf("%d",j);
			lcd.displayNumeric(lcd_input[j], 0, 5*i+4-j);
			printf("%d ",j);
		}
		printf(")");
	}	
	printf("\n");

	return true;
}

int main() {
	// Enable UART so we can print status output
	stdio_init_all();
	// I2C setting
	i2c_init(i2c0, 20000);
	gpio_set_function(0, GPIO_FUNC_I2C);
	gpio_set_function(1, GPIO_FUNC_I2C);
	gpio_pull_up(0);
	gpio_pull_up(1);
	
	// IMU setting	
	imu.Initialize(i2c0, INTERVAL);
	// LCD setting
	lcd.Initialize(i2c0);
	
	// Timer setting
	struct repeating_timer imu_timer;
	add_repeating_timer_ms(INTERVAL, imu_callback, NULL, &imu_timer);

	struct repeating_timer lcd_timer;
	add_repeating_timer_ms(1000, lcd_callback, NULL, &lcd_timer);
		
	while(1) {}

	return 0;
}
