#ifndef SmallPid_h
#define SmallPid_h

#define __STDC_LIMIT_MACROS
#include <stdint.h>

namespace PID {

template <float kpT, float kiT, float kdT, float hzT>
struct Cfg {
   static constexpr int32_t integMax    = INT32_MAX;
   static constexpr int32_t integMin    = INT32_MIN;
   static constexpr int16_t derivMax    = INT16_MAX;
   static constexpr int16_t derivMin    = INT16_MIN;

   static constexpr uint8_t paramShift  = 8;
   static constexpr uint8_t paramBits   = 16;
   static constexpr auto paramMax    = (((uint32_t{1} << paramBits)-1) >> paramShift);
   static constexpr auto paramMult   = (((uint32_t{1} << paramBits)) >> (paramBits - paramShift));

   static constexpr uint32_t p = kpT * paramMult;
   static constexpr uint32_t i = (kiT * paramMult) / hzT;
   static constexpr uint32_t d = kdT * hzT * paramMult;

   static constexpr bool getErr() {
      if (kpT < 0 || kpT > paramMax || (kpT > 0 && p == 0))
         return true;
      if (kiT < 0 || (kiT / hzT) > paramMax || (kiT > 0 && i == 0))
         return true;
      if (kdT < 0 || (kdT * hzT) > paramMax || (kdT > 0 && d == 0))
         return true;
      return false;
   }
   static constexpr bool cfgErr = getErr();
};

/**
  A fixed point PID controller with a 32-bit internal calculation pipeline.

  todo: output range could be configured as template parameter as
  well. However not sure if that makes sense.
*/
template <class CfgT>
class SmallPID {

public:

   using Cfg = CfgT;

   /// construct default object with bits = 16 and sign = false.
   SmallPID() {
      configure(16, false);
   }

   SmallPID(int bits, bool sign){
      configure(bits, sign);
   }

   ~SmallPID() = default;

   bool configure(int bits=16, bool sign=false){
      clear();
      _cfg_err = false;
      setOutputConfig(bits, sign);
      return ! _cfg_err;
   }


   bool setOutputConfig(int bits, bool sign);

   bool setOutputRange(int16_t min, int16_t max);

   void clear() {
      _last_sp = 0;
      _last_out = 0;
      _sum = 0;
      _last_err = 0;
   }

   int16_t step(int16_t sp, int16_t fb);

   bool err() {
      return _cfg_err;
   }

private:

   void setCfgErr(){
      _cfg_err = true;
   }

private:

   ///@{
   ///@name State and Configuration
   /// make sure they are size sorted to keep resources tight
   int64_t _sum{0};
   int64_t _outmax{0}, _outmin{0};
   int32_t _last_err{0};
   static constexpr uint32_t _p{Cfg::p};
   static constexpr uint32_t _i{Cfg::i};
   static constexpr uint32_t _d{Cfg::d};
   int16_t _last_sp{0}, _last_out{0};
   bool _cfg_err{Cfg::cfgErr};
   ///@}

};

} // namespace PID

#include "SmallPid.inl"

#endif // include SmallPid_h
