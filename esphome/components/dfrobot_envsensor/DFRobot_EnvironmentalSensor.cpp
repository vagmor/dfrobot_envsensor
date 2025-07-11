/*!
 * @file  DFRobot_EnvironmentalSensor.cpp
 * @brief Basic structure of DFRobot_EnvironmentalSensor class, the implementation of basic method
 *
 * @copyright	Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license   The MIT License (MIT)
 * @author    [TangJie](jie.tang@dfrobot.com)
 * @version   V1.0
 * @date      2021-08-31
 * @url       https://github.com/DFRobot/DFRobot_EnvironmentalSensor
 */

#include "DFRobot_EnvironmentalSensor.h"

#ifdef __AVR__
typedef uint16_t    platformBitWidth_t;
#else
typedef uint32_t    platformBitWidth_t;
#endif

DFRobot_EnvironmentalSensor::DFRobot_EnvironmentalSensor(uint8_t addr, TwoWire *pWire)
{
  _pWire = pWire;
  _addr = addr;
};

DFRobot_EnvironmentalSensor::DFRobot_EnvironmentalSensor(uint8_t addr, Stream *s):DFRobot_RTU(s)
{
  _s = s;
  _addr = addr;
}

int8_t DFRobot_EnvironmentalSensor::begin(void)
{
  delay(500);
  setTimeoutTimeMs(200);
  if(_addr > 0xF7){
    DBG("Invaild Device addr.");
  }
  if(_addr != 0){
    if(!detectDeviceAddress(_addr)){
      DBG("Device addr Error.");
      return -1;
    }
  }else{
    return -1;
  }
  return 0;
}

uint16_t DFRobot_EnvironmentalSensor::getDevicePID(void)
{
  uint8_t buf[2];
  if(_pWire){
    readReg(REG_PID, buf, 2);
  }else{
    readReg(REG_PID, buf, 2);
  }
  return (buf[0] << 8 | buf[1]);

}

uint16_t DFRobot_EnvironmentalSensor::getDeviceVID(void)
{
  uint8_t buf[2];
  if(_pWire){
    readReg(REG_VID, buf, 2);
  }else{
    readReg(REG_VID, buf, 2);
  }
  return (buf[0] << 8 | buf[1]);

}

float DFRobot_EnvironmentalSensor::getTemperature(uint8_t unist)
{
  uint8_t buffer[2];
  uint16_t data;
  float temp;
  readReg(REG_TEMP,buffer,2);
  data = buffer[0] << 8 | buffer[1];
  temp = (-45) +((data * 175.00) / 1024.00 / 64.00);
  if(unist == TEMP_F){
    temp = temp * 1.8 + 32 ;
  }
  return temp;
}

float DFRobot_EnvironmentalSensor::getHumidity(void)
{
  uint8_t buffer[2];
  float humidity;
  uint16_t data;
  readReg(REG_HUMIDITY,buffer, 2);
  data = ((buffer[0] << 8 | buffer[1]));
  humidity = (float)data * 100 / 65536;
  return humidity;
}

float DFRobot_EnvironmentalSensor::mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float DFRobot_EnvironmentalSensor::getUltravioletIntensity(eUVSOC soc)
{
  uint16_t uvLevel;
  uint8_t buffer[2];
  float ultraviolet;
  readReg(REG_ULTRAVIOLET_INTENSITY,buffer,2);
  uvLevel = buffer[0] << 8 | buffer[1];
  DBG(uvLevel);
  if(soc == eUVSOC::eS12SD){
    float outputVoltage = 3000.0 * uvLevel / (1024.0 * 1000.0);
    float na  = (outputVoltage * 1000000000.0) / 4303300;
    ultraviolet = na / 113.0;
  }else {
    float outputVoltage = 3.0 * uvLevel/1024;
    if(outputVoltage <= 0.99)
      outputVoltage = 0.99;
    else if(outputVoltage >= 2.99)
      outputVoltage = 2.99;
    ultraviolet = mapfloat(outputVoltage, 0.99, 2.9, 0.0, 15.0);
  }
  
  return ultraviolet;
}

float DFRobot_EnvironmentalSensor::getLuminousIntensity(void)
{
  uint16_t data;
  uint8_t buffer[2];
  readReg(REG_LUMINOUS_INTENSITY, buffer, 2);

  data = buffer[0] << 8 | buffer[1];
  float luminous = data;
	luminous = luminous * (1.0023f + luminous * (8.1488e-5f + luminous * (-9.3924e-9f + luminous * 6.0135e-13f)));
//        luminous *= 10.0f;
  return luminous;
}

