#pragma once

#include "SmartMessage/MessageBuilder.hpp"

namespace BaseSmartMessage
{
    class NotifyDeviceAvailable : public MessageBuilder
    {
    public:
        NotifyDeviceAvailable();

        virtual std::shared_ptr<MqttMessage> build() override;

    private:

    };
};
