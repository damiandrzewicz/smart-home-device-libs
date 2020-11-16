#pragma once

#include <string>

#include "esp_log.h"

#include "cJSON.h"
#include "SmartMessage/Message.hpp"

class MessageJson : public Message
{
public:
    MessageJson(int qos, const std::string command, const std::string subcommand = "")
        : Message(qos, command, subcommand)
    {
        _root = cJSON_CreateObject();
        if(!_root)
        {
            ESP_LOGE(TAG, "Cannot create [root] object!");
        }

        _data = cJSON_CreateObject();
        if(!_root)
        {
            ESP_LOGE(TAG, "Cannot create [data] object!");
        }
    }

    virtual ~MessageJson(){
        cJSON_Delete(_root);
    }

protected:
    auto getRootJsonObject(){return _root;}

    auto getDataJsonObject(){return _data;}

    auto getStringFormatted(){
        char *s = cJSON_Print(_root);
        std::string tmp(s);
        if(tmp.empty()){
            ESP_LOGE(TAG, "Failed to print formatted JSON message!");
        }
        cJSON_free(static_cast<char*>(s));
        return tmp;
    }

    auto getStringUnformatted(){
        char *s = cJSON_PrintUnformatted(_root);
        std::string tmp(s);
        if(tmp.empty()){
            ESP_LOGE(TAG, "Failed to print unformatted JSON message!");
        }
        cJSON_free(static_cast<char*>(s));
        return tmp;
    }

private:
    cJSON *_root;
    cJSON *_data;

    static constexpr const char *TAG = "MessageJson";
};