uint16_t DFRobot_EnvironmentalSensor::getAtmospherePressure(uint8_t units)
{
  uint16_t atmosphere;
  uint8_t buffer[2];
  readReg(REG_ATMOSPHERIC_PRESSURE, buffer, 2); 
  atmosphere = buffer[0] << 8 | buffer[1];
  if(units == KPA){
    atmosphere /=10;
  }
  return atmosphere;
}

float DFRobot_EnvironmentalSensor::getElevation(void)
{
  uint16_t atmosphere;
  uint8_t buffer[2];
  readReg(REG_ATMOSPHERIC_PRESSURE, buffer, 2);
  atmosphere = buffer[0] << 8 | buffer[1];
 float elevation = 44330 * (1.0 - pow(atmosphere / 1015.0f, 0.1903));
  return elevation;
}

uint16_t DFRobot_EnvironmentalSensor::getUltravioletRaw() {
  uint8_t buffer[2];
  this->readReg(REG_ULTRAVIOLET_INTENSITY, buffer, 2);
  return (buffer[0] << 8) | buffer[1];
}

void DFRobot_EnvironmentalSensor::getUVRaw(uint8_t *buffer) {
  this->readReg(REG_ULTRAVIOLET_INTENSITY, buffer, 2);
}


uint8_t DFRobot_EnvironmentalSensor::readReg(uint16_t reg, void *pBuf, uint8_t size)
{
  uint8_t* _pBuf = (uint8_t*)pBuf;
  
    if(pBuf == NULL){
      DBG("data error");
      return 0;
    }
  if(_pWire){
    uint8_t _reg = reg * 2;
    _pWire->begin();
    _pWire->beginTransmission(_addr);
    _pWire->write(_reg);
    _pWire->endTransmission();
    _pWire->requestFrom(_addr, size);
    for(uint8_t i = 0; i < size; i++)
      _pBuf[i] = _pWire->read();
      
    return size;
  }else{
    return readInputRegister(_addr, reg, _pBuf, size);
  }
}
void DFRobot_EnvironmentalSensor::setUVGain(uint8_t gain) {
  uint8_t data[1] = { gain };
  writeReg(REG_UV_GAIN, data, 1);
}

void DFRobot_EnvironmentalSensor::setUVResolution(uint8_t resolution) {
  uint8_t data[1] = { resolution };
  writeReg(REG_UV_RESOLUTION, data, 1);
}

void DFRobot_EnvironmentalSensor::setUVMeasurementRate(uint8_t rate) {
  uint8_t data[1] = { rate };
  writeReg(REG_UV_MEASUREMENT_RATE, data, 1);
}

/**
 * @brief  Write raw bytes to a register (I�C or Modbus)
 * @param  reg    register address
 * @param  pBuf   pointer to the data to write
 * @param  size   number of bytes to write
 * @return I�C: result of endTransmission(); Modbus: exception code
 */
uint8_t DFRobot_EnvironmentalSensor::writeReg(uint16_t reg, const void *pBuf, uint8_t size) {
  const uint8_t* buf = (const uint8_t*)pBuf;
  if (_pWire) {
    uint8_t _reg = reg * 2;
    _pWire->begin();
    _pWire->beginTransmission(_addr);
    _pWire->write(_reg);
    for (uint8_t i = 0; i < size; i++) {
      _pWire->write(buf[i]);
    }
    return _pWire->endTransmission();
  } else {
    // For Modbus RTU: writeHoldingRegister(id, reg, value)
    // Here we assume size is even and write as uint16_t blocks:
    return this->writeHoldingRegister(_addr, reg, (uint16_t*)buf, size / 2);
  }
}

// Read raw luminous-intensity register
uint16_t DFRobot_EnvironmentalSensor::getLuminousRaw() {
  uint8_t buf[2];
  this->readReg(REG_LUMINOUS_INTENSITY, buf, 2);
  return (uint16_t)buf[0] << 8 | buf[1];
}

bool  DFRobot_EnvironmentalSensor::detectDeviceAddress(uint8_t addr)
{
   if(_pWire){
   uint8_t buf[2];
   readReg(REG_DEVICE_ADDR, buf, 2);
   if(addr == ((buf[0] << 8 | buf[1]) & 0xFF))
    return true;
    
  }else{
    uint16_t ret = readInputRegister(addr, REG_DEVICE_ADDR);
    DBG(ret);
    if((ret & 0xFF) == addr)
      return true;
  }
  return false;
  
  
}
