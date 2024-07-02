#include "SmallPid.h"

namespace PID {


template <class CfgT>
bool SmallPID<CfgT>::setOutputConfig(int bits, bool sign) {
   // Set output bits
   if (bits > 16 || bits < 1) {
      setCfgErr();
   }
   else {
      if (bits == 16) {
         _outmax = (0xFFFFULL >> (17 - bits)) * Cfg::paramMult;
      }
      else{
         _outmax = (0xFFFFULL >> (16 - bits)) * Cfg::paramMult;
      }
      if (sign) {
         _outmin = -((0xFFFFULL >> (17 - bits)) + 1) * Cfg::paramMult;
      }
      else {
         _outmin = 0;
      }
   }
   return ! _cfg_err;
}

template <class CfgT>
bool SmallPID<CfgT>::setOutputRange(int16_t min, int16_t max)
{
   if (min >= max) {
      setCfgErr();
      return ! _cfg_err;
   }
   _outmin = int64_t(min) * Cfg::paramMult;
   _outmax = int64_t(max) * Cfg::paramMult;
   return ! _cfg_err;
}

template <class CfgT>
int16_t SmallPID<CfgT>::step(int16_t sp, int16_t fb) {

   // int16 + int16 = int17
   int32_t err = int32_t(sp) - int32_t(fb);
   int32_t P = 0, I = 0;
   int32_t D = 0;

   if (_p) {
      // uint16 * int16 = int32
      P = int32_t(_p) * int32_t(err);
   }

   if (_i) {
      // int17 * int16 = int33
      _sum += int64_t(err) * int64_t(_i);

      // Limit sum to 32-bit signed value so that it saturates, never overflows.
      if (_sum > Cfg::integMax)
         _sum = Cfg::integMax;
      else if (_sum < Cfg::integMin)
         _sum = Cfg::integMin;

      // int32
      I = _sum;
   }

   if (_d) {
      // (int17 - int16) - (int16 - int16) = int19
      int32_t deriv = (err - _last_err) - int32_t(sp - _last_sp);
      _last_sp = sp;
      _last_err = err;

      // Limit the derivative to 16-bit signed value.
      if (deriv > Cfg::derivMax)
         deriv = Cfg::derivMax;
      else if (deriv < Cfg::derivMin)
         deriv = Cfg::derivMin;

      // int16 * int16 = int32
      D = int32_t(_d) * int32_t(deriv);
   }

   // int32 (P) + int32 (I) + int32 (D) = int34
   int64_t out = int64_t(P) + int64_t(I) + int64_t(D);

   // Make the output saturate
   if (out > _outmax)
      out = _outmax;
   else if (out < _outmin)
      out = _outmin;

   // Remove the integer scaling factor.
   int16_t rval = out >> Cfg::paramShift;

   // Fair rounding.
   if (out & (0x1ULL << (Cfg::paramShift - 1))) {
      rval++;
   }

   return rval;
}


} // namespace PID
