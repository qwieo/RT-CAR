#ifndef GARAGE_H
#define GARAGE_H
#include "image_process.h"
extern uint8_t pixels[114];

uint8_t otsuThreshold(uint8_t *image, int Sumpix);
bool check_zebra(void);

#endif