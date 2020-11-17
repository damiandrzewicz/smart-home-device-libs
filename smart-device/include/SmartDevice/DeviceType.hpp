#pragma once

#include <string>

namespace SmartDevice
{
    class DeviceType
    {
    public:
        enum class Type
        {
            Unknown,
            Template,
            RelayDriver,
            PwmDriver,
            TemperatureSensor
        };

        DeviceType(Type type)
            : _type(type){}

        auto getType(){return _type;}

        bool operator==(Type type){return type.getType() == _type;}

        auto toString(Type type){
            if(type == Type::Template) return "Template";
            else if(type == Type::RelayDriver) return "RelayDriver";
            else if(type == Type::PwmDriver) return "PwmDriver";
            else if(type == Type::TemperatureSensor) return "TemperatureSensor";
            else return "Unknown";
        }

    private:
        Type _type = Type::Unknown;
    };
};
