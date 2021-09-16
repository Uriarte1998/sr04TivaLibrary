#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include "inc/hw_timer.h"
#include "driverlib/debug.h"
#include "driverlib/timer.h"

class SR04
{
private:

public:
  static int32_t tEcho;
  static bool pAlto;
  uint8_t distancia = 0;

  static void interrupcionEcho(){
      tEcho = TimerValueGet(TIMER0_BASE, TIMER_A)/120;
  }
  static void interrupcionPulse(){
      TimerIntClear(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
      if(pAlto){
          GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0| GPIO_PIN_4, 0x00);
          GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0x00);
      }
      else{
          GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0| GPIO_PIN_4, 0xFF);
          GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0xFF);
      }
      pAlto = !pAlto;
  }
  int16_t calculoDistancia();
  void timerReset();
  void timerPulseInic();
  void pulsoInic();
  void timerEchoInic();
  void echoPinInic();
  void sr04Inic();


};
