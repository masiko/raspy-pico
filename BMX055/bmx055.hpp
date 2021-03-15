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
	double measurment_interval;
	double accl_resolution;
	double gyro_resolution;
	double accl[3];
	double gyro[3];
	double chip_temp;
	// gyro estimate
	double gyro_past[3];
	double gyro_delta[3];
	double gyro_delta_integra[3];
	double gyro_offset[3];
	double angle[3];

	void readAccl();
	void readGyro();
	void readTemp();
	void warmingUp();
public:
	bmx055(i2c_inst_t *ch, int delay_ms);
	bmx055();
	~bmx055(){};
	void Initialize(i2c_inst_t *ch, int delay_ms);
	void imuInitialize();
	void readData();
	void getAcclData(double data[3]);
	void getGyroData(double data[3]);
	void getTempData(double *temp);
	void getAnglData(double data[3]);
	void getData(double accl_data[3], double gyro_data[3], double *temp);
	void readDataCallback();
};
}//end namespace bmx055
