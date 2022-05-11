#ifndef LCM_H_
#define LCM_H_

#endif /*LCM_H_*/

#include <msp430.h>



#define LCM_DIR1 P1DIR
#define LCM_OUT1 P1OUT
#define LCM_DIR2 P2DIR
#define LCM_OUT2 P2OUT

#define RS  BIT1
#define EN  BIT0
#define D4  BIT4
#define D5  BIT5
#define D6  BIT6
#define D7  BIT7


#define MASK1    (D4 + D5 + D6 + D7)
#define MASK2    (RS + EN)
#define FALSE 0
#define TRUE 1


void pulse(void);
void SendByte(char, char);
void LCM_init(void);
void clear(void);
void MoveCursor(char, char);
void PrintStr(char *);
