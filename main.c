#include <esp/uart.h>
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"

#include "data.h"
#include "wifi.h"
#include "display.h"
#include "weather.h"
#include "boot_errors.h"



void user_init(void){
  uart_set_baud(0, 115200);
  printf("SDK version:%s\n", sdk_system_get_sdk_version());

  display_init();
  config_wifi();

  xTaskCreate(&update_weather_info, "weather_control", 1548, NULL, 2, NULL);
}
