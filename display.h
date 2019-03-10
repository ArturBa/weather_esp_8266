#include "weather.h"
#include <i2c/i2c.h>
#include <ssd1306/ssd1306.h>
#include <fonts/fonts.h>
#include "etstimer.h"

#define SCL_PIN 5
#define SDA_PIN 4
#define I2C_BUS 0

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#define FONT FONT_FACE_TERMINUS_6X12_ISO8859_1 

void display_init(void);

void __print_temp(const float);
void __print_weather_icon(const weather_condition);
void __print_weather(const weather*);
void update_weather_info(TimerHandle_t);

void print_text(char*);
