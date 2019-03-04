#include <esp/uart.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "FreeRTOS.h"
#include "etstimer.h"
#include "task.h"

#include "data.h"
#include "wifi.h"
#include "display.h"
#include "weather.h"
#include "boot_errors.h"

#define WEATHER_UPDATE_INTERVAL 5000
#define TIMER_ID 1


void user_init(void){
  uart_set_baud(0, 115200);
  printf("SDK version:%s\n", sdk_system_get_sdk_version());

  display_init();
  print_text("Hello there");
  config_wifi();

  TimerHandle_t weather_timer;
  weather_timer = xTimerCreate("weather_timer", pdMS_TO_TICKS(WEATHER_UPDATE_INTERVAL), 
      pdTRUE, (void*) TIMER_ID, &update_weather_info);
  if(weather_timer == NULL)
    printf("[timer] error\n");
  else{
    printf("[timer] ok\n");

    if(xTimerStart(weather_timer, 0)!=pdPASS)
      printf("[timer start] error\n");
    else{
      printf("[timer start] ok\n");
    }
  }
}
