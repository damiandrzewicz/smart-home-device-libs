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
        char temp[50];
        sprintf(temp, "At least 3 topic paths supported, provided %d: %s", words.size(), items.c_str());
        throw MessageTopicProcessorException(temp);
    }

    MessageTopic mt;
    if(words.size() >= 1) mt.setDomain(words.at(0));
    if(words.size() >= 2) mt.setSubdomain(words.at(1));
    if(words.size() >= 3) mt.setCommand(words.at(2));
    if(words.size() >= 4) mt.setSubcommand(words.at(3));
    return mt;
};
