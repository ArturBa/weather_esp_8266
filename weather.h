#include "espressif/esp_common.h"
#include "espressif/esp_common.h"
#include <unistd.h>
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include <string.h>
#ifndef __WEATHER_H__
#define __WEATHER_H__


typedef enum{
  CLEAR = 0,
  FEW_CLOUDS,
  SCATTERED_CLOUDS,
  BROKEN_CLOUDS,
  SHOWER_RAIN,
  RAIN,
  THUNDERSTORM,
  SNOW,
  MIST 
}weather_condition;

typedef struct{
  float temperature; 
  weather_condition condition;
}weather;

const float __get_temp(const char* weather_data);
const weather_condition __get_cond(const char* weather_data);

weather* get_weather();

#endif
