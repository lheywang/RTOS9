/******************************************************************************
 *  --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * --COPYRIGHT--
 *
 ******************************************************************************
 ******************************************************************************
 *  ======== hal_LCD.c ========
 *
 * Hardware abstraction layer for the FH-1138P Segmented LCD
 * on MSP-EXP430FR6989
 *
 * February 2015
 * E. Chen
 *
 ******************************************************************************/
#include <string.h>
/* XDCtools Header files */
#include <ti/sysbios/knl/Task.h>
/* BIOS Header files */
#include <driverlib.h>
/* Board Header file */
#include "main.h"
#include "hal_LCD.h"

/******************************************************************************
 *  ======== Variables globales ========
 ******************************************************************************/
char LCDBufferText[64];

//*****************************************************************************
// LCD memory map for numeric digits
//*****************************************************************************
const char digit[10][2] =
{
    {0xFC, 0x28},  /* "0" LCD segments a+b+c+d+e+f+k+q */
    {0x60, 0x20},  /* "1" */
    {0xDB, 0x00},  /* "2" */
    {0xF3, 0x00},  /* "3" */
    {0x67, 0x00},  /* "4" */
    {0xB7, 0x00},  /* "5" */
    {0xBF, 0x00},  /* "6" */
    {0xE4, 0x00},  /* "7" */
    {0xFF, 0x00},  /* "8" */
    {0xF7, 0x00}   /* "9" */
};


const char small_digit[10][2] =
{
    {0xCF, 0x00},       /* 0 */
    {0x06, 0x00},       /* 1 */
    {0xAD, 0x00},       /* 2 */
    {0x2F, 0x00},       /* 3 */
    {0x66, 0x00},       /* 4 */
    {0x6B, 0x00},       /* 5 */
    {0xEB, 0x00},       /* 6 */
    {0x0E, 0x00},       /* 7 */
    {0xEF, 0x00},       /* 8 */
    {0x6F, 0x00}        /* 9 */
};

// LCD memory map for uppercase letters
const char alphabetBig[26][2] =
{
    {0xEF, 0x00},  /* "A" LCD segments a+b+c+e+f+g+m */
    {0xF1, 0x50},  /* "B" */
    {0x9C, 0x00},  /* "C" */
    {0xF0, 0x50},  /* "D" */
    {0x9F, 0x00},  /* "E" */
    {0x8F, 0x00},  /* "F" */
    {0xBD, 0x00},  /* "G" */
    {0x6F, 0x00},  /* "H" */
    {0x90, 0x50},  /* "I" */
    {0x78, 0x00},  /* "J" */
    {0x0E, 0x22},  /* "K" */
    {0x1C, 0x00},  /* "L" */
    {0x6C, 0xA0},  /* "M" */
    {0x6C, 0x82},  /* "N" */
    {0xFC, 0x00},  /* "O" */
    {0xCF, 0x00},  /* "P" */
    {0xFC, 0x02},  /* "Q" */
    {0xCF, 0x02},  /* "R" */
    {0xB7, 0x00},  /* "S" */
    {0x80, 0x50},  /* "T" */
    {0x7C, 0x00},  /* "U" */
    {0x0C, 0x28},  /* "V" */
    {0x6C, 0x0A},  /* "W" */
    {0x00, 0xAA},  /* "X" */
    {0x00, 0xB0},  /* "Y" */
    {0x90, 0x28}   /* "Z" */
};

// LCD memory map for specials car
const char specialcar[11][2] =
{
    {0xEF, 0x00},  /* "TX-RX" LCD segments a+b+c+e+f+g+m */
    {0xF1, 0x50},  /* "Heart" */
    {0x9C, 0x00},  /* "exclamation" */
    {0xF0, 0x50},  /* "TIME" */
    {0x9F, 0x00},  /* "REC" */
    {0x8F, 0x00},  /* "BAT1" */
    {0xBD, 0x00},  /* "BAT2" */
    {0x6F, 0x00},  /* "BAT3" */
    {0x90, 0x50},  /* "BAT4" */
    {0x78, 0x00},  /* "BAT5" */
    {0x0E, 0x22}   /* "BAT6" */
};

uint8_t TabPosi[6]= {9,5,3,18,14,7};

/******************************************************************************
 * Init_LCD
 ******************************************************************************/
