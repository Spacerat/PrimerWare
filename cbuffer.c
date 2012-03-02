/* Circular buffer, taken and modified from an example on wikipedia
http://en.wikipedia.org/w/index.php?title=Circular_buffer&oldid=474915405
*/

#include "cbuffer.h"



__attribute__((section(".rodata"))) void cbInit(CircularBuffer *cb, int size) {
	cb->size  = size + 1; /* include empty elem */
	cb->start = 0;
	cb->end   = 0;
	cb->elems = (buff_t *)calloc(cb->size, sizeof(buff_t));
}

__attribute__((section(".rodata"))) void cbFree(CircularBuffer *cb) {
	free(cb->elems); /* OK if null */ 
}

__attribute__((section(".rodata"))) int cbIsFull(CircularBuffer *cb) {
	return (cb->end + 1) % cb->size == cb->start; 
}

__attribute__((section(".rodata"))) int cbIsEmpty(CircularBuffer *cb) {
	return cb->end == cb->start; 
}

/* Write an element, overwriting oldest element if buffer is full. App can
choose to avoid the overwrite by checking cbIsFull(). */
__attribute__((section(".rodata"))) void cbWrite(CircularBuffer *cb, buff_t elem) {
	cb->elems[cb->end] = elem;
	cb->end = (cb->end + 1) % cb->size;
	if (cb->end == cb->start)
		cb->start = (cb->start + 1) % cb->size; /* full, overwrite */
}

/* Read oldest element. App must ensure !cbIsEmpty() first. */
__attribute__((section(".rodata"))) void cbRead(CircularBuffer *cb, buff_t *elem) {
	*elem = cb->elems[cb->start];
	cb->start = (cb->start + 1) % cb->size;
}


#ifdef TEST_CBUFF

#include <stdio.h>

__attribute__((section(".rodata"))) int main( void ) {
	CircularBuffer buff;
	cbInit(&buff, 10);
	cbWrite(&buff, 3);
	cbWrite(&buff, 23);
	cbWrite(&buff, 43);	
	buff_t dat;
	cbRead(&buff, &dat);
	printf("%d\n", dat);

	cbRead(&buff, &dat);
	printf("%d\n", dat);

	cbRead(&buff, &dat);
	printf("%d\n", dat);

	printf("%d\n", cbIsEmpty(&buff));
	return 0;
}

#endif