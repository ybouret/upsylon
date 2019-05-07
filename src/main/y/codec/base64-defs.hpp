//! \file
#ifndef Y_CODEC_BASE64_DEFS_INCLUDED
#define Y_CODEC_BASE64_DEFS_INCLUDED


//! number of 8-bits to hold multiple of 6 input
#define Y_BASE64_BITS_FOR(INPUT_BITS)   ( (((INPUT_BITS)/6)*8)+( (INPUT_BITS) % 6 > 0 ? 8 : 0 ))

//! number of bytes to be converted
#define Y_BASE64_BYTES_FOR(INPUT_BYTES) ( Y_BASE64_BITS_FOR(INPUT_BYTES<<3) >> 3 )

#endif

