//! \file
#ifndef _DoubleHot_INCLUDED
#define _DoubleHot_INCLUDED 1

#include "y/graphic/color/ramp.hpp"

namespace upsylon { namespace Graphic {

//! DoubleHot color ramp
class DoubleHot : public Ramp {
 public: explicit DoubleHot() throw();
         virtual ~DoubleHot() throw();
 private: Y_DISABLE_COPY_AND_ASSIGN(DoubleHot);
  virtual const rgb *_256colors() const throw();
};

}}
#endif
