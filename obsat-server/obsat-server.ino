/*
 * Autor: Zoh-j02r - Dept. Software Embarcado
 * Data: 2023-09-18
 *
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_http_server.h"
#include <WiFi.h>
#include "cJSON.h"

void json_deserialize(cJSON *element)
{
  if (element->type == cJSON_Array) {
    int array_size = cJSON_GetArraySize(element);
    for (int i = 0; i < array_size; i++) {
      cJSON* aux = cJSON_GetArrayItem(element, i);
      json_deserialize(aux);
    }
  } else if (element->type == cJSON_Object) {
    cJSON *child_element = NULL;
    cJSON_ArrayForEach(child_element, element) {
      json_deserialize(child_element);
    }
  } else {
    json_deserialize_not_object(element);
  }
}
//
void json_deserialize_not_object(cJSON* element)
{
  if (element->type == cJSON_Invalid || element->type == cJSON_NULL) {

    Serial.print("[ NULL ]");
  } else if (element->type == cJSON_False) {
    Serial.print("[ BOOL: false ]");
  } else if (element->type == cJSON_True) {
    Serial.print("[ BOOL: true ]");
  } else if (element->type == cJSON_Number) {
    if (element->valueint == element->valuedouble) {
      Serial.print("[ INT: ");
      Serial.print(element->valueint);
      Serial.print("]");
    } else {
      Serial.print("[ FLOAT: ");
      Serial.print(element->valuedouble);
      Serial.print("]");
    }
  } else if (element->type == cJSON_String) {
    Serial.print("[ CHAR:");
    Serial.print(element->valuestring);
    Serial.print("]");
  }
}
//
esp_err_t root_post_handler(httpd_req_t *req)
{
  int total_len = req->content_len;
  int received;
  int remaining = total_len;
  char *buf = (char*)malloc(total_len + 1);
  while (remaining > 0) {
    received = httpd_req_recv(req, buf + (total_len - remaining), remaining);
    if (received <= 0) {
      if (received == HTTPD_SOCK_ERR_TIMEOUT) {
      }
      break;
    }
    remaining -= received;
  }

  buf[total_len] = '\0';
  // Json deserialization
  cJSON *root = cJSON_Parse(buf);
  cJSON *payload = cJSON_GetObjectItem(root, "payload");
  if (cJSON_IsObject(payload)) {
    int item_num = cJSON_GetArraySize(payload);
    Serial.println("Carga recebida com sucesso!");
    Serial.print("Números de elementos encontrados na carga: ");
    Serial.println(item_num);
    for (int i = 0; i < item_num; i++) {
      cJSON *element = cJSON_GetArrayItem(payload, i);
      json_deserialize(element);
    }
    Serial.println();
  } else {
      Serial.println("Carga util não encontrada");
  }
  Serial.println("Certifique que os tipos de dados conferem");
  Serial.println(" - Listas e objetos contam como \"elemento\"");
  Serial.println(" - Se floats não tiverem parte decimal, eles são interpretados como INT");
  Serial.println(" - Strings contam como CHAR");
  httpd_resp_send(req, "Carga Recebida", HTTPD_RESP_USE_STRLEN);
  cJSON_Delete(root);
  free(buf);
  return ESP_OK;
}
//
httpd_handle_t start_http_server(void)
{
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  httpd_handle_t server = NULL;
  if (httpd_start(&server, &config) == ESP_OK) {
    httpd_uri_t root_post_uri = {
      .uri = "/",
      .method = HTTP_POST,
      .handler = root_post_handler,
      .user_ctx = NULL
    };
    httpd_register_uri_handler(server, &root_post_uri);
    return server;
  }
  return NULL;
}

void webserver_task(void *pvParameters) {
  start_http_server();
  while(1) {
    delay(1000);
  }
}

const char* ssid = "obsat-test-server";      
const char* password = "obsatpass";

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.println("Inicializando Servidor Web");
  xTaskCreate(webserver_task, "webserver", configMINIMAL_STACK_SIZE * 5, NULL, 5, NULL);
}

void loop() {
}
