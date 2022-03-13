#include <WiFiUdp.h>
#include "gp_packet.h"

WiFiUDP Udp;

void udp_init() {
  Udp.begin(7016);
}

int udp_getpacket(char * packet) {
  int packet_len = Udp.parsePacket();
  if (!packet_len) return 0;
  return Udp.read(packet, MAX_PACKET_LEN);
}
