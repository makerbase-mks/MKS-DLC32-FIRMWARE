#include "mks_ringbuff.h"


ringbuff rb_sd;

void rb_init(ringbuff *rb) {
	rb->head = 0;
	rb->tail = 0;
	rb->length = 0;
}

bool rb_write(ringbuff* rb, char *data) {

	if (rb->length >= CMD_NUM) {
		return false;
	}

	strcpy(rb->buff[rb->tail], data);
	rb->tail = (rb->tail + 1) % CMD_NUM;
	rb->length++;
    return true;
}

bool rb_read(ringbuff* rb, char *data) {

	if (rb->length == 0) {
		return false;
	}
	strcpy(data, rb->buff[rb->head]);
	rb->head = (rb->head + 1) % CMD_NUM;
	rb->length--;
    return true;
}

bool is_rb_empty(ringbuff* rb) {
    if(rb->length == 0) return true;
    else return false;
}

bool is_rb_full(ringbuff* rb) {
    if(rb->length == CMD_NUM) return true;
    else return false;
}