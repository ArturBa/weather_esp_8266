#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "weather.h"
#include "wifi.h"
#include "data.h"


const float __get_temp(const char* weather_data){
  float temp;
  char* ptr = strstr(weather_data, "temperature value");
  ptr = strchr(ptr, '"');
  sscanf(ptr+1, "%f", &temp);
  return temp;
}

const weather_condition __get_cond(const char* weather_data){
  int w_cond;
  char* ptr = strstr(weather_data, "icon");
  ptr = strchr(ptr, '"');

  sscanf(ptr+1, "%d", &w_cond);

  switch(w_cond){
    case 1:
      return CLEAR;
    case 2:
      return FEW_CLOUDS;
    case 3:
      return SCATTERED_CLOUDS;
    case 4:
      return BROKEN_CLOUDS;
    case 9:
      return SHOWER_RAIN;
    case 10:
      return RAIN;
    case 11:
      return THUNDERSTORM;
    case 13:
      return SNOW;
    case 50:
      return MIST;
    default:
      return 0;
  }
}

weather* get_weather(void){
  char* weather_data = get_data(WEATHER_SERVER, WEATHER_PORT, WEATHER_PATH);
  if(weather_data == NULL){
    printf("[Weather][get_weather] Socket problem\n");
    return NULL;
  }
  weather *weather_buffor = calloc(sizeof(*weather_buffor), 1);

  weather_buffor->temperature = __get_temp(weather_data);
  weather_buffor->condition = __get_cond(weather_data);

  free(weather_data);
  return weather_buffor;
}
