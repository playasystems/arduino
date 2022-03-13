#ifndef GP_SSID_DB_H
#define GP_SSID_DB_H

#define MAX_SSID_LENGTH 30

typedef struct {
  const char ssid[MAX_SSID_LENGTH+1];
  uint32_t last_connect_attempt;
} ssid_db_t;

void gp_ssid_db_init();
uint32_t get_last_join_attempt(const char * ssid);
void remember_join_attempt(const char * ssid, uint32_t attempt_time);

#endif
