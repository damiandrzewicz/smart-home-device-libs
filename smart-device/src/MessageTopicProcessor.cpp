#include "SmartMessage/MessageTopicProcessor.hpp"

#include <regex>

#include "esp_log.h"

static const char *TAG = "MessageTopicProcessor";

std::string MessageTopicProcessor::build(MessageTopic messateTopic)
{
    std::string mt = messateTopic.getDomain() + "/" + messateTopic.getSubdomain() + "/" + messateTopic.getCommand();
    if(!messateTopic.getSubcommand().empty()) mt += "/" + messateTopic.getSubcommand();
    return mt;
}

MessageTopic MessageTopicProcessor::parse(std::string messageTopic)
{
    std::regex regex{R"([\/]+)"};
    std::sregex_token_iterator it{messageTopic.begin(), messageTopic.end(), regex, -1};
    std::vector<std::string> words{it, {}}; 

    std::string items;
    for(auto item : words) items += item + ", ";

    ESP_LOGD(TAG, "Parsed items: [%s]", items.c_str());
    
    if(words.size() < 3)
    {
        ESP_LOGW(TAG, "At least 3 topic paths supported, provided %d: %s", words.size(), items.c_str());
    }

    MessageTopic mt;
    if(words.size() >= 1) mt.setDomain(items.at(1));
    if(words.size() >= 2) mt.setSubdomain(items.at(2));
    if(words.size() >= 3) mt.setDomain(items.at(3));
    if(words.size() >= 4) mt.setSubcommand(items.at(4));
    return mt;
};
