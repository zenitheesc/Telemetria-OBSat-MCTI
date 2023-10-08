/*
 * Autor: Zoh-j02r - Dept. Software Embarcado
 * Data: 2023-09-18
 *
 */

// TODO: removal of Serial output, all messages are handled by HTTP

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_http_server.h"
#include <WiFi.h>
#include "cJSON.h"

size_t bytes_size = 0;
size_t response_offset = 0;
const char* strs[] = {"equipe","bateria","temperatura","pressao","giroscopio","acelerometro","payload"};

char response[1000];
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

void json_deserialize_not_object(cJSON* element)
{
  if (element->type == cJSON_Invalid || element->type == cJSON_NULL) {
    Serial.print("[NULL]");
    response_offset += snprintf(response + response_offset, sizeof(response) - response_offset,"[NULL]");
    bytes_size += 1; 
  } else if (element->type == cJSON_False) {
    Serial.print("[BOOL: false]");
    response_offset += snprintf(response + response_offset, sizeof(response) - response_offset,"[BOOL: false]");
    bytes_size += 1; 
  } else if (element->type == cJSON_True) {
    Serial.print("[BOOL: true]");
    response_offset += snprintf(response + response_offset, sizeof(response) - response_offset,"[BOOL: true]");
    bytes_size += 1; 
  } else if (element->type == cJSON_Number) {
    if (element->valueint == element->valuedouble) {
      Serial.print("[INT: ");
      Serial.print(element->valueint);
      Serial.print("]");
      response_offset += snprintf(response + response_offset, sizeof(response) - response_offset,"[INT: %d]",element->valueint);
      bytes_size += 4; 
    } else {
      Serial.print("[FLOAT: ");
      Serial.print(element->valuedouble);
      Serial.print("]");
      response_offset += snprintf(response + response_offset, sizeof(response) - response_offset,"[FLOAT: %.3lf]", element->valuedouble);
      bytes_size += 8; 
    }
  } else if (element->type == cJSON_String) {
    Serial.print("[CHAR: ");
    Serial.print(element->valuestring);
    Serial.print("]");
    response_offset += snprintf(response + response_offset, sizeof(response) - response_offset,"[STRING: %s]", element->valuestring);
    bytes_size += strlen(element->valuestring);
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
  cJSON *root = cJSON_Parse(buf);
  for (int i = 0; i < 7; i++) {
    cJSON *val = cJSON_GetObjectItem(root, strs[i]);
    int item_num = cJSON_GetArraySize(val);
    if (val != NULL){
    } else {
      // 🤯🤯🤯🤯🤯🤯🤯🤯🤯🤯🤯🤯
      Serial.print(strs[i]);
      Serial.println(" - aparenta estar incorreto!");
      Serial.println("O JSON recebido não segue a formatação correta");
      httpd_resp_send(req, "O JSON recebido não segue a formatação correta", HTTPD_RESP_USE_STRLEN);
      cJSON_Delete(root);
      free(buf);
      return ESP_OK;
    }
  }

  int equipe = 0;
  cJSON *nM = cJSON_GetObjectItem(root, "equipe");
  if (nM != NULL && nM->type == cJSON_Number) {
      equipe = nM->valueint;
  }

  Serial.print("Recebendo dados da equipe: ");
  Serial.println(equipe);
  bytes_size = 0;
  cJSON *payload = cJSON_GetObjectItem(root, "payload");
  int item_num = cJSON_GetArraySize(payload);
  Serial.println("JSON recebido com sucesso!");
  Serial.print("Números de elementos encontrados na carga: ");
  Serial.println(item_num);
  for (int i = 0; i < item_num; i++) {
    cJSON *element = cJSON_GetArrayItem(payload, i);
    json_deserialize(element);
    if (bytes_size > 91) {
      Serial.println();
      Serial.print("Quantidade de bytes excece o maximo permitido (");
      Serial.print(bytes_size);
      Serial.println("/90 bytes)");
      char except[61];
      snprintf(except, 61 ,"Quantidade de bytes excede o maximo permitido (%d/90 bytes)", bytes_size);
      httpd_resp_send(req, except, HTTPD_RESP_USE_STRLEN);
      cJSON_Delete(root);
      free(buf);
      return ESP_OK;
    }
  }
  snprintf(response + response_offset, sizeof(response) - response_offset,"(%d/90)", bytes_size);
  Serial.println();
  Serial.println("Transferência bem sucedida");
  httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
  memset(response,0,sizeof(response)); 
  response_offset = 0;
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
  memset(response,0,sizeof(response)); 
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.println("Inicializando Servidor Web");
  xTaskCreate(webserver_task, "webserver", configMINIMAL_STACK_SIZE * 5, NULL, 5, NULL);
}

void loop() {
}
