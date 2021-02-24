#include<stdio.h>
#include"aqm1602.hpp"

int main() {
	// Enable UART so we can print status output
    stdio_init_all();
	// i2c init setting
	i2c_init(i2c0, 10000);
	gpio_set_function(0, GPIO_FUNC_I2C);
	gpio_set_function(1, GPIO_FUNC_I2C);
	gpio_pull_up(0);
	gpio_pull_up(1);
	// LCD setting
	aqm1602::aqm1602 lcd(i2c0);
	int num = -99;
	char sign;
	uint8_t lcd_input[5];

	//output -99~+100
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
