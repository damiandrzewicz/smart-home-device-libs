#pragma once

#include <string>
#include <exception>

#include "esp_log.h"

#include "cJSON.h"
#include "SmartMessage/Message.hpp"

class MessageJsonException : public std::exception
{
public:
    MessageJsonException(const char *msg_) : msg(msg_){}
    const char *what(){return msg;}

private:
    const char *msg = nullptr;
};

class MessageJson : public Message
{
public:
    MessageJson()
    {

    }

    virtual ~MessageJson()
    {

    }

protected:
    auto getRootJsonObject(){return _root;}

    auto getDataJsonObject(){return _data;}

    auto getStringFormatted(){
        char *s = cJSON_Print(_root);
        std::string tmp(s);
        if(tmp.empty()){
            throw MessageJsonException("Failed to print formatted JSON message!");
        }
        cJSON_free(static_cast<char*>(s));
        return tmp;
    }

    auto getStringUnformatted(){
        char *s = cJSON_PrintUnformatted(_root);
        std::string tmp(s);
        if(tmp.empty()){
            throw MessageJsonException("Failed to print unformatted JSON message!");
        }
        cJSON_free(static_cast<char*>(s));
        return tmp;
    }

    void buildRootJsonObject(){
        _root = cJSON_CreateObject();
        if(!_root)
        {
            throw MessageJsonException("Cannot create [root] object!");
        }
    }

    void buildDataJsonObject(){
        _data = cJSON_CreateObject();
        if(!_data)
        {
            throw MessageJsonException("Cannot create [data] object!");
        }
    }

    void parseRootJsonString(const std::string &data){
        _root = cJSON_Parse(data.c_str());
        if(!_root)
        {
            throw MessageJsonException("Cannot parse root string: 'root' nullptr");
        }

        _data = cJSON_GetObjectItemCaseSensitive(_root, "data");
        if(!_data)
        {
            throw MessageJsonException("Cannot parse root string: 'data' nullptr");
        }
    }

    void clearRootJsonObject(){
        if(_root)
        {
            cJSON_Delete(_root);
        }
    }

private:
    cJSON *_root = NULL;
    cJSON *_data = NULL;

    static constexpr const char *TAG = "MessageJson";
};