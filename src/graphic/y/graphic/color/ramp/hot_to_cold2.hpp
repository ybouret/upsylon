//! \file
#ifndef _HotToCold2_INCLUDED
#define _HotToCold2_INCLUDED 1

#include "y/graphic/color/ramp.hpp"

namespace upsylon { namespace Graphic {

//! HotToCold2 color ramp
class HotToCold2 : public Ramp {
 public: explicit HotToCold2() throw();
         virtual ~HotToCold2() throw();
 private: Y_DISABLE_COPY_AND_ASSIGN(HotToCold2);
  virtual const rgb *_256colors() const throw();
};

}}
#endif
