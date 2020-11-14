#include "Ota/OtaTask.hpp"

#include <string.h>


#include "esp_event.h"
#include "esp_log.h"



#include "nvs.h"
#include "nvs_flash.h"

#define ASD "https://192.168.1.20:8070/test.bin"


esp_https_ota_handle_t OtaTask::_Https_ota_handle = NULL;

static const char *TAG = "Ota";
extern const uint8_t server_cert_pem_start[] asm("_binary_ca_cert_ota_pem_start");
extern const uint8_t server_cert_pem_end[] asm("_binary_ca_cert_ota_pem_end");

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id) {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
        break;
    }
    return ESP_OK;
}

OtaTask::OtaTask() : SingleShootTask("Ota", 4, 1024 * 10)
{
    setHost(CONFIG_EXAMPLE_FIRMWARE_UPGRADE_URL);
}


void OtaTask::setHost(std::string host)
{
    _host = host;
}

void OtaTask::setBinaryName(std::string binaryName)
{
     _binaryName = binaryName;
}

esp_err_t OtaTask::validateImageHeader(esp_app_desc_t *new_app_info)
{
    if (new_app_info == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    const esp_partition_t *running = esp_ota_get_running_partition();
    esp_app_desc_t running_app_info;
    if (esp_ota_get_partition_description(running, &running_app_info) == ESP_OK) {
        ESP_LOGI(TAG, "Running firmware version: %s", running_app_info.version);
    }

#ifndef CONFIG_EXAMPLE_SKIP_VERSION_CHECK
    if (memcmp(new_app_info->version, running_app_info.version, sizeof(new_app_info->version)) == 0) {
        ESP_LOGW(TAG, "Current running version is the same as a new. We will not continue the update.");
        return ESP_FAIL;
    }
#endif

    return ESP_OK;
}

void OtaTask::finishOta(esp_err_t err)
{
    esp_err_t ota_finish_err = esp_https_ota_finish(_Https_ota_handle);
    if ((err == ESP_OK) && (ota_finish_err == ESP_OK)) {
        ESP_LOGI(TAG, "ESP_HTTPS_OTA upgrade successful. Rebooting ...");
        sleep(3000);
        esp_restart();
    } else {
        if (ota_finish_err == ESP_ERR_OTA_VALIDATE_FAILED) {
            ESP_LOGE(TAG, "Image validation failed, image is corrupted");
        }
        ESP_LOGE(TAG, "ESP_HTTPS_OTA upgrade failed %d", ota_finish_err);
    }
}

void OtaTask::task() 
{

    std::string _url = _host + "/" + _binaryName;

    ESP_LOGI(TAG, "Starting OTA...");
    ESP_LOGD(TAG, "URL: [%s]", _url.c_str());

    esp_http_client_config_t config = {};
    config.url = _url.c_str();
    config.cert_pem = (char *)server_cert_pem_start;
    config.timeout_ms = CONFIG_EXAMPLE_OTA_RECV_TIMEOUT;
    config.event_handler = _http_event_handler;
    

#ifdef CONFIG_EXAMPLE_SKIP_COMMON_NAME_CHECK
    config.skip_cert_common_name_check = true;
#endif

    esp_https_ota_config_t ota_config = {
        .http_config = &config,
    };

    esp_err_t err = esp_https_ota_begin(&ota_config, &_Https_ota_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "ESP HTTPS OTA Begin failed");
        return;
    }

    esp_app_desc_t app_desc;
    err = esp_https_ota_get_img_desc(_Https_ota_handle, &app_desc);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_https_ota_read_img_desc failed");
        finishOta(err);
        return;
    }

    err = validateImageHeader(&app_desc);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "image header verification failed");
        finishOta(err);
        return;
    }

    while (1) {
        err = esp_https_ota_perform(_Https_ota_handle);
        if (err != ESP_ERR_HTTPS_OTA_IN_PROGRESS) {
            break;
        }
        // esp_https_ota_perform returns after every read operation which gives user the ability to
        // monitor the status of OTA upgrade by calling esp_https_ota_get_image_len_read, which gives length of image
        // data read so far.
        ESP_LOGD(TAG, "Image bytes read: %d", esp_https_ota_get_image_len_read(_Https_ota_handle));
    }

    if (esp_https_ota_is_complete_data_received(_Https_ota_handle) != true) {
        // the OTA image was not completely received and user can customise the response to this situation.
        ESP_LOGE(TAG, "Complete data was not received.");
    }

    finishOta(err);
}