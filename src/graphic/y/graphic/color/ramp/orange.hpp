//! \file
#ifndef _Orange_INCLUDED
#define _Orange_INCLUDED 1

#include "y/graphic/color/ramp.hpp"

namespace upsylon { namespace Graphic {

class Orange : public Ramp {
 public: explicit Orange() throw();
         virtual ~Orange() throw();
 private: Y_DISABLE_COPY_AND_ASSIGN(Orange);
  virtual const rgb *_256colors() const throw();
};

}}
#endif
