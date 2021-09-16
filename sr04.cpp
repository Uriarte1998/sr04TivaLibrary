#include "sr04.hpp"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include "inc/hw_timer.h"
#include "driverlib/debug.h"
#include "driverlib/timer.h"

#define SR04_Const 58

int32_t SR04::tEcho = 0;
bool SR04::pAlto = false;

int16_t SR04::calculoDistancia(){
    SR04::timerReset();
    return SR04::tEcho / SR04_Const;
};

void SR04::timerReset()
{
    uint32_t ui32Base = TIMER0_BASE;
    uint32_t ui32Timer = TIMER_A;
    uint32_t offset = (ui32Timer == TIMER_A) ? TIMER_O_TAV : TIMER_O_TBV;
    HWREG(ui32Base + offset) = 0;
}
void SR04::timerPulseInic(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC_UP);
    TimerLoadSet(TIMER1_BASE, TIMER_A ,60000000/2); //quiero que cuente medio segundo entonces 60000000
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT );
    TimerIntRegister(TIMER1_BASE, TIMER_A, SR04::interrupcionPulse);
    IntEnable(INT_TIMER1A);
    IntMasterEnable();
    TimerEnable(TIMER1_BASE, TIMER_A);
};
void SR04::pulsoInic(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0| GPIO_PIN_4, 0x00);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE,GPIO_PIN_5);
    GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0x00);
};
void SR04::timerEchoInic(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC_UP);
    TimerLoadSet(TIMER0_BASE, TIMER_A , 120000000*4);
    TimerEnable(TIMER0_BASE, TIMER_A);
};
void SR04::echoPinInic(){
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
  GPIOPinTypeGPIOInput(GPIO_PORTM_BASE, GPIO_PIN_4);
  GPIOPadConfigSet(GPIO_PORTM_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPD);
  // interrupt
  GPIOIntTypeSet(GPIO_PORTM_BASE, GPIO_PIN_4, GPIO_HIGH_LEVEL);
  GPIOIntRegister(GPIO_PORTM_BASE, SR04::interrupcionEcho);
  GPIOIntEnable(GPIO_PORTM_BASE, GPIO_PIN_4);
}


void SR04::sr04Inic()
{
    SR04::timerPulseInic();
    SR04::pulsoInic();
    SR04::timerEchoInic();
    SR04::echoPinInic();
}
