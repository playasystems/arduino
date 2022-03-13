#include "gp_packet.h"
#include "gp_neopixel.h"
#include "gp_wifi.h"
#include "gp_udp.h"

void setup() {
  gp_neopixel_init();
  gp_neopixel_set_half(GRAY);

  Serial.begin(9600);
  while(true) {
    if (Serial) break;
    if (millis() > 5000) break;
  }

  gp_neopixel_set_half(CYAN);

  Serial.println("Booted GigglePixel subscriber v1.0");
  wifi_init();
}

void handle_packet(gp_t *gp) {
  Serial.print("Got a packet of type ");
  Serial.println(gp->packet_type);

  if (gp->packet_type == PALETTE && gp->palette_size > 0) {
    for (int i = 0; i < NEOPIXEL_COUNT; i++) {
      uint8_t pal_index = i % gp->palette_size;
      gp_neopixel_set(i, gp->palette_r[pal_index], gp->palette_g[pal_index], gp->palette_b[pal_index]);
    }
    gp_neopixel_show();
  }
}

void loop() {
  wifi_loop();
  if (last_wifi_status != WL_CONNECTED) return;

  char packet[MAX_PACKET_LEN+1];
  int packet_len = udp_getpacket(packet);

  if (packet_len == 0) return;

  gp_t gp;
  bool valid_packet = packet_parse(packet, packet_len, &gp);
  if (!valid_packet) return;

  wifi_update_timeout();
  handle_packet(&gp);
}
