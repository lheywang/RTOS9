/*
 * ring_buffer.c
 *
 *  Created on: 30 sept. 2025
 *      Author: l.heywang
 */
#include "ring_buffer.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/cfg/global.h>

#include <driverlib.h>

#include "main.h"

static uint8_t buffer[RING_BUFFER_SIZE];
static uint8_t* head;
static uint8_t* tail;

uint8_t *__update_pointer(uint8_t* base, uint8_t* ptr)
{
    uint8_t* max = base + RING_BUFFER_SIZE;
    uint8_t* next = ptr + 1;

    if (next > max)
        return base;
    return next;
}

void init_ring_buffer(void)
{
    // empty the buffer
    memset((void*)buffer, 0x00, (size_t)RING_BUFFER_SIZE);

    // Place pointers to the start of the buffer
    head = &buffer[0];
    tail = &buffer[0];

    return;
}

void buffer_push(uint8_t data)
{
    *head = data;
    head = __update_pointer(buffer, head);
    return;
}

uint8_t buffer_pop(void)
{
    uint8_t tmp = *tail;
    tail = __update_pointer(buffer, tail);
    return tmp;
}

/*
 * Issue with this function : Return incorrect value when tail is near origin, and head is after.
 * buffer works at start, and then start to return invalid data.
 */
uint32_t buffer_get_size(void)
{
    // Case where head is behind tail
    if (head > tail)
    {
        uint32_t val = (uint32_t)(head - tail);
        return val;
    }

    // Case where tail is greater than head (because head passed the origin
    uint32_t val = (uint32_t)(head - buffer) + (uint32_t)(buffer - tail);
    return val;

}


