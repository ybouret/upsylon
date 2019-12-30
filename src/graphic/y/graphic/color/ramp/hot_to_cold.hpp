//! \file
#ifndef _HotToCold_INCLUDED
#define _HotToCold_INCLUDED 1

#include "y/graphic/color/ramp.hpp"

namespace upsylon { namespace Graphic {

//! HotToCold color ramp
class HotToCold : public Ramp {
 public: explicit HotToCold() throw();
         virtual ~HotToCold() throw();
 private: Y_DISABLE_COPY_AND_ASSIGN(HotToCold);
  virtual const rgb *_256colors() const throw();
};

}}
#endif
