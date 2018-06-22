#include "quantum.h"

/* reactive rgb mode bit field */
#define RGB_REACTIVE_ENABLED  (1 << 0)
#define RGB_WAS_ENABLED       (1 << 1)
#define LIGHT_ALL_LEDS        (1 << 2)
#define RGB_FADE_OUT          (1 << 3)

/* RGB flag bit field helpers */
void flip_rgb_bit_on(uint8_t bit);
void flip_rgb_bit_off(uint8_t bit);
void flip_rgb_bit(uint8_t bit);

/* refers to an LED in memory */
typedef struct {
  int h;
  uint8_t s;
  uint8_t v;
  bool on;
} rgbled;

/* RGB functions */
void light_all_leds(int hue);
void light_led_in_color(uint8_t index, int hue);

void turn_off_all_leds(void);
void fadeLeds(void);
void set_leds(void);

/* called from qmk functions */
void process_leds(keyrecord_t* record);
void scan_leds(void);
void process_reactive_toggle(void);
void process_reactive_fade(void);
void process_reactive_all(void);
void process_rgb_toggle(void);