void Init_LCD(void)
{
    LCD_C_initParam initParams = {0};
    initParams.clockSource      = LCD_C_CLOCKSOURCE_ACLK;
    initParams.clockDivider     = LCD_C_CLOCKDIVIDER_1;
    initParams.clockPrescalar   = LCD_C_CLOCKPRESCALAR_16;
    initParams.muxRate          = LCD_C_4_MUX;
    initParams.waveforms        = LCD_C_LOW_POWER_WAVEFORMS;
    initParams.segments         = LCD_C_SEGMENTS_ENABLED;
    LCD_C_init(LCD_C_BASE, &initParams);

    // LCD Operation - VLCD generated internally, V2-V4 generated internally, v5 to ground
    LCD_C_setPinAsLCDFunctionEx(LCD_C_BASE, LCD_C_SEGMENT_LINE_0, LCD_C_SEGMENT_LINE_21);
    LCD_C_setPinAsLCDFunctionEx(LCD_C_BASE, LCD_C_SEGMENT_LINE_26, LCD_C_SEGMENT_LINE_39);

    LCD_C_setVLCDSource(LCD_C_BASE, LCD_C_VLCD_GENERATED_INTERNALLY,
                        LCD_C_V2V3V4_GENERATED_INTERNALLY_NOT_SWITCHED_TO_PINS,
                        LCD_C_V5_VSS);

    // Set VLCD voltage to 3.20v
    LCD_C_setVLCDVoltage(LCD_C_BASE, LCD_C_CHARGEPUMP_VOLTAGE_3_02V_OR_2_52VREF);

    // Enable charge pump and select internal reference for it
    LCD_C_enableChargePump(LCD_C_BASE);
    LCD_C_selectChargePumpReference(LCD_C_BASE, LCD_C_INTERNAL_REFERENCE_VOLTAGE);

    LCD_C_configChargePump(LCD_C_BASE, LCD_C_SYNCHRONIZATION_ENABLED, 0);

    // Clear LCD memory
    LCD_C_clearMemory(LCD_C_BASE);

    //Turn LCD on
    LCD_C_on(LCD_C_BASE);
}

/******************************************************************************
 * ======== displayScrollText ========
 * Scrolls input string across LCD screen from  right to left
 ******************************************************************************/
void DisplayScrollText(char *msg, uint32_t Speed)
{
    int length = strlen(msg);
    int i;
    int s = 5;
    char buffer[6] = "      ";

    for (i=0; i<length; i++)
        {
        int t;
        for (t=0; t<6; t++)
            buffer[t] = ' ';

        int j;
        for (j=0; j<length; j++)
            {
            if (((s+j) >= 0) && ((s+j) < 6))
                buffer[s+j] = msg[j];
            }

        s--;

        ShowChar(buffer[0], pos1);
        ShowChar(buffer[1], pos2);
        ShowChar(buffer[2], pos3);
        ShowChar(buffer[3], pos4);
        ShowChar(buffer[4], pos5);
        ShowChar(buffer[5], pos6);

        Task_sleep(Speed);
        }
}

/******************************************************************************
 * ======== displayText ========
 * input string across LCD screen from  right to left
 ******************************************************************************/
void DisplayText(char *msg)
{
    int length, i;

    length = strlen(msg);
    if(length > 7)
        DisplayScrollText(msg, 100);
    else
        {
        for(i = 6;i>=0;--i)
            {
            if (msg[i] >= '0' && msg[i] <= '9')
                {
                // Display digit
                LCDMEM[TabPosi[i]] = digit[msg[i]-48][0];
                LCDMEM[TabPosi[i]+1] = digit[msg[i]-48][1];
                }
            else if (msg[i] >= 'A' && msg[i] <= 'Z')
                {
                // Display alphabet
                LCDMEM[TabPosi[i]] = alphabetBig[msg[i]-65][0];
                LCDMEM[TabPosi[i]+1] = alphabetBig[msg[i]-65][1];
                }
            }
        }
}

/******************************************************************************
 * ======== ShowChar ========
 * Displays input character at given LCD digit/position
 * Only spaces, numeric digits, and uppercase letters are accepted characters
 ******************************************************************************/
void ShowChar(char c, int position )
{
    if (c == ' ')
        {
        // Display space
        LCDMEM[position] = 0;
        LCDMEM[position+1] = 0;
        }

    else if (c == '-')
        {
        // Display space
        LCDMEM[position] = 0;
        LCDMEM[position+1] = 0;
        }

    else if (c >= '0' && c <= '9')
        {
        // Display digit
        LCDMEM[position] = digit[c-48][0];
        LCDMEM[position+1] = digit[c-48][1];
        }

    else if (c >= 'A' && c <= 'Z')
        {
        // Display alphabet
        LCDMEM[position] = alphabetBig[c-65][0];
        LCDMEM[position+1] = alphabetBig[c-65][1];
        }

    else
        {
        // Turn all segments on if character is not a space, digit, or uppercase letter
        LCDMEM[position] = 0xFF;
        LCDMEM[position+1] = 0xFF;
        }
}

/******************************************************************************
 * ======== clearLCD ========
 * Clears memories to all 6 digits on the LCD
 ******************************************************************************/
void ClearLCD(void)
{
    LCDMEM[pos1]    = LCDBMEM[pos1]     = 0;
    LCDMEM[pos1+1]  = LCDBMEM[pos1+1]   = 0;
    LCDMEM[pos2]    = LCDBMEM[pos2]     = 0;
    LCDMEM[pos2+1]  = LCDBMEM[pos2+1]   = 0;
    LCDMEM[pos3]    = LCDBMEM[pos3]     = 0;
    LCDMEM[pos3+1]  = LCDBMEM[pos3+1]   = 0;
    LCDMEM[pos4]    = LCDBMEM[pos4]     = 0;
    LCDMEM[pos4+1]  = LCDBMEM[pos4+1]   = 0;
    LCDMEM[pos5]    = LCDBMEM[pos5]     = 0;
    LCDMEM[pos5+1]  = LCDBMEM[pos5+1]   = 0;
    LCDMEM[pos6]    = LCDBMEM[pos6]     = 0;
    LCDMEM[pos6+1]  = LCDBMEM[pos6+1]   = 0;
    LCDM14          = LCDBM14           = 0x00;
    LCDM18          = LCDBM18           = 0x00;
    LCDM3           = LCDBM3            = 0x00;
}

