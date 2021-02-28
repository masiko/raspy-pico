#include<stdio.h>
#include"pico/stdlib.h"
#include"hardware/i2c.h"

namespace bmx055
{
#define BMX055_ACCL_ADDRESS (0x19)
#define BMX055_GYRO_ADDRESS (0x69)
#define BMX055_MAG_ADDRESS (0x13)

class bmx055 {
private:
	i2c_inst_t *i2c_ch;
	double accl_resolution;
	double gyro_resolution;
	double accl[3];
	double gyro[3];
	double chip_temp;
	void readAccl();
	void readGyro();
	void readTemp();
	void readData();
public:
	bmx055(i2c_inst_t *ch);
	~bmx055(){};
	void imuInitialize();
	void getAcclData(double data[3]);
	void getGyroData(double data[3]);
	void getTempData(double *temp);
	void getData(double accl_data[3], double gyro_data[3], double *temp);
};
}//end namespace bmx055
