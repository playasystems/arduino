#include "gp_wifi.h"
#include "gp_udp.h"
#include "gp_ssid_db.h"

#define WIFI_CS_PIN 8
#define WIFI_IRQ_PIN 7
#define WIFI_RESET_PIN 4
#define WIFI_ENABLE_PIN 2

#define CONNECT_TIMEOUT (10 * 1000)
#define IDLE_TIMEOUT (60 * 1000)

uint8_t last_wifi_status;
uint32_t next_connect_attempt;

void wifi_init() {
  WiFi.setPins(WIFI_CS_PIN, WIFI_IRQ_PIN, WIFI_RESET_PIN, WIFI_ENABLE_PIN);
  gp_ssid_db_init();
  next_connect_attempt = 0;
  last_wifi_status = WL_NO_SHIELD;
}

bool is_gp(int network) {
  if (WiFi.encryptionType(network) != ENC_TYPE_NONE) return false;
  const char * ssid = WiFi.SSID(network);
  return strstr(ssid, "*GP*") != NULL;
}


// Find least-recently-joined GP network, using signal strength as tiebreaker
const char * next_network(uint32_t now) {
  int8_t ssids_found = WiFi.scanNetworks();

  uint32_t best_lja = 999999;

  for (int i = 0; i < ssids_found; i++) {
    if (!is_gp(i)) continue;
    uint32_t lja = get_last_join_attempt(WiFi.SSID(i));
    if (lja < best_lja) best_lja = lja;
  }

  int32_t best_strength = -9999;
  int8_t strongest = -1;
  for (int i = 0; i < ssids_found; i++) {
    if (!is_gp(i)) continue;
    uint32_t lja = get_last_join_attempt(WiFi.SSID(i));
    if (lja > best_lja) continue;
    int32_t strength = WiFi.RSSI(i);
    if (strength > best_strength) {
      best_strength = strength;
      strongest = i;
    }
  }
  if (strongest < 0) return NULL;
  const char * ssid = WiFi.SSID(strongest);
  if (ssid[0] == 0) {
    Serial.print("Got blank SSID for position ");
    Serial.println(strongest);
    return NULL;
  }
  remember_join_attempt(ssid, now);
  return ssid;
}

void join_new_network(uint32_t now) {
  const char * ssid;

  WiFi.setTimeout(CONNECT_TIMEOUT);
  next_connect_attempt = now + CONNECT_TIMEOUT;
  ssid = next_network(now);

  if (ssid == NULL) {
    Serial.println("No GP networks found");
    gp_neopixel_set_half(GRAY);
    return;
  }

  Serial.print("Attempting to connect to ");
  Serial.println(ssid);
  gp_neopixel_set_half(YELLOW);
  uint8_t rv = WiFi.begin(ssid);  // Could block for up to CONNECT_TIMEOUT seconds
}

void wifi_loop() {
  uint32_t now = millis();
  uint8_t wifi_status = WiFi.status();

  if (now >= next_connect_attempt) {
    last_wifi_status = 254;
    wifi_status = WL_IDLE_STATUS;
  }

  if (wifi_status != last_wifi_status) {
    last_wifi_status = wifi_status;
    switch (wifi_status) {
      case WL_NO_SHIELD:
        Serial.println("No WiFi shield detected");
        gp_neopixel_set_half(RED);
        while(true);
      case WL_CONNECT_FAILED:
      case WL_CONNECTION_LOST:
        gp_neopixel_set_half(BLUE);
        // Fallthrough
      case WL_IDLE_STATUS:
        join_new_network(now);
        break;
      case WL_CONNECTED:
        gp_neopixel_set_half(GREEN);
        wifi_update_timeout();
        printWiFiStatus();
        udp_init();
        break;
      case WL_DISCONNECTED:
        // We maybe just started trying to join a network
        break;
      case WL_NO_SSID_AVAIL:
      case WL_SCAN_COMPLETED:
      case WL_AP_LISTENING:
      case WL_AP_CONNECTED:
      case WL_AP_FAILED:
      case WL_PROVISIONING:
      case WL_PROVISIONING_FAILED:
      default:
        Serial.print("Unknown WiFi status ");
        Serial.println(wifi_status);
        gp_neopixel_set_half(PURPLE);
        while(true);
    }
  }
}

void wifi_update_timeout() {
  next_connect_attempt = millis() + IDLE_TIMEOUT;
}


void printWiFiStatus() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
