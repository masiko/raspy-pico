#include<stdio.h>
#include"pico/stdlib.h"
#include"hardware/i2c.h"

namespace aqm1602
{
#define AQM1602_DEV_ADDRESS (0x3E)

class aqm1602 {
private:
	//i2c init setting
	void i2cInitialize(uint8_t ch, unsigned int baudrate, uint8_t sda, uint8_t scl);
	//write "Command" to LCD
    void writeCmd(uint8_t cmd);
	//write "Data" to LCD
    void writeData(uint8_t data);
public:
    aqm1602();
    aqm1602(uint8_t ch, unsigned int baudrate, uint8_t sda, uint8_t scl);
    ~aqm1602() {};
	//LCD init setting
    void lcdInitialize();
	//display data to ddram address
    int8_t displayData(uint8_t data, uint8_t display_addr);
	//display data to [col, row]
    int8_t displayData(uint8_t data, uint8_t row, uint8_t col);
    int8_t displayNumeric(uint8_t num, uint8_t row, uint8_t col);
    int8_t displaySign(char sign, uint8_t row, uint8_t col);
    int8_t displayClear();
    int8_t displayClear(uint8_t row, uint8_t col);
	int8_t cvtNum2LcdInput(int num, char *sign, uint8_t out[5]);
};

}//end namespace aqm1602
