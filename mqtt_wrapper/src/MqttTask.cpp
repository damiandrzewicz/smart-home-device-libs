#include "Mqtt/MqttTask.hpp"

#include <functional>

#include "esp_log.h"

#include "Mqtt/MessageOut.hpp"
#include "Mqtt/MqttException.hpp"

static const char *TAG = "Mqtt";

using namespace std::placeholders;

typedef std::function<void(void*, esp_event_base_t, int32_t, void*)> EventCallback;

//Callback wrappers
namespace{
    static EventCallback eventCallback;
    extern "C"{
        static void eventCallbackWrapper(void *a, esp_event_base_t b, int32_t c, void * d){
                eventCallback(a, b, c, d);
        }
    }  
}

MqttTask::MqttTask(MessagesContainer &messagesContainer) 
    :   SingleShootTask("MqttTask", 4, 1024 * 6), 
        _messagesContainer(messagesContainer)
{
    _broker_url = CONFIG_BROKER_URL;
    setClientId("test_mqtt_cl_id");
    //_pIncomingMessagesQueue = std::make_shared<MapTaskSafe<int, std::shared_ptr<MessageIn>>>();

#ifdef CONFIG_BROKER_USE_PASSWORD
    _use_credentials = true;
    _login = CONFIG_BROKER_LOGIN;
    _password = CONFIG_BROKER_PASSWORD;
#endif

}

void MqttTask::setClientId(const std::string &id)
{
    _client_id = id;
}

void MqttTask::send(const MessageOut & msgOut)
{
    ESP_LOGD(TAG, "Sending: topic: %s, data: %s, qos: %d, retain: %d", msgOut.topic.c_str(), msgOut.data.c_str(), msgOut.qos, msgOut.retain);
    int msg_id = esp_mqtt_client_publish(_client, msgOut.topic.c_str(), msgOut.data.c_str(), 0, static_cast<int>(msgOut.qos), static_cast<int>(msgOut.retain));
    if(msg_id == -1)
    {
        std::string err = "Cannot send message, id: " + std::to_string(msg_id);
        ESP_LOGW(TAG, "%s", err.c_str());
        throw MqttException(err.c_str());
    }
}

void MqttTask::initTask()
{
    ESP_LOGI(TAG, "Initialising mqtt...");

    if(_broker_url.empty())
    {
        ESP_LOGE(TAG, "Empty broker url");
        return;
    }

    ESP_LOGI(TAG, "Starting mqtt client, id: [%s], broker: [%s]", _client_id.c_str(), _broker_url.c_str());

    esp_mqtt_client_config_t config = {};
    config.uri = _broker_url.c_str();
    config.client_id = _client_id.c_str();
    config.disable_auto_reconnect = false;

    if(_use_credentials)
    {
        config.username = _login.c_str();
        config.password = _password.c_str();
        ESP_LOGD(TAG, "Authenticating user: [%s]", _login.c_str());
    }

    _client = esp_mqtt_client_init(&config);

    ESP_LOGI(TAG, "MQTT client created!");

    eventCallback = std::bind(&MqttTask::onEvent, this, _1, _2, _3, _4);

    esp_mqtt_client_register_event(
        _client, 
        static_cast<esp_mqtt_event_id_t>(ESP_EVENT_ANY_ID), 
        eventCallbackWrapper, 
        _client);

    esp_mqtt_client_start(_client);

    ESP_LOGI(TAG, "MQTT client initialised!");
   
}

void MqttTask::task()
{

}

/************************************/
//Events
/************************************/

void MqttTask::onEvent(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = static_cast<esp_mqtt_event_handle_t>(event_data);
    ESP_LOGD(TAG, "event dispatched from event loop base=%s, event_id=%d", base, event_id);

    //ESP_LOGD(TAG, "sunscr size: %d", subscribtions.size());
    int msg_id ;
     switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            //xEventGroupSetBits(this->eventBits, Status::Connected);
            //doSubscribtions();
            //doAutoPublications();

            msg_id = esp_mqtt_client_subscribe(_client, "/topic/qos0", 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            onConnected();
            break;
        case MQTT_EVENT_DISCONNECTED:
            //xEventGroupClearBits(this->eventBits, Status::Connected);
            //stopAllSubscribtions();
            onDisconnected();
            break;
        case MQTT_EVENT_SUBSCRIBED:
            //confirmSubscribtion(event->msg_id);
            onSubscribed(event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            //cancelSubscribtion(event->msg_id);
            onUnsubscribed(event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            onPublished(event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            onData(
                event->msg_id, 
                std::string(event->topic, event->topic_len ), 
                std::string(event->data, event->data_len), 
                event->total_data_len);
            break;
        case MQTT_EVENT_ERROR:
            onError();
            break;
        default:
            onUnhandled(event->event_id);
            break;
     }
}

void MqttTask::onConnected()
{
    ESP_LOGD(TAG, "onConnected...");
}

void MqttTask::onDisconnected()
{
    ESP_LOGD(TAG, "onDisconnected...");
}

void MqttTask::onSubscribed(int msgId)
{
    ESP_LOGD(TAG, "onSubscribed...");
}

void MqttTask::onUnsubscribed(int msgId)
{
    ESP_LOGD(TAG, "onUnsubscribed...");    
}

void MqttTask::onPublished(int msgId)
{
    ESP_LOGD(TAG, "onPublished...");
}

void MqttTask::onData(int msgId, std::string topic, std::string data, int totalDataLen)
{
    ESP_LOGD(TAG, "onData");

    ESP_LOGD(TAG, "Received message: id: %d topic: %s (%d bytes), data: %s (%d bytes), totalDataLen: %d",
        msgId, topic.c_str(), topic.length(), data.c_str(), data.length(), totalDataLen);

    MessagesContainer::IncomingMessages &incomingMessages = _messagesContainer.getIncomingMessages();

    std::shared_ptr<MessageIn> message = nullptr;

    for(int i = 0; i < incomingMessages.size(); i++)
    {
        auto msg = incomingMessages.at(i);
        if(msg && msg->id == msgId){ message = msg; break; }
    }

    if(!message)
    { 
        message = std::make_shared<MessageIn>(msgId);
        incomingMessages.push_back(message);
        message->topic = topic;
    }

    message->data += data;

    if(message->data.length() == totalDataLen)
    { 
        ESP_LOGD(TAG, "Setting msg (id: %d) to ready", msgId);
        message->ready = true; 
    }
}

void MqttTask::onError()
{    ESP_LOGD(TAG, "onError...");
}

void MqttTask::onUnhandled(int eventId)
{
    ESP_LOGD(TAG, "onUnhandled...");
}