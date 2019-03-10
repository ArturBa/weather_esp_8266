#include "display.h"
#include "weather_icon.h"

static uint8_t *frame_buf;
static const ssd1306_t device = {
	.protocol     = SSD1306_PROTO_I2C,
	.screen       = SSD1306_SCREEN, // or SH1106_SCREEN
	.i2c_dev.bus  = I2C_BUS,
	.i2c_dev.addr = SSD1306_I2C_ADDR_0,
	.width        = DISPLAY_WIDTH,
	.height       = DISPLAY_HEIGHT
};

void display_init(void){
  i2c_init(I2C_BUS, SCL_PIN, SDA_PIN, I2C_FREQ_400K);

  if (ssd1306_init(&device)) {
    printf("[ssd1306][Init] Failure\n");
  }else{
    frame_buf = calloc(sizeof(uint8_t), DISPLAY_WIDTH*DISPLAY_HEIGHT/8); 
    ssd1306_set_whole_display_lighting(&device, false);
    printf("[ssd1306][Init] Success\n");
  }
}


void __print_temp(const float temp){
  char temp_text[5];
  sprintf(temp_text, "%3.1f'C", temp);
  ssd1306_draw_string(&device, frame_buf, 
      font_builtin_fonts[FONT], 60, 10, temp_text, 
      OLED_COLOR_WHITE, OLED_COLOR_BLACK);
}

void __print_weather_icon(const weather_condition _cond){
  volatile int i, j;
  for(i=0; i!=32; i++){
    for(j=0; j!=32; j++){
      if(weather_icon[_cond][j+32*i])
        ssd1306_draw_pixel(&device, frame_buf, j, i, OLED_COLOR_WHITE);
    }
  }
}

void __print_weather(const weather* _weather){
  bzero(frame_buf, DISPLAY_WIDTH*DISPLAY_HEIGHT/8);
  __print_temp(_weather->temperature);
  __print_weather_icon(_weather->condition);
  if(ssd1306_load_frame_buffer(&device, frame_buf))
    printf("[Ssd1306][load_frame_buffer] Error");
}

void update_weather_info(TimerHandle_t xTimer){
  printf("In update_weather_info\n");
  weather *weather_data=get_weather();
  if(weather_data == NULL){
    printf("[Ssd1306][update_weather_info] Get weather data failure\n");
    return;
  }
  __print_weather(weather_data);

  free(weather_data);
  return;
}


void print_text(char* _text){
  ssd1306_set_whole_display_lighting(&device, false);

  ssd1306_draw_string(&device, frame_buf, 
      font_builtin_fonts[FONT], 20, 20, _text, 
      OLED_COLOR_WHITE, OLED_COLOR_BLACK);
  if(ssd1306_load_frame_buffer(&device, frame_buf))
    printf("[Ssd1306][load_frame_buffer] Error");
}
