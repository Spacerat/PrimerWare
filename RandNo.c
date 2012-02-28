/******************************************************************************
*
* File Name          :  RandNo.c
* Description        :  Generates random numbers.
*
*******************************************************************************/
#define PHI 0x9e3779b9 //(Joe: lol.)

static int Q[32], c = 65535;
 
void init_rand(int x) {
	int i;
	Q[0] = x;
	Q[1] = x + PHI;
	Q[2] = x + PHI + PHI;
	for (i = 3; i < 32; i++)
		Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
}

unsigned int rand_cmwc(void) {
	int t, a = 18782;
	static int i = 4095;
	int x, r = 0xfffffffe;
	i = (i + 1) & 4095;
	t = a * Q[i] + c;
	c = (t >> 16);
	x = t + c;
	if (x < c) {
		x++;
		c++;
	}
	return (Q[i] = r - x);
}