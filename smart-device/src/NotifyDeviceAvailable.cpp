#include "BaseSmartMessage/NotifyDeviceAvailable.hpp"
#include "SmartMessage/MessageTopic.hpp"
#include "SmartMessage/MessageDomain.hpp"
#include "SystemUtils/Utils.hpp"

#include "SmartMessage/MessageTopicProcessor.hpp"

namespace BaseSmartMessage
{
    NotifyDeviceAvailable::NotifyDeviceAvailable()
    : MessageBuilder(1, "NotifyDeviceAvailable")
    {
        
    }

    std::shared_ptr<MqttMessage> NotifyDeviceAvailable::build()
    {
        auto msg = std::make_shared<MqttMessage>();
        msg->qos = getQos();

        MessageTopic topic(MessageDomain::MY_HOME, System::Utils::MAC::GetClientId(), getCommand(), getSubcommand());
        auto strTopic = MessageTopicProcessor::build(topic);
        msg->topic = strTopic;
        msg->data = "test data 123";
        msg->ready = true;

        return msg;
    }
};
