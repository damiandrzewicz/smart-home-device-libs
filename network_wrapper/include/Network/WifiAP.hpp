//TODO implement if necessary

// Wifi::Wifi()
// {
//     ESP_ERROR_CHECK(esp_netif_init());
//     ESP_ERROR_CHECK(esp_event_loop_create_default());

//     this->config = WIFI_INIT_CONFIG_DEFAULT();
    
//     //esp_netif_create_default_wifi_ap();
// }

// void WiFi::printConnectedStationsToAp()
// {
//     wifi_sta_list_t wifi_sta_list;
//     esp_wifi_ap_get_sta_list(&wifi_sta_list);
//     tcpip_adapter_sta_list_t tcpip_adapter_sta_list;
//     tcpip_adapter_get_sta_list(&wifi_sta_list, &tcpip_adapter_sta_list);
//     for (int i=0; i<wifi_sta_list.num; i++){
//         printf("StaInfo:"MACSTR","IPSTR"",MAC2STR(wifi_sta_list.sta[i].mac),IP2STR(&tcpip_adapter_sta_list.sta[i].ip));
//     }
// }

// void WiFi::stopWifi()
// {   
//     if(!started)
//     {
//         ESP_LOGI(__PRETTY_FUNCTION__, "WiFi already stopped!");
//         return;
//     }

//     if(m_mode == WIFI_MODE_AP)
//     {
//         ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &APWifiEventHandler));
//     }
//     else if(m_mode == WIFI_MODE_STA)
//     {
//         ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &StationWifiEventHandler));
//         ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &GotIpEventHandler));
//         ESP_ERROR_CHECK(esp_wifi_disconnect());
//     }

    
//     ESP_ERROR_CHECK(esp_wifi_stop());
//     //ESP_ERROR_CHECK(esp_wifi_deinit());

//     vEventGroupDelete(s_WifiEventGroup);
//     started = false;
// }

// void WiFi::start(wifi_mode_t mode, const std::string &ssid, const std::string &password)
// {
//     const char *TAG = "WiFi::start";

//     if(started)
//     {
//         ESP_LOGI(TAG, "WiFi already started!");
//         return;
//     }

//     m_mode = mode;
//     m_ssid = ssid;
//     m_password = password;

//     s_WifiEventGroup = xEventGroupCreate();
//     m_stationAttemptToConnect = 0;

//     // if(m_mode == WIFI_MODE_AP)
//     // {
//     //     esp_netif_create_default_wifi_ap();
//     // }
//     // else if(m_mode == WIFI_MODE_STA)
//     // {
//     //     esp_netif_create_default_wifi_sta();
//     // }
        
//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&cfg));

//     ESP_ERROR_CHECK(esp_wifi_set_mode(m_mode));

//     wifi_config_t wifi_config = {};
//     if(m_mode == WIFI_MODE_AP)
//     {
//         ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &APWifiEventHandler, NULL));

//         strcpy((char*)wifi_config.ap.ssid, (char*)m_ssid.c_str());
//         wifi_config.ap.ssid_len = m_ssid.length();
//         wifi_config.ap.max_connection = m_apMaxConnections;

//         if(m_password.length())
//         {
//             wifi_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
//         } 
//         else
//         {
//             wifi_config.ap.authmode = WIFI_AUTH_OPEN;
//         }

//         strcpy((char*)wifi_config.ap.password, (char*)m_password.c_str());
//         ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
//         ESP_LOGI(__PRETTY_FUNCTION__, "Starting WifiMode=AccessPoint");
//     }
//     else if(m_mode == WIFI_MODE_STA)
//     {
//         ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &StationWifiEventHandler, NULL));
//         ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &GotIpEventHandler, NULL));

//         strcpy((char*)wifi_config.sta.ssid, (char*)m_ssid.c_str());
//         strcpy((char*)wifi_config.sta.password, (char*)m_password.c_str());
//         ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
//         ESP_LOGI(__PRETTY_FUNCTION__, "Starting WifiMode=Station. Connecting to SSID=%s", m_ssid.c_str());
//     }

//     ESP_ERROR_CHECK(esp_wifi_start());
//     started = true;
// }

/**
 * Event Handlers
 */ 
// void WiFi::APWifiEventHandler(void* arg, esp_event_base_t event_base,
//                                 int32_t event_id, void* event_data)
// {
//     if(event_id == WIFI_EVENT_AP_STACONNECTED)
//     {
//         wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
//         ESP_LOGI(__PRETTY_FUNCTION__, "station " MACSTR " join, AID=%d",
//                  MAC2STR(event->mac), event->aid);
//     } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
//         wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
//         ESP_LOGI(__PRETTY_FUNCTION__, "station " MACSTR " leave, AID=%d",
//                  MAC2STR(event->mac), event->aid);
//     }
// }