#include<stdio.h>
#include"aqm1602.hpp"

int main() {
	// Enable UART so we can print status output
    stdio_init_all();
	// This example will use I2C0 on GPIO0 (SDA) and GPIO1 (SCL)
    i2c_init(i2c0, 10 * 1000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0);
    gpio_pull_up(1);

	// LCD setting	
	aqm1602::aqm1602 lcd;
	int num = -99;
	char sign;
	uint8_t lcd_input[5];

	for (int i=0; i<200; i++) {
		lcd.cvtNum2LcdInput(num, &sign, lcd_input);
		lcd.displaySign(sign, 0, 0);
		for (int j=0; j<5; j++) {
			lcd.displayNumeric(lcd_input[j], 0, (5-j));
		}
		num++;
		sleep_ms(10);
	}
	return 0;
}
