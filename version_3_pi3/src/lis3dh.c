/**************************************************************************/
/*!
    @file     lis3dh.c
    @author   K. Townsend (microBuilder.eu)
    @ingroup  Sensors
    @brief    Driver for the ST LIS3DH I2C/SPI Accelerometer
    @section LICENSE
    Software License Agreement (BSD License)
    Copyright (c) 2013, K. Townsend
    All rights reserved.
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#include<stdlib.h>
#include<stdio.h> 
#include <string.h>
#include<lib_i2c.h>
#include<bcm2835.h>
#include<lis3dh.h>
#define LIS3DH_SENSITIVITY_2G  (0.001F)
#define LIS3DH_SENSITIVITY_4G  (0.002F)
#define LIS3DH_SENSITIVITY_8G  (0.004F)
#define LIS3DH_SENSITIVITY_16G (0.012F)





/**************************************************************************/
/*!
    @brief  Initialises the I2C block
*/
/**************************************************************************/

void lis3dhInit(void)
{
 
  uint8_t data[1];
  int res;
  
    
  /* Ping the I2C device first to see if it exists! */
  //ASSERT(i2cCheckAddress(LIS3DH_ADDRESS), ERROR_I2C_DEVICENOTFOUND);BCM2835_I2C_REASON_OK
  data[0]=LIS3DH_CTRL_REG1_DATARATE_50HZ | LIS3DH_CTRL_REG1_XYZEN;
  res=i2c_write_data_block(LIS3DH_ADDRESS,LIS3DH_REGISTER_CTRL_REG1,data,1);  
  /* Normal mode, 50Hz & Set data rate and power mode, and enable X/Y/Z */
  if(res != BCM2835_I2C_REASON_OK) 
    printf("REG1 not succeed\n");	  
  data[0]=LIS3DH_CTRL_REG4_BLOCKDATAUPDATE ;
  res=i2c_write_data_block(LIS3DH_ADDRESS,LIS3DH_REGISTER_CTRL_REG4,data,1);
  if(res != BCM2835_I2C_REASON_OK) 
    printf("REG4 not succeed\n");
  /* Enable block update Enable block update and set range to +/-2G */
  //return BCM2835_I2C_REASON_OK;
}

/**************************************************************************/
/*!
    @brief  Polls the device for a new X/Y/Z reading
*/
/**************************************************************************/
uint8_t lis3dhPoll(lis3dhData_t* data)
{
  uint8_t timeout = 0;
  uint8_t buffer = 0x00;
  int res;
  uint8_t _buff[6];
  

  /* For now, always read data even if it hasn't changed */
  res=i2c_read_data_block(LIS3DH_ADDRESS,LIS3DH_REGISTER_OUT_X_L, &_buff[0], 1);
  res=i2c_read_data_block(LIS3DH_ADDRESS,LIS3DH_REGISTER_OUT_X_H, &_buff[1], 1);
  res=i2c_read_data_block(LIS3DH_ADDRESS,LIS3DH_REGISTER_OUT_Y_L, &_buff[2], 1);
  res=i2c_read_data_block(LIS3DH_ADDRESS,LIS3DH_REGISTER_OUT_Y_H, &_buff[3], 1);
  res=i2c_read_data_block(LIS3DH_ADDRESS,LIS3DH_REGISTER_OUT_Z_L, &_buff[4], 1);
  res=i2c_read_data_block(LIS3DH_ADDRESS,LIS3DH_REGISTER_OUT_Z_H, &_buff[5], 1);
  data->x = (int16_t)(_buff[1] << 8 | _buff[0]);
  data->y = (int16_t)(_buff[3] << 8 | _buff[2]);
  data->z = (int16_t)(_buff[5] << 8 | _buff[4]);
  if(res != BCM2835_I2C_REASON_OK) return res;
  return BCM2835_I2C_REASON_OK;
}

