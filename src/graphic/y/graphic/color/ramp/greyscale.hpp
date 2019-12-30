//! \file
#ifndef _Greyscale_INCLUDED
#define _Greyscale_INCLUDED 1

#include "y/graphic/color/ramp.hpp"

namespace upsylon { namespace Graphic {

//! Greyscale color ramp
class Greyscale : public Ramp {
 public: explicit Greyscale() throw();
         virtual ~Greyscale() throw();
 private: Y_DISABLE_COPY_AND_ASSIGN(Greyscale);
  virtual const rgb *_256colors() const throw();
};

}}
#endif
