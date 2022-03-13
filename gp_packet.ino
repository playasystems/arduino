#include "gp_packet.h"

const char * MAGIC = "GLPX";

uint16_t unpack16(const char * bytes) {
  return *bytes | *(bytes+1);
}

bool packet_parse(const char * packet, uint16_t packet_len, gp_t * gp) {
  if (packet_len < MIN_PACKET_LEN) return false;
  if (memcmp(packet, MAGIC, 4) != 0) return false;

  memset(gp, 0, sizeof(gp_t));

  uint16_t length = unpack16(packet + 5);
  gp->version = packet[4];
  gp->priority = packet[8];
  gp->flags = packet[9];
  gp->source = unpack16(packet + 10);
  gp->dest = unpack16(packet + 12);
  gp->packet_type = packet[7];

  if (packet_len == length) {
    gp->crc_status = CRC_NONE;
  } else {
    gp->crc_status = CRC_INVALID; // FIXME: Verify CRCs and start throwing away invalid packets
  }

  switch (gp->packet_type) {
    case PALETTE:
      parse_palette(gp, packet + 14, packet_len - 14);
      break;
    default:
      break;
  }

  return true;
}

void parse_palette(gp_t * gp, const char * payload, uint16_t payload_size) {
  if (payload_size < 2) return;

  uint16_t size = unpack16(payload);
  if (payload_size < 2 + size * 4) return;

  if (size > MAX_PALETTE_SIZE) size = MAX_PALETTE_SIZE;
  gp->palette_size = size;

  for (int i = 0; i < size; i++) {
    // Ignore frac
    gp->palette_r[i] = payload[2 + 4 * i + 1];
    gp->palette_g[i] = payload[2 + 4 * i + 2];
    gp->palette_b[i] = payload[2 + 4 * i + 3];
  }
}