/******************************************************************************
 * ======== clearLCD ========
 * Clears memories to 1 digits on the LCD
 ******************************************************************************/
void ClearDIGIT(void)
{
    LCDMEM[pos1]    = LCDBMEM[pos1]     = 0;
    LCDMEM[pos1+1]  = LCDBMEM[pos1+1]   = 0;
    LCDMEM[pos2]    = LCDBMEM[pos2]     = 0;
    LCDMEM[pos2+1]  = LCDBMEM[pos2+1]   = 0;
    LCDMEM[pos3]    = LCDBMEM[pos3]     = 0;
    LCDMEM[pos3+1]  = LCDBMEM[pos3+1]   = 0;
    LCDMEM[pos4]    = LCDBMEM[pos4]     = 0;
    LCDMEM[pos4+1]  = LCDBMEM[pos4+1]   = 0;
    LCDMEM[pos5]    = LCDBMEM[pos5]     = 0;
    LCDMEM[pos5+1]  = LCDBMEM[pos5+1]   = 0;
    LCDMEM[pos6]    = LCDBMEM[pos6]     = 0;
    LCDMEM[pos6+1]  = LCDBMEM[pos6+1]   = 0;
}

//*****************************************************************************
void NegCar(void)
{
    LCDMEM[pos1+1] |= 0x04;
}

//*****************************************************************************
void NegCar_Clr(void)
{
    LCDMEM[pos1+1] &= ~0x04;
}

//*****************************************************************************
void Decimal_Point(int position)
{
    LCDMEM[TabPosi[position]+1] |= 0x01;
}

//*****************************************************************************
void Degre_Point(void)
{
    LCDMEM[pos4+1] |= 0x04;
}

//*****************************************************************************
void Degre_Point_Clr(void)
{
    LCDMEM[pos4+1] &= ~0x04;
}

//*****************************************************************************
void RXD_Point(void)
{
    LCDMEM[pos6+1] |= 0x01;
}

//*****************************************************************************
void RXD_Point_Clr(void)
{
    LCDMEM[pos6+1] &= ~0x01;
}

//*****************************************************************************
void TXD_Point(void)
{
    LCDMEM[pos6+1] |= 0x04;
}

//*****************************************************************************
void TXD_Point_Clr(void)
{
    LCDMEM[pos6+1] &= ~0x04;
}

//*****************************************************************************
void Ant_Point(void)
{
    LCDMEM[pos3+1] |= 0x04;
}

//*****************************************************************************
void Ant_Clr(void)
{
    LCDMEM[pos3+1] = 0;
}

//*****************************************************************************
void Exclamation_Point(void)
{
    LCDMEM[pos7] |= 0x01;
}

//*****************************************************************************
void Exclamation_Point_Clr(void)
{
    LCDMEM[pos7] &= ~0x01;
}

//*****************************************************************************
void Rec_Point(void)
{
    LCDMEM[pos7] |= 0x02;
}

//*****************************************************************************
void Rec_Point_Clr(void)
{
    LCDMEM[pos7] &= ~0x02;
}

//*****************************************************************************
void Heart_Point(void)
{
    // Degree symbol
    LCDMEM[pos7] |= 0x04;
}

//*****************************************************************************
void Heart_Clr(void)
{
    LCDMEM[pos7] &= ~0x04;
}

//*****************************************************************************
void Timer_Point(void)
{
    LCDMEM[pos7] |= 0x08;
}

//*****************************************************************************
void Timer_Point_Clr(void)
{
    LCDMEM[pos7] &= ~0x08;
}

//****************************************************************************
void Bat_Point(void)
{
    LCDMEM[pos8] |= BAT2+BAT4+BAT6+BATL;
    LCDMEM[pos9] |= BAT1+BAT3+BAT5+BATR;
}

//*****************************************************************************
void Clock_ON(void)
{
    LCDM3  |= 0x08;
    LCDBM3 |= 0x08;
}

//*****************************************************************************
void Clock_OFF(void)
{
    LCDM3  &= ~0x08;
    LCDBM3 &= ~0x08;
}

/******************************************************************************
 * ======== LCDBufferText ========
 * Clears memories to 1 digits on the LCD
 ******************************************************************************/
char *LCDstrupr(char *string)
{
    int i = 0;
    char len = strlen(string);
    char *Ptr = &LCDBufferText[0];

    while(len != 0)
        {
        if(string[i] >= 'a' && string[i] <= 'z')
            {
            LCDBufferText[i] = string[i] - 32;
            }

        else LCDBufferText[i] = string[i];
        i++;
        --len;
        }

    LCDBufferText[i] = '\0';
    return Ptr;
}

//******************************************************************************








