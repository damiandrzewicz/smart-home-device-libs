#pragma once

#include <string>

#include "RtosUtils/SingleShootTask.hpp"

#include "mqtt_client.h"

#include "RtosUtils/MapTaskSafe.hpp"
#include "RtosUtils/VectorTaskSafe.hpp"
#include "Mqtt/MessageIn.hpp"

class MessageOut;

class MqttTask : public SingleShootTask
{
public:
    MqttTask();

    void setClientId(const std::string &id);

    void send(const MessageOut & msgOut);

    VectorTaskSafe<std::shared_ptr<MessageIn>> &getIncomingMessages();

    void setIncomingMessages(VectorTaskSafe<std::shared_ptr<MessageIn>> inomingMessages);

    //void registerEvent(const std::string &topic)

protected:

    virtual void task() override;
    virtual void initTask() override;

    void onEvent(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
    virtual void onConnected();
    virtual void onDisconnected();
    virtual void onSubscribed(int msgId);
    virtual void onUnsubscribed(int msgId);
    virtual void onPublished(int msgId);
    virtual void onData(int msgId, std::string topic, std::string data, int totalDataLen);
    virtual void onError();
    virtual void onUnhandled(int eventId);



private:
    esp_mqtt_client_handle_t _client;

    bool _use_credentials = false;
    bool _use_ssl = false;

    std::string _broker_url;
    std::string _login;
    std::string _password;

    std::string _client_id;

    VectorTaskSafe<std::shared_ptr<MessageIn>> _incomingMessages;
};