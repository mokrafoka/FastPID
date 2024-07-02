#include "SmallPid.h"
#include <stdint.h>

#ifndef __AVR
#include <iostream>
#endif

/// fast systems
using cfgT = PID::Cfg<0.45f, 0.6f, 0.0f, 1.f>;
/// slow systems (measured value is lagging about 4 values after set point)
/// ./test.py -p 0.28 -i 0.2 -d 0.1 -n 400 --pid FastPID reference


using pidT = PID::SmallPID<cfgT>;

pidT localPid;

int main(){
#ifndef __AVR
   std::cerr << "Setup: " << std::endl;
   std::cerr << "P: " << cfgT::p << " I: " << cfgT::i << " D: " << cfgT::d << std::endl;
   std::cerr << "error: " << cfgT::cfgErr << std::endl;
#endif

   int16_t measuredFeedback = 0;
   int16_t pidCtrlVal = 0;
   uint32_t setArr[]{  0,   0, 100, 100, 100, 100, 100, 100, 100, 100,
                     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
                     100, 100, 100, 100, 100,  95,  95,  95,  95,  95,
                      95,  95,  95,  95,  95, 100, 100, 100, 100, 100,
                     100, 100, 100, 100, 100, 100, 100, 100, 100, 100,};
   for (auto sp: setArr){
      measuredFeedback = pidCtrlVal;
      pidCtrlVal = localPid.step(sp, measuredFeedback);
#ifndef __AVR
      std::cerr << "sp: " << sp << ", fb: " << measuredFeedback << ", output: " << pidCtrlVal << std::endl;
#endif
   }

   return 0;
}
