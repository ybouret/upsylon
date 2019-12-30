//! \file
#ifndef _YellowToBlue_INCLUDED
#define _YellowToBlue_INCLUDED 1

#include "y/graphic/color/ramp.hpp"

namespace upsylon { namespace Graphic {

//! YellowToBlue color ramp
class YellowToBlue : public Ramp {
 public: explicit YellowToBlue() throw();
         virtual ~YellowToBlue() throw();
 private: Y_DISABLE_COPY_AND_ASSIGN(YellowToBlue);
  virtual const rgb *_256colors() const throw();
};

}}
#endif
