#include "SmallPid.h"
#include <stdint.h>

using cfgT = PID::Cfg<0.2f, 0.4f, 0.5f, 1.f>;
using pidT = PID::SmallPID<cfgT>;

pidT localPid;

int main(){
   int16_t desiredVal = 0x815;
   int16_t measuredValue = 0x816;
   int16_t result = localPid.step(desiredVal, measuredValue);
   (void)result;
   return 0;
}
