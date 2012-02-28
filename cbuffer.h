/* Circular buffer, taken and modified from an example on wikipedia
http://en.wikipedia.org/w/index.php?title=Circular_buffer&oldid=474915405
*/

#ifndef CIRCLE_BUFFER_LIB_H_GUARD
#define CIRCLE_BUFFER_LIB_H_GUARD

//#define TEST_CBUFF

#ifdef TEST_CBUFF

#include <malloc.h>
typedef unsigned char buff_t;

#else

#include "circle_api.h"
#include <malloc.h>
typedef u8 buff_t;

#endif

/* Circular buffer object */
typedef struct {
	int         size;   /* maximum number of elements           */
	int         start;  /* index of oldest element              */
	int         end;    /* index at which to write new element  */
	buff_t   *elems;  /* vector of elements                   */
} CircularBuffer;


void cbInit(CircularBuffer *cb, int size);
void cbFree(CircularBuffer *cb);
int cbIsFull(CircularBuffer *cb);
int cbIsEmpty(CircularBuffer *cb);
/* Write an element, overwriting oldest element if buffer is full. App can
choose to avoid the overwrite by checking cbIsFull(). */
void cbWrite(CircularBuffer *cb, buff_t elem);
/* Read oldest element. App must ensure !cbIsEmpty() first. */
void cbRead(CircularBuffer *cb, buff_t *elem);

#endif