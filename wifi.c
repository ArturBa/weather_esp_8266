#include "wifi.h"
#include "data.h"

#include "espressif/esp_common.h"
#include <unistd.h>
#include <string.h>

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

void config_wifi(void){
    struct sdk_station_config config = {
        .ssid = WIFI_SSID,
        .password = WIFI_PASS,
    };


    /* required to call wifi_set_opmode before station_set_config */
    sdk_wifi_set_opmode(STATION_MODE);
    sdk_wifi_station_set_config(&config);
}

int __get_socket(const char* server, const char* port){
  const struct addrinfo hints = {
      .ai_family = AF_UNSPEC,
      .ai_socktype = SOCK_STREAM,
  };
  struct addrinfo *res;

  int i;
  for(i=0; i!=5; ++i){
    printf("[WiFi][__get_socket] Running DNS lookup for %s\r\n", server);
    int err = getaddrinfo(server, port, &hints, &res);

    if (err != 0 || res == NULL) {
        printf("[WiFi][__get_socket] DNS lookup failed err=%d res=%p\r\n", err, res);
        if(res){
            freeaddrinfo(res);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }else{
      break;
    }
  }
  if(i==5)
    return 0;

  struct sockaddr *sa = res->ai_addr;
  if (sa->sa_family == AF_INET) {
      printf("[WiFi][__get_socket] DNS lookup succeeded. IP=%s\r\n", inet_ntoa(((struct sockaddr_in *)sa)->sin_addr));
  }

  int s = socket(res->ai_family, res->ai_socktype, 0);
  if(s < 0) {
      printf("[WiFi][__get_socket] Failed to allocate socket.\r\n");
      freeaddrinfo(res);
      return 0;
  }
  if(connect(s, res->ai_addr, res->ai_addrlen) != 0) {
       close(s);
       freeaddrinfo(res);
       printf("[WiFi][__get_socket] Failed connect socket.\r\n");
       return 0;
   }

  return s;
}

char* get_data(const char* server, const char* port, const char* path){
  char* data = calloc(sizeof(*data), MAX_RESP);

  int s = __get_socket(server, port);
  if(s == 0){
    printf("[WiFi][get_data] Socket get failed\r\n");
    free(data);
    return NULL;
  }

  char req[200]={0};
  sprintf(req, 
      "GET %s HTTP/1.1\r\nHost: %s \r\nUser-Agent: esp-open-rtos/0.1 esp8266\r\nConnection: close\r\n\r\n", path, server);
  if (write(s, req, strlen(req)) < 0) {
    printf("[WiFi][get_data] Socket send failed. Errno: %s\r\n", strerror(errno));
    printf("%d\n", close(s));
    free(data);
    return NULL;
  }

  int r;
  do{
    r = read(s, data, MAX_RESP-1);
  }while(r>0);

  close(s);
  return data;
}
