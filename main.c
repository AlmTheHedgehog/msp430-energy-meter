#include <msp430g2553.h>
#include <LCM.h>

#define A0  BIT0
#define dv  0.0032258

float volts=0.0;
unsigned long long int energy=0;//in piko J
unsigned int OFCount;

void ADC_init(void);
void update_display(unsigned short int mode);
void initTimer_A(void);
void delayMS(int msecs);

void main(void) {

    WDTCTL = WDTPW + WDTHOLD;

    LCM_init();
    ADC_init();

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    initTimer_A();
    _enable_interrupt();

    update_display(2);

    for (;;) {
        if(P1IN & 0x02){
            update_display(1);
            energy=0;
            while(P1IN & 0x02){
                delayMS(10);
                ADC10CTL0 |= ADC10SC;  
                while ((ADC10CTL1 & ADC10BUSY) == 0x01); 
                volts=ADC10MEM*dv;
                energy+=(((volts*volts)/10000)*0.01)*10000000000000;//10000000000000 - convert to 10 piko j
            }
            update_display(0);
        }
    }

} // main

void ADC_init(void) {
            //  Vcc/Vss
            // 16 x ADC10CLKs
    ADC10CTL0 = SREF_0 + ADC10SHT_2 + ADC10ON;
            //A0, ADC10CLK
    ADC10CTL1 =  INCH_0 + SHS_0 + ADC10SSEL_0 + ADC10DIV_0 + CONSEQ_0;
    ADC10AE0 = A0;      // P1.0

    ADC10CTL0 |= ENC;    
} // ADC_init

void initTimer_A(void)
{
    //Timer0_A3 Configuration
    TACCR0 = 0; //Initially, Stop the Timer
    TACCTL0 |= CCIE; //Enable interrupt for CCR0.
    TACTL = TASSEL_2 + ID_0 + MC_1; //Select SMCLK, SMCLK/1, Up Mode
}

void delayMS(int msecs)
{
    OFCount = 0; //Reset Over-Flow counter
    TACCR0 = 1000-1; //Start Timer, Compare value for Up Mode to get 1ms delay per loop
    //Total count = TACCR0 + 1. Hence we need to subtract 1.

    while(OFCount<=msecs);

    TACCR0 = 0; //Stop Timer
}

void update_display(unsigned short int mode) {
    unsigned long int fpart= energy/10000;//convert to nJ
    clear();
    switch (mode){
        case 0:
            MoveCursor(0,7);
            delayMS(100);
            SendByte(0x04, FALSE);
            SendByte(0x4A, TRUE);// 'J'
            SendByte(0x6E, TRUE);// 'n'
            if (fpart == 0){
                SendByte(0x30 + (fpart%10), TRUE);
            }else{
                while (fpart > 0) {
                    SendByte(0x30 + (fpart%10), TRUE);
                    fpart /= 10;
                }
            }
            MoveCursor(1,0);
            delayMS(100);
            SendByte(0x06, FALSE); 
            PrintStr("Hold btn");
            break;
        case 1:
            MoveCursor(0,0);
            delayMS(100);
            SendByte(0x06, FALSE);
            PrintStr("Measure");
            break;
        case 2:
            MoveCursor(0,0);
            delayMS(100);
            SendByte(0x06, FALSE);
            PrintStr("Hold");
            MoveCursor(1,0);
            delayMS(50);
            SendByte(0x06, FALSE);
            PrintStr("Button");
            break;
    }
}
//Timer ISR
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_CCR0_ISR(void)
{
    OFCount++; //Increment Over-Flow Counter
}
