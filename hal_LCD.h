/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * --/COPYRIGHT--*/
/*******************************************************************************
 *
 * hal_LCD.h
 *
 * Hardware abstraction layer for the FH-1138P Segmented LCD
 * on MSP-EXP430FR6989
 *
 * February 2015
 * E. Chen
 *
 ******************************************************************************/

#ifndef OUTOFBOX_MSP430FR6989_HAL_LCD_H_
#define OUTOFBOX_MSP430FR6989_HAL_LCD_H_

//Change based on LCD Memory locations
#define pos1 9   /* Digit A1 begins at S18/2=9 */
#define pos2 5   /* Digit A2 begins at S10/2=5 */
#define pos3 3   /* Digit A3 begins at S6/2=3  */
#define pos4 18  /* Digit A4 begins at S36/2=18 */
#define pos5 14  /* Digit A5 begins at S28/2=14 */
#define pos6 7   /* Digit A6 begins at S14/2=7 */
#define pos7 2   /* Digit A7 begins at S4/2=2 */
#define pos8 13  /* Digit BATT begins at S26/2=13 */
#define pos9 17  /* Digit BATT begins at S34/2=13 */

#define BATL    0x10    // indication affichage de la batterie
#define BATR    0x10
#define BAT1    0x20
#define BAT2    0x20
#define BAT3    0X40
#define BAT4    0X40
#define BAT5    0X80
#define BAT6    0X80


// Define word access definitions to LCD memories
#ifndef LCDMEMW
#define LCDMEMW    ((int*) LCDMEM) /* LCD Memory (for C) */
#endif

//*****************************************************************************

char BufferText[30];    // pour strupr()

//*****************************************************************************

extern const char digit[10][2];
extern const char alphabetBig[26][2];
extern const char small_digit[10][2];
extern const char specialcar[11][2];

void Init_LCD(void);
void DisplayScrollText(char *msg, uint32_t Speed);
void DisplayText(char *msg);
void ShowChar(char, int);
void ClearLCD(void);
void ClearDIGIT(void);
void NegCar(void);
void NegCar_Clr(void);
void Decimal_Point(int position);
void Degre_Point(void);
void Degre_Point_Clr(void);
void RXD_Point(void);
void RXD_Point_Clr(void);
void TXD_Point(void);
void TXD_Point_Clr(void);
void Ant_Point(void);
void Ant_Clr(void);
void Heart_Point(void);
void Heart_Clr(void);
void Exclamation_Point(void);
void Exclamation_Point_Clr(void);
void Timer_Point(void);
void Timer_Point_Clr(void);
void Rec_Point(void);
void Rec_Point_Clr(void);
void Bat_Point(void);
void Clock_ON(void);
void Clock_OFF(void);
char *LCDstrupr(char *string);

#endif /* OUTOFBOX_MSP430FR6989_HAL_LCD_H_ */
