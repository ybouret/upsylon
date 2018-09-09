//! \file
#ifndef Y_FOURCC_INCLUDED
#define Y_FOURCC_INCLUDED

#include "y/os/platform.hpp"

#define Y_FOURCC0(VALUE) ( uint32_t(uint8_t(VALUE)) )
#define Y_FOURCC1(VALUE) ( Y_FOURCC0(VALUE) << 0x08 )
#define Y_FOURCC2(VALUE) ( Y_FOURCC0(VALUE) << 0x10 )
#define Y_FOURCC3(VALUE) ( Y_FOURCC0(VALUE) << 0x18 )

#define Y_FOURCC(A,B,C,D) (Y_FOURCC3(A) | Y_FOURCC2(B) | Y_FOURCC3(C) | Y_FOURCC0(D))

#endif

