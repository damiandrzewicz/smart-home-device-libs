#pragma once

#include <string>

#include "Network/WifiException.hpp"

#include "esp_wifi.h"
#include "esp_event.h"

#define WIFI_SSID  CONFIG_WIFI_SSID 
#define WIFI_PASSWORD CONFIG_WIFI_PASSWORD 

class Wifi{
public: 
    Wifi();

    //Operations
    virtual void start();
    virtual void stop();

    //Setters
    void setMode(wifi_mode_t mode);
    void setSsid(const std::string &ssid, const std::string &password);

    //Getters
    std::string getSsid();

    //Events
    virtual void onEvent(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) = 0;


protected:
    void init();
    std::string getPassword();

private:
    wifi_mode_t mode;
    std::string ssid = WIFI_SSID;
    std::string password = WIFI_PASSWORD;

    wifi_init_config_t config;
    static bool started;
};