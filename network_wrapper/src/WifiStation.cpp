#include <string.h>
#include <functional>

#include "Network/WifiStation.hpp"

#include "esp_log.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sys.h"

using namespace std::placeholders;

typedef std::function<void(void*, esp_event_base_t, int32_t, void*)> BaseEventCallback;

//Callback wrappers
namespace{
    static BaseEventCallback eventCallback;
    extern "C"{
        static void eventCallbackWrapper(void *a, esp_event_base_t b, int32_t c, void * d){
            eventCallback(a, b, c, d);
        }
    }  
}

WifiStation::WifiStation()
{
    this->stationEvents = xEventGroupCreate();
    esp_netif_create_default_wifi_sta();
    setMode(WIFI_MODE_STA);
}

void WifiStation::start()
{
    const char *TAG = "WifiStation::start";

    Wifi::init();

    wifi_config_t wifi_config = {};

    //Register main event dispatcher
    eventCallback = std::bind(&WifiStation::onEvent, *this, _1, _2, _3, _4);

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, eventCallbackWrapper, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, eventCallbackWrapper, NULL));

    strcpy((char*)wifi_config.sta.ssid, (char*)getSsid().c_str());
    strcpy((char*)wifi_config.sta.password, (char*)getPassword().c_str());
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_LOGI(TAG, "starting mode=[station], connecting to SSID=%s", getSsid().c_str());

    Wifi::start();
}

void WifiStation::stop()
{
    const char *TAG = "WiFi::stop";

    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, eventCallbackWrapper));
    ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, eventCallbackWrapper));
    ESP_ERROR_CHECK(esp_wifi_disconnect());

    Wifi::stop();
}

bool WifiStation::isStarted(bool block)
{
    TickType_t tickstoWait = block ? portMAX_DELAY : 0;

    EventBits_t bits = xEventGroupWaitBits(
        this->stationEvents,
        StationBit::Started,
        pdFALSE,
        pdFALSE,
        tickstoWait);

    return (bits & StationBit::Started);
}

bool WifiStation::isConnected(bool block)
{
    TickType_t tickstoWait = block ? portMAX_DELAY : 0;

    EventBits_t bits = xEventGroupWaitBits(
        this->stationEvents,
        StationBit::Connected,
        pdFALSE,
        pdFALSE,
        tickstoWait);

    return (bits & StationBit::Connected);
}

std::string WifiStation::getIp()
{
    return this->ip;
}

void WifiStation::onEvent(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    //const char *TAG = "WifiStation::onEvent";

    if (base == WIFI_EVENT && event_id == WIFI_EVENT_SCAN_DONE) 
    {
        onScanDone();
    }
    else if (base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) 
    {
        onStart();
    } 
    else if (base == WIFI_EVENT && event_id == WIFI_EVENT_STA_STOP) 
    {
        onStop();
    }
    else if (base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) 
    {
        onConnected();
    }
    else if (base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) 
    {
        onDisconnected();
    } 
    else if (base == WIFI_EVENT && event_id == WIFI_EVENT_STA_AUTHMODE_CHANGE) 
    {
        onAuthmodeChanged();
    }
    else if (base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        onGotIp(ip4addr_ntoa(reinterpret_cast<const ip4_addr_t*>(&event->ip_info.ip)));
    }
    else if (base == IP_EVENT && event_id == IP_EVENT_GOT_IP6) 
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        onGotIpv6(ip4addr_ntoa(reinterpret_cast<const ip4_addr_t*>(&event->ip_info.ip)));
    }
}

void WifiStation::onScanDone()
{
    const char *TAG = "WifiStation::onScanDone";
    ESP_LOGI(TAG, "event...");
}

void WifiStation::onStart()
{
    const char *TAG = "WifiStation::onStart";
    ESP_LOGI(TAG, "event...");

    ESP_ERROR_CHECK(esp_wifi_connect());
    xEventGroupSetBits(this->stationEvents, StationBit::Started);
}

void WifiStation::onStop()
{
    const char *TAG = "WifiStation::onStop";
    ESP_LOGI(TAG, "event...");
}

void WifiStation::onConnected()
{
    const char *TAG = "WifiStation::onConnected";
    ESP_LOGI(TAG, "event...");
}

void WifiStation::onDisconnected()
{
    const char *TAG = "WifiStation::onDisconnected";
    ESP_LOGI(TAG, "event...");

    ESP_ERROR_CHECK(esp_wifi_connect());
}

void WifiStation::onAuthmodeChanged()
{
    const char *TAG = "WifiStation::onAuthmodeChange";
    ESP_LOGI(TAG, "event...");
}   

void WifiStation::onGotIp(const std::string &ip)
{
    const char *TAG = "WifiStation::onGotIp";
    ESP_LOGI(TAG, "event...");
    ESP_LOGI(TAG, "assigned ip=[%s]", ip.c_str());
    this->ip = ip;
    xEventGroupSetBits(this->stationEvents, StationBit::Connected);
}

void WifiStation::onGotIpv6(const std::string &ip)
{
    const char *TAG = "WifiStation::onGotIpv6";
    ESP_LOGI(TAG, "event...");
    ESP_LOGI(TAG, "assigned ipv6=[%s]", ip.c_str());
    this->ip = ip;
    xEventGroupSetBits(this->stationEvents, StationBit::Connected);
}