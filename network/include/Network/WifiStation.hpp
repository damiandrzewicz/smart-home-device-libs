#pragma once

#include "Network/Wifi.h"

#include "freertos/event_groups.h"


class WifiStation : public Wifi
{
    enum StationBit{
        Started = BIT0,
        Connected = BIT1,
    };

public:
    WifiStation();

    virtual void start() override;
    virtual void stop() override;

    bool isStarted(bool block = false);
    bool isConnected(bool block = false);

    std::string getIp();

protected:

    //Events
    virtual void onEvent(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) override;
    virtual void onScanDone();
    virtual void onStart();
    virtual void onStop();
    virtual void onConnected();
    virtual void onDisconnected();
    virtual void onAuthmodeChanged();
    virtual void onGotIp(const std::string &ip);
    virtual void onGotIpv6(const std::string &ip);


private:
    EventGroupHandle_t stationEvents;
    std::string ip;
};