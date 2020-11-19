#pragma once

#include <string>

namespace SmartDevice
{
    class DeviceType
    {
    public:
        enum class Value
        {
            Unknown,
            Template,
            RelayDriver,
            PwmDriver,
            TemperatureSensor
        };

        DeviceType(){}

        DeviceType(Value value)
            : _value(value){}

        DeviceType(const DeviceType &deviceType){
            _value = deviceType.getValue();
        }
        DeviceType &operator=(const DeviceType &deviceType){
            _value = deviceType.getValue();
            return *this;
        }

        Value getValue() const {return _value;}

        bool operator==(DeviceType type){return type.getValue() == _value;}

        auto toString() const {
            if(_value == Value::Template) return "template";
            else if(_value == Value::RelayDriver) return "relay_driver";
            else if(_value == Value::PwmDriver) return "pwm_driver";
            else if(_value == Value::TemperatureSensor) return "temperature_sensor";
            else return "Unknown";
        }

    private:
        Value _value = Value::Unknown;
    };
};
