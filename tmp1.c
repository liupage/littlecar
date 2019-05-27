  
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#define key GpioDataRegs.GPADAT.bit.GPIO12
//
// Function Prototypes
//
void delay(int time);
int scan_key(void);
void change(void);
uint16_t rate=1;

//
// Defines that configure the period for each timer
//

void main(void)
{

#ifdef _FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
#endif


    InitSysCtrl();


    InitEPwm1Gpio();



    uint16_t tone[8]={1,57306,51063,45489,42949,38265,34090,30364};
    short mus[14]={3,0,2,0,1,0,2,0,3,0,3,0,3,0};


 /*   DINT;


    InitPieCtrl();


    IER = 0x0000;
    IFR = 0x0000;


    InitPieVectTable();



*/

     EALLOW;
     EPwm1Regs.TBPRD= tone[0];
     EPwm1Regs.CMPA.half.CMPA=tone[0]/2;    // Clear PWM1A on event A, up count
     EDIS;


     EALLOW;

     EPwm1Regs.TBCTR = 0;                  // Clear counter

     EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
     EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading

     EPwm1Regs.TBCTL.bit.PRDLD=TB_SHADOW;
     EPwm1Regs.TBCTL.bit.SYNCOSEL=TB_SYNC_DISABLE;



     EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;   // Clock ratio to SYSCLKOUT
     EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;




     EPwm1Regs.AQCTLA.bit.ZRO=  AQ_CLEAR;
     EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;    // Clear PWM1A on event A, up count

     EDIS;
    //
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO12=00;  // All GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO12=0;   // All outputs
    GpioCtrlRegs.GPAPUD.bit.GPIO12=1;
    EDIS;



    int count=0;
    while(1)
 {
    EALLOW;
    EPwm1Regs.TBPRD = tone[mus[count]];
    EPwm1Regs.CMPA.half.CMPA = tone[mus[count]]/6*rate;
    EDIS;
    change();
    count=count>14?0:count+1;

 }


}






void delay(int time)
{
    long j=0;
    for(j=0;j<time*200000;j++);
}
int scan_key(void)
{
    if(key==1)
    {
        delay(1);
        if(key==1)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
        else
        {
            return 0;
        }


}
void change(void)
{
    long i=0;
    for(i=0;i<500000;i++)
    {
        if(scan_key()==1)
        {
            rate=(rate+2)%5+1;
        }
    }
}
