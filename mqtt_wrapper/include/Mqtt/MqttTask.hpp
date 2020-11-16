#pragma once

#include <string>
#include <memory>
#include <list>

#include "RtosUtils/RoutineTask.hpp"
#include "RtosUtils/SemaphoreGuard.hpp"

#include "mqtt_client.h"


#include "Mqtt/MqttMessage.hpp"

#include <functional>

class MqttTask : public RoutineTask
{
    typedef std::list<std::shared_ptr<MqttMessage>> MessageBuffer;

public:
    MqttTask();

    void setClientId(const std::string &id);

    void appendMessage(std::shared_ptr<MqttMessage> msgOut);
    void appendSubscribtion(std::shared_ptr<MqttMessage> msgSubscr);

    void setMessageProcessor(std::function<void(std::shared_ptr<MqttMessage>)> fun);

    void setOnConnectedCallback(auto clbck);
    void setOnDisconnectedCallback(auto clbck);
    void setOnSubscribedCallback(auto clbck);
    void setOnUnsubscribedCallback(auto clbck);
    void setOnPublishedCallback(auto clbck);
    void setOnErrorCallback(auto clbck);
    void setOnUnhandledCallback(auto clbck);
    
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
    void send(std::shared_ptr<MqttMessage> msg);
    void subscribe(std::shared_ptr<MqttMessage> msg);

    void doSubscribtions();


private:
    esp_mqtt_client_handle_t _client;

    bool _use_credentials = false;
    bool _use_ssl = false;

    std::string _broker_url;
    std::string _login;
    std::string _password;

    std::string _client_id;

    static constexpr const int MaxBufferSize = 10;
    static constexpr const int MaxTimesNotReady = 15;

    MessageBuffer _incomingMessageBuffer;
    SemaphoreHandle_t _incomingMessageMutex;

    MessageBuffer _outcomingMessageBuffer;
    SemaphoreHandle_t _outcomingMessageMutex;

    std::function<void(std::shared_ptr<MqttMessage>)> _messageProcessor;

    std::function<void()> _onConnectedCallback;
    std::function<void()> _onDisconnectedCallback;
    std::function<void()> _onSubscribedCallback;
    std::function<void()> _onUnsubscribedCallback;
    std::function<void()> _onPublishedCallback;
    std::function<void()> _onErrorCallback;
    std::function<void()> _onUnhandledCallback;

    std::list<std::shared_ptr<MqttMessage>> _subscribtions;
};