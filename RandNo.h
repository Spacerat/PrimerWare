#ifndef RANDNO_H_GUARD
#define RANDNO_H_GUARD

#include "circle_api.h"

void init_rand(int x);
unsigned int rand_cmwc(void);
bool rand_bool(void);

#endif