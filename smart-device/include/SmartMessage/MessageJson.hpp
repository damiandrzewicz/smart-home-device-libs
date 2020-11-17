#pragma once

#include <string>

#include "esp_log.h"

#include "cJSON.h"
#include "SmartMessage/Message.hpp"

class MessageJson : public Message
{
public:
    MessageJson(){

    }

    virtual ~MessageJson(){
        clearRootJsonObject();
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

    void buildRootJsonObject(){
        _root = cJSON_CreateObject();
        if(!_root)
        {
            ESP_LOGE(TAG, "Cannot create [root] object!");
        }
    }

    void buildDataJsonObject(){
        _data = cJSON_CreateObject();
        if(!_root)
        {
            ESP_LOGE(TAG, "Cannot create [data] object!");
        }
    }

    void parseRootJsonString(const std::string &data){
        _root = cJSON_Parse(data.c_str());

        _data = cJSON_GetObjectItemCaseSensitive(_root, "data");
        //TODO handle errors
    }

    void clearRootJsonObject(){cJSON_Delete(_root);}

private:
    cJSON *_root;
    cJSON *_data;

    static constexpr const char *TAG = "MessageJson";
};