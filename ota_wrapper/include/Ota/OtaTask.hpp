#pragma once

#include "RtosUtils/SingleShootTask.hpp"

#include "freertos/event_groups.h"

#include <string>

#include "esp_system.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"

class OtaTask : public SingleShootTask
{
public:
    enum OtaBit{
        Performed = BIT0
    };

    OtaTask();

    void setHost(std::string host);

    void setBinaryName(std::string binaryName);

    esp_err_t validateImageHeader(esp_app_desc_t *new_app_info);

    void finishOta(esp_err_t err);

protected:
    virtual void task() override;

private:
    std::string _host;
    std::string _binaryName;

    static esp_https_ota_handle_t _Https_ota_handle;
};