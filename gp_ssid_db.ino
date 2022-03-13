#include "gp_ssid_db.h"

#define NUM_SSIDS 10

char network_names[NUM_SSIDS][MAX_SSID_LENGTH];
uint32_t last_join_attempt[NUM_SSIDS];

void gp_ssid_db_init() {
  memset(network_names, 0, sizeof(network_names));
  memset(last_join_attempt, 0, sizeof(last_join_attempt));
}

uint32_t get_last_join_attempt(const char * ssid) {
  for (int i = 0; i < NUM_SSIDS; i++) {
    if (strncmp(ssid, network_names[i], MAX_SSID_LENGTH) == 0) {
      return last_join_attempt[i];
    }
  }
  return 0;
}

void remember_join_attempt(const char * ssid, uint32_t attempt_time) {
  uint32_t oldest_time_seen = attempt_time + 1;
  uint8_t slot = 0;
  for (int i = 0; i < NUM_SSIDS; i++) {
    if (strncmp(ssid, network_names[i], MAX_SSID_LENGTH) == 0) {
      // We already have a record for this network
      last_join_attempt[i] = attempt_time;
      return;
    }
    if (last_join_attempt[i] < oldest_time_seen) {
      oldest_time_seen = last_join_attempt[i];
      slot = i;
    }
  }

  // Need to make a new record for this SSID. Use the oldest slot. If any are empty,
  // we've got a reference to the first one.

  last_join_attempt[slot] = attempt_time;
  strncpy(network_names[slot], ssid, MAX_SSID_LENGTH);
}
