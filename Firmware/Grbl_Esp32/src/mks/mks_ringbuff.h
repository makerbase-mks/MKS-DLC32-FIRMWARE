#ifndef __mks_ringbuff_h
#define __mks_ringbuff_h

#include "MKS_draw_lvgl.h"


#define CMD_NUM         4
#define CMD_LEN         64


typedef struct {
	unsigned int head;
	unsigned int tail;
	unsigned int length;
	char buff[CMD_NUM][CMD_LEN];
}ringbuff;
extern ringbuff rb_sd;

void rb_init(ringbuff *rb);
bool rb_write(ringbuff* rb, char *data);
bool rb_read(ringbuff* rb, char *data);
bool is_rb_empty(ringbuff* rb);
bool is_rb_full(ringbuff* rb);
#endif
