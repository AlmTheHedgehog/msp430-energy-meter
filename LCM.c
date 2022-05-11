/* LCM.c: MSP430.
 */

#include "LCM.h"

void pulse(void) {    
    LCM_OUT2 |= EN;
    __delay_cycles(200);
    
    LCM_OUT2 &= ~EN;
    __delay_cycles(200);
} // pulse


void SendByte(char ByteToSend, char IsData) {
    LCM_OUT1 &= ~MASK1;
    LCM_OUT2 &= ~MASK2;
    LCM_OUT1 |= (ByteToSend & 0xF0);
    
    if (IsData == TRUE) LCM_OUT2 |= RS;
    else LCM_OUT2 &= ~RS;
    pulse();
    
    LCM_OUT1 &= ~MASK1;
    LCM_OUT2 &= ~MASK2;
    LCM_OUT1 |= ((ByteToSend & 0x0F) << 4);
    
    if (IsData == TRUE) LCM_OUT2 |= RS;
    else LCM_OUT2 &= ~RS;
    pulse();
} // SendByte


void LCM_init(void) {
    LCM_DIR1 |= MASK1;
    LCM_DIR2 |= MASK2;
    LCM_OUT1 &= ~MASK1;
    LCM_OUT2 &= ~MASK2;
    
   __delay_cycles(100000);
   
   LCM_OUT1 = 0x20;
   pulse();
   
   SendByte(0x28, FALSE);
   SendByte(0x0C, FALSE);
   SendByte(0x06, FALSE);
} // LCM_init


void clear(void) {
    SendByte(0x01, FALSE);
    SendByte(0x02, FALSE);
    __delay_cycles(100000);
} // clear


void MoveCursor(char Row, char Col) {
    char address;
    if (Row == 0) address = 0;
    else address = 0x40;
    address |= Col;
    SendByte(0x80|address, FALSE);
} // MoveCursor

void PrintStr(char *Text) {
    char *c;
    c = Text;
    while ((c != 0) && (*c != 0)) {
        SendByte(*c, TRUE);
        c++;
    }
} // PrintStr

