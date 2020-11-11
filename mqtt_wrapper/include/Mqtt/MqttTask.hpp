#pragma once

#include <string>
#include <memory>
#include <list>

#include "RtosUtils/RoutineTask.hpp"
#include "RtosUtils/SemaphoreGuard.hpp"

#include "mqtt_client.h"


#include "Mqtt/MqttMessage.hpp"

class MqttTask : public RoutineTask
{
    typedef std::list<std::shared_ptr<MqttMessage>> MessageBuffer;

public:
    MqttTask();

    void setClientId(const std::string &id);

    void send(std::shared_ptr<MqttMessage> msgOut);

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
    void processIncomingMessages();
    void processOutcomingMessages();


private:
    esp_mqtt_client_handle_t _client;

    bool _use_credentials = false;
    bool _use_ssl = false;

    std::string _broker_url;
    std::string _login;
    std::string _password;

    std::string _client_id;

    MessageBuffer _incomingMessageBuffer;
    SemaphoreHandle_t _incomingMessageMutex;

    MessageBuffer _outcomingMessageBuffer;
    SemaphoreHandle_t _outcomingMessageMutex;
};