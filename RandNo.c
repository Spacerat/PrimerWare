/******************************************************************************
*
* File Name          :  RandNo.c
* Description        :  Generates random numbers.
*
*******************************************************************************/
#include "RandNo.h"

#define PHI 0x9e3779b9

static int Q[32], c = 65535;
 
__attribute__((section(".rodata"))) void init_rand(int x) {
	int i;
	Q[0] = x;
	Q[1] = x + PHI;
	Q[2] = x + PHI + PHI;
	for (i = 3; i < 32; i++)
		Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
}

__attribute__((section(".rodata"))) unsigned int rand_cmwc(void) {
	int t, a = 18782;
	static int i = 31;
	int x, r = 0xfffffffe;
	i = (i + 1) & 31;
	t = a * Q[i] + c;
	c = (t >> 16);
	x = t + c;
	if (x < c) {
		x++;
		c++;
	}
	return (Q[i] = r - x);
}

__attribute__((section(".rodata"))) bool rand_bool(void) {
	return rand_cmwc() % 2;
}