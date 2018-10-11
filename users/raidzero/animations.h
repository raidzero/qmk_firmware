#include "quantum.h"

/* animation modes */

#define ANIMATION_MODE_STATIC   (1 << 0)
#define ANIMATION_MODE_BREATHE  (1 << 1)
#define ANIMATION_MODE_SPECTRUM (1 << 2)
#define ANIMATION_MODE_RAINBOW  (1 << 3)
#define ANIMATION_MODE_SWIRL    (1 << 4)

#define BREATHE_STEPS 5

/* EEPROM location of animation mode */
#define ANIM_EEPROM_BYTE (uint8_t*) 21

void init_animation(void);
void scan_animation(void);

void animation_mode(uint8_t newMode);
void animation_step_static(void);
void animation_step_breathe(void);
void animation_step_spectrum(void);
void animation_step_rainbow(void);
void animation_step_swirl(void);

void speed_increase(void);
void speed_decrease(void);
