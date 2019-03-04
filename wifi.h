#ifndef __WIFI_H_
#define __WIFI_H_

void config_wifi(void);

int __get_socket(const char*, const char*);

#define MAX_RESP 1000
char* get_data(const char*, const char*, const char*);
#endif
