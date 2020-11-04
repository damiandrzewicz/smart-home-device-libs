#include "Network/Wifi.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "esp_log.h"

bool Wifi::started = false;

Wifi::Wifi()
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    this->config = WIFI_INIT_CONFIG_DEFAULT();
}

//Setters
void Wifi::setMode(wifi_mode_t mode)
{
    this->mode = mode;
}

void Wifi::setSsid(const std::string &ssid, const std::string &password)
{
    this->ssid = ssid;
    this->password = password;
}

//Getters
std::string Wifi::getSsid()
{
    return this->ssid;
}

std::string Wifi::getPassword()
{
    return this->password;
}

/**
 * Operations
 */ 

void Wifi::init()
{
    const char *TAG = "WiFi::init";

    ESP_ERROR_CHECK(esp_wifi_init(&(this->config)));
    ESP_ERROR_CHECK(esp_wifi_set_mode(this->mode));
}

void Wifi::start()
{
    const char *TAG = "WiFi::start";

    if(this->started)
    {
        WiFiException ex("WiFi already started!");
        ESP_LOGI(TAG, "%s", ex.what());
        throw ex;
    }

    ESP_ERROR_CHECK(esp_wifi_start());
    started = true;
}

void Wifi::stop()
{
    const char *TAG = "WiFi::stop";

    if(!this->started)
    {
        WiFiException ex("WiFi already stopped!");
        ESP_LOGI(TAG, "%s", ex.what());
        throw ex;
    }

    ESP_ERROR_CHECK(esp_wifi_stop());
    this->started = false;
}



// bool WiFi::isStationConnected()
// {
//     EventBits_t bits = xEventGroupGetBits(s_WifiEventGroup);
//     return (bits & s_StationConnectedBit);
// }

// bool WiFi::waitStationConnected()
// {
//     EventBits_t bits = xEventGroupWaitBits(s_WifiEventGroup,
//         s_StationConnectedBit,
//         pdFALSE,
//         pdFALSE,
//         portMAX_DELAY);

//     return (bits & s_StationConnectedBit);
// }

// tcpip_adapter_ip_info_t WiFi::getStationTcpIpInfo()
// {
//     tcpip_adapter_ip_info_t if_info;
//     tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &if_info);
//     return if_info;
// }