/**************************************************************************/
/*!
    @brief  Provides the sensor_t data for this sensor
*/
/**************************************************************************/
#if 0
void lis3dhGetSensor(sensor_t *sensor)
{
  /* Clear the sensor_t object */
  memset(sensor, 0, sizeof(sensor_t));

  /* Insert the sensor name in the fixed length char array */
  strncpy (sensor->name, "LIS3DH", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name)- 1] = 0;
  sensor->version     = 1;
  sensor->sensor_id   = _lis3dhSensorID;
  sensor->type        = SENSOR_TYPE_ACCELEROMETER;
  sensor->min_delay   = 0;

  /* We need to do some calculations to determine resolution and maxRange in m/s^2 */
  sensor->max_value   = _lis3dhMeasurementRange * SENSORS_GRAVITY_STANDARD;
  sensor->min_value   = 0;
  switch (_lis3dhMeasurementRange)
  {
    case 16:
      sensor->resolution  = (_lis3dhMeasurementRange / 32767.0F) * LIS3DH_SENSITIVITY_16G * SENSORS_GRAVITY_STANDARD;
      break;
    case 8:
      sensor->resolution  = (_lis3dhMeasurementRange / 32767.0F) * LIS3DH_SENSITIVITY_8G * SENSORS_GRAVITY_STANDARD;
      break;
    case 4:
      sensor->resolution  = (_lis3dhMeasurementRange / 32767.0F) * LIS3DH_SENSITIVITY_4G * SENSORS_GRAVITY_STANDARD;
      break;
    case 2:
    default:
      sensor->resolution  = (_lis3dhMeasurementRange / 32767.0F) * LIS3DH_SENSITIVITY_2G * SENSORS_GRAVITY_STANDARD;
      break;
  }
}

/**************************************************************************/
/*!
    @brief  Reads the sensor and returns the data as a sensors_event_t
*/
/**************************************************************************/
void lis3dhGetSensorEvent(sensors_event_t *event)
{
  lis3dhData_t data;

  /* Clear the event */
  memset(event, 0, sizeof(sensors_event_t));

  event->version   = sizeof(sensors_event_t);
  event->sensor_id = _lis3dhSensorID;
  event->type      = SENSOR_TYPE_ACCELEROMETER;
  event->timestamp = delayGetTicks();

  /* Retrieve values from the sensor */
  ASSERT_STATUS(lis3dhPoll(&data));

  /* The LIS3DH returns a raw g value which needs to be adjusted by
   * sensitivity which is shown as mg/digit in the datasheet.  To convert
   * this to a normal g value, multiply by the appropriate sensitivity value
   * and then convert it to the m/s^2 value that sensors_event_t is expecting. */
  switch (_lis3dhMeasurementRange)
  {
    case 16:
      event->acceleration.x = data.x * LIS3DH_SENSITIVITY_16G * SENSORS_GRAVITY_STANDARD;
      event->acceleration.y = data.y * LIS3DH_SENSITIVITY_16G * SENSORS_GRAVITY_STANDARD;
      event->acceleration.z = data.z * LIS3DH_SENSITIVITY_16G * SENSORS_GRAVITY_STANDARD;
      break;
    case 8:
      event->acceleration.x = data.x * LIS3DH_SENSITIVITY_8G * SENSORS_GRAVITY_STANDARD;
      event->acceleration.y = data.y * LIS3DH_SENSITIVITY_8G * SENSORS_GRAVITY_STANDARD;
      event->acceleration.z = data.z * LIS3DH_SENSITIVITY_8G * SENSORS_GRAVITY_STANDARD;
      break;
    case 4:
      event->acceleration.x = data.x * LIS3DH_SENSITIVITY_4G * SENSORS_GRAVITY_STANDARD;
      event->acceleration.y = data.y * LIS3DH_SENSITIVITY_4G * SENSORS_GRAVITY_STANDARD;
      event->acceleration.z = data.z * LIS3DH_SENSITIVITY_4G * SENSORS_GRAVITY_STANDARD;
      break;
    case 2:
    default:
      event->acceleration.x = data.x * LIS3DH_SENSITIVITY_2G * SENSORS_GRAVITY_STANDARD;
      event->acceleration.y = data.y * LIS3DH_SENSITIVITY_2G * SENSORS_GRAVITY_STANDARD;
      event->acceleration.z = data.z * LIS3DH_SENSITIVITY_2G * SENSORS_GRAVITY_STANDARD;
      break;
  }

  return ERROR_NONE;
}
#endif
