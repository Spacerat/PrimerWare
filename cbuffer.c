/* Circular buffer, taken and modified from an example on wikipedia
http://en.wikipedia.org/w/index.php?title=Circular_buffer&oldid=474915405
*/

#include "cbuffer.h"

void cbInit(CircularBuffer *cb, int size) {
	cb->size  = size + 1; /* include empty elem */
	cb->start = 0;
	cb->end   = 0;
	cb->elems = (buff_t *)calloc(cb->size, sizeof(buff_t));
}

void cbFree(CircularBuffer *cb) {
	free(cb->elems); /* OK if null */ 
}

int cbIsFull(CircularBuffer *cb) {
	return (cb->end + 1) % cb->size == cb->start; 
}

int cbIsEmpty(CircularBuffer *cb) {
	return cb->end == cb->start; 
}

/* Write an element, overwriting oldest element if buffer is full. App can
choose to avoid the overwrite by checking cbIsFull(). */
void cbWrite(CircularBuffer *cb, buff_t *elem) {
	cb->elems[cb->end] = *elem;
	cb->end = (cb->end + 1) % cb->size;
	if (cb->end == cb->start)
		cb->start = (cb->start + 1) % cb->size; /* full, overwrite */
}

/* Read oldest element. App must ensure !cbIsEmpty() first. */
void cbRead(CircularBuffer *cb, buff_t *elem) {
	*elem = cb->elems[cb->start];
	cb->start = (cb->start + 1) % cb->size;
}