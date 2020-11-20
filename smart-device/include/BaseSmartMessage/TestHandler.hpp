#pragma once

#include "SmartMessage/MessageHandler.hpp"

#include "esp_log.h"

class TestHandler : public MessageHandler
{
public:
    TestHandler(){
        setQos(1);
        getTopic().setCommand("Test");
    }

protected:
    virtual void _handle() override{
        auto testValue = cJSON_GetObjectItemCaseSensitive(getDataJsonObject(), "testkey");
        if(cJSON_IsString(testValue) && testValue->valuestring)
        {
            ESP_LOGI(TAG, "testkey: %s", testValue->valuestring);
        }
    }

private:
    static constexpr const char *TAG = "TestHandler";
};