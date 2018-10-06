#include "quantum.h"

/* animation modes */

#define ANIMATION_MODE_STATIC   10
#define ANIMATION_MODE_BREATHE  11
#define ANIMATION_MODE_SPECTRUM 12
#define ANIMATION_MODE_RAINBOW  13

#define BREATHE_STEPS 5

void scan_animation(void);

void animation_mode(uint8_t newMode);
void animation_step_static(void);
void animation_step_breathe(void);
void animation_step_spectrum(void);
void animation_step_rainbow(void);

void speed_increase(void);
void speed_decrease(void);
