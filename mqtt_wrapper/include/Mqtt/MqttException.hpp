#pragma once

#include <exception>

class MqttException : public std::exception
{
public:
    MqttException(const char *msg){ _msg = msg; }

    const char * what (){ return _msg; }
private:
    const char *_msg;
};