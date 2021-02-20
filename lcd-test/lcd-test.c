#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define AQM1602_DEV_ADDRESS (0x3E) 

void writeCmd(uint8_t cmd) {
	uint8_t val[2];
	val[0] = 0x00;
	val[1] = cmd;
	i2c_write_blocking(i2c0, AQM1602_DEV_ADDRESS, val, 2, false);
}

void writeData(uint8_t data) {
	uint8_t val[2];
	val[0] = 0x40;
	val[1] = data;
	i2c_write_blocking(i2c0, AQM1602_DEV_ADDRESS, val, 2, false);
}

void lcd_init() {
	//start lcd init setting
	sleep_ms(40);
	//Function set
	writeCmd(0x38);
	sleep_us(27);
	//Function set
	writeCmd(0x39);
	sleep_us(27);
	//Internal OSC frequency
	writeCmd(0x14);
	sleep_us(27);
	//Contrast set
	writeCmd(0x78);
	sleep_us(27);
	//Power/IOCN/Contrast control
	writeCmd(0x56);
	sleep_us(27);
	//Follower control
	writeCmd(0x6C);
	sleep_ms(200);
	//Function set
	writeCmd(0x38);
	sleep_us(27);
	//Display ON/OFF control
	writeCmd(0x0C);
	sleep_us(27);
	//Clear Display
	writeCmd(0x01);
	sleep_ms(2);
}


int main() {
    // Enable UART so we can print status output
    stdio_init_all();

    // This example will use I2C0 on GPIO0 (SDA) and GPIO1 (SCL)
    i2c_init(i2c0, 10 * 1000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0);
    gpio_pull_up(1);

	lcd_init();

	//display
	uint8_t val;
	//cmd: set ddram address [0,0]
	writeCmd(0x80);
	//data: set data "0"
	writeData(0x30);

    return 0;
}

