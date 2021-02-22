#include<cmath>
#include"aqm1602.hpp"

namespace aqm1602
{

aqm1602::aqm1602() {
	lcdInitialize();
}

//write "Command" to LCD
void aqm1602::writeCmd(uint8_t cmd) {
	uint8_t val[2];
	val[0] = 0x00;
	val[1] = cmd;
	i2c_write_blocking(i2c0, AQM1602_DEV_ADDRESS, val, 2, false);
}

//write "Data" to LCD
void aqm1602::writeData(uint8_t data) {
	uint8_t val[2];
	val[0] = 0x40;
	val[1] = data;
	i2c_write_blocking(i2c0, AQM1602_DEV_ADDRESS, val, 2, false);
}

//LCD init setting
void aqm1602::lcdInitialize() {
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

int8_t aqm1602::displayData(uint8_t data, uint8_t display_addr = 0x00) {
    uint8_t mask = 0x80;
    uint8_t set_ddram_addr = mask | display_addr;
    writeCmd(set_ddram_addr);
    writeData(data);
    return 0;
}

int8_t aqm1602::displayData(uint8_t data, uint8_t row, uint8_t col) {
    //row: [0,1]
    //col: [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]
    uint8_t display_addr;
    
    if (row!=0 && row!=1)   return -1;
    if (16 <= col)   return -2;
    display_addr = 0x40*row + col;
    return displayData(data, display_addr);
}

int8_t aqm1602::displayNumeric(uint8_t num, uint8_t row = 0x00, uint8_t col = 0x00) {
    //num: [0,1,2,3,4,5,6,7,8,9]
    //row: [0,1]
    //col: [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]
    uint8_t data;
    uint8_t mask = 0x30;

	if (num > 9)	return -1;
    data = num | mask;
    return displayData(data, row, col);
}

int8_t aqm1602::displaySign(char sign, uint8_t row, uint8_t col) {
    //sign: [+,-]
    //row: [0,1]
    //col: [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]
    char data;
    
    if (sign == '+')        data = 0x2B;
    else if (sign == '-')   data = 0x2D;
    else                    return -1;
    return displayData(data, row, col);
}

int8_t aqm1602::displayClear() {
    writeCmd(0x01);
    return 0;
}

int8_t aqm1602::displayClear(uint8_t row, uint8_t col) {
    //row: [0,1]
    //col: [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]
    return displayData(0x20, row, col);
}

int8_t aqm1602::cvtNum2LcdInput(int num, char *sign, uint8_t out[5]) {
	//out[0] = n1 (10^0)
	//out[1] = n10 (10^1)
	//out[2] = n100 (10^2)
	//out[3] = n1000 (10^3)
	//out[4] = n10000 (10^4)
	//sign = '+','-'

	int quot[6];
	if (num > 99999)	return 1;
	else if (num < -99999)	return -1;

	quot[0] = std::abs(num);
	if (num<0)	*sign = '-';
	else		*sign = '+';

	for (int c=0; c<5; c++) {
		quot[c+1] = quot[c] / 10;
		out[c] = quot[c] % 10;
	}
	return 0;
}

}//end namespace aqm1602
