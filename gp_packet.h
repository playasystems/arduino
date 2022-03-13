#ifndef GP_PACKET_H
#define GP_PACKET_H

#define MAX_PACKET_LEN 512
#define MIN_PACKET_LEN 14

#define CRC_NONE 0
#define CRC_VALID 1
#define CRC_INVALID -1

#define MAX_PALETTE_SIZE 5

#define PALETTE 1
#define IDENTITY 3

typedef struct {
  uint8_t version;
  uint8_t priority;
  uint8_t flags;
  uint16_t source;
  uint16_t dest;
  uint8_t packet_type;

  uint8_t palette_r[MAX_PALETTE_SIZE];
  uint8_t palette_g[MAX_PALETTE_SIZE];
  uint8_t palette_b[MAX_PALETTE_SIZE];
  uint8_t palette_size;

  uint8_t crc_status;
} gp_t;


bool packet_parse(const char * packet, uint16_t packet_len, gp_t * gp);

#endif
