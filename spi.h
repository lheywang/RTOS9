/*
 * spi.h
 *
 *  Created on: 30 sept. 2025
 *      Author: l.heywang
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

void init_spi(void);
void IRQ_spi(unsigned index);

void SPI_putchar(uint8_t Data);
void SPI_putstring(uint8_t *Data);

#endif /* SPI_H_ */
