/*
 * ring_buffer.h
 *
 *  Created on: 30 sept. 2025
 *      Author: l.heywang
 */

#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define RING_BUFFER_SIZE 128

void init_ring_buffer(void);

void buffer_push(uint8_t data);
uint8_t buffer_pop(void);
uint32_t buffer_get_size(void);


#endif /* RING_BUFFER_H_ */
