#include "yellow_to_blue.hpp"

namespace upsylon { namespace Graphic {

 YellowToBlue:: YellowToBlue() throw() : Ramp() {}
 YellowToBlue::~YellowToBlue() throw() {}
 const rgb * YellowToBlue:: _256colors() const throw() {
  static const rgb _[256] = {
	rgb(0xff,0xff,0x00), rgb(0xfe,0xfe,0x00), rgb(0xfd,0xfd,0x01), rgb(0xfc,0xfc,0x02), rgb(0xfa,0xfa,0x03), rgb(0xf9,0xf9,0x04), rgb(0xf8,0xf8,0x05), rgb(0xf7,0xf7,0x06),
	rgb(0xf7,0xf7,0x07), rgb(0xf6,0xf6,0x08), rgb(0xf5,0xf5,0x09), rgb(0xf4,0xf4,0x0a), rgb(0xf2,0xf2,0x0b), rgb(0xf1,0xf1,0x0c), rgb(0xf0,0xf0,0x0d), rgb(0xef,0xef,0x0e),
	rgb(0xef,0xef,0x0f), rgb(0xee,0xee,0x10), rgb(0xed,0xed,0x11), rgb(0xec,0xec,0x12), rgb(0xea,0xea,0x13), rgb(0xe9,0xe9,0x14), rgb(0xe8,0xe8,0x15), rgb(0xe7,0xe7,0x16),
	rgb(0xe7,0xe7,0x17), rgb(0xe6,0xe6,0x18), rgb(0xe5,0xe5,0x19), rgb(0xe4,0xe4,0x1a), rgb(0xe2,0xe2,0x1b), rgb(0xe1,0xe1,0x1c), rgb(0xe0,0xe0,0x1d), rgb(0xdf,0xdf,0x1e),
	rgb(0xdf,0xdf,0x1f), rgb(0xde,0xde,0x21), rgb(0xdd,0xdd,0x21), rgb(0xdc,0xdc,0x23), rgb(0xda,0xda,0x23), rgb(0xd9,0xd9,0x25), rgb(0xd8,0xd8,0x25), rgb(0xd7,0xd7,0x27),
	rgb(0xd7,0xd7,0x27), rgb(0xd6,0xd6,0x29), rgb(0xd5,0xd5,0x29), rgb(0xd4,0xd4,0x2b), rgb(0xd2,0xd2,0x2b), rgb(0xd1,0xd1,0x2d), rgb(0xd0,0xd0,0x2d), rgb(0xcf,0xcf,0x2f),
	rgb(0xcf,0xcf,0x2f), rgb(0xce,0xce,0x31), rgb(0xcd,0xcd,0x31), rgb(0xcc,0xcc,0x33), rgb(0xca,0xca,0x33), rgb(0xc9,0xc9,0x35), rgb(0xc8,0xc8,0x35), rgb(0xc7,0xc7,0x37),
	rgb(0xc7,0xc7,0x37), rgb(0xc6,0xc6,0x39), rgb(0xc5,0xc5,0x39), rgb(0xc4,0xc4,0x3b), rgb(0xc2,0xc2,0x3b), rgb(0xc1,0xc1,0x3d), rgb(0xc0,0xc0,0x3d), rgb(0xbf,0xbf,0x3f),
	rgb(0xbf,0xbf,0x3f), rgb(0xbe,0xbe,0x40), rgb(0xbd,0xbd,0x42), rgb(0xbc,0xbc,0x43), rgb(0xba,0xba,0x43), rgb(0xb9,0xb9,0x44), rgb(0xb8,0xb8,0x46), rgb(0xb7,0xb7,0x47),
	rgb(0xb7,0xb7,0x47), rgb(0xb6,0xb6,0x48), rgb(0xb5,0xb5,0x4a), rgb(0xb4,0xb4,0x4b), rgb(0xb2,0xb2,0x4b), rgb(0xb1,0xb1,0x4c), rgb(0xb0,0xb0,0x4e), rgb(0xaf,0xaf,0x4f),
	rgb(0xaf,0xaf,0x4f), rgb(0xae,0xae,0x50), rgb(0xad,0xad,0x52), rgb(0xac,0xac,0x53), rgb(0xaa,0xaa,0x53), rgb(0xa9,0xa9,0x54), rgb(0xa8,0xa8,0x56), rgb(0xa7,0xa7,0x57),
	rgb(0xa7,0xa7,0x57), rgb(0xa6,0xa6,0x58), rgb(0xa5,0xa5,0x5a), rgb(0xa4,0xa4,0x5b), rgb(0xa2,0xa2,0x5b), rgb(0xa1,0xa1,0x5c), rgb(0xa0,0xa0,0x5e), rgb(0x9f,0x9f,0x5f),
	rgb(0x9f,0x9f,0x5f), rgb(0x9e,0x9e,0x60), rgb(0x9d,0x9d,0x62), rgb(0x9c,0x9c,0x63), rgb(0x9a,0x9a,0x63), rgb(0x99,0x99,0x64), rgb(0x98,0x98,0x66), rgb(0x97,0x97,0x67),
	rgb(0x97,0x97,0x67), rgb(0x96,0x96,0x68), rgb(0x95,0x95,0x6a), rgb(0x94,0x94,0x6b), rgb(0x92,0x92,0x6b), rgb(0x91,0x91,0x6c), rgb(0x90,0x90,0x6e), rgb(0x8f,0x8f,0x6f),
	rgb(0x8f,0x8f,0x6f), rgb(0x8e,0x8e,0x70), rgb(0x8d,0x8d,0x72), rgb(0x8c,0x8c,0x73), rgb(0x8a,0x8a,0x73), rgb(0x89,0x89,0x74), rgb(0x88,0x88,0x76), rgb(0x87,0x87,0x77),
	rgb(0x87,0x87,0x77), rgb(0x86,0x86,0x78), rgb(0x85,0x85,0x7a), rgb(0x84,0x84,0x7b), rgb(0x82,0x82,0x7b), rgb(0x81,0x81,0x7c), rgb(0x80,0x80,0x7e), rgb(0x7f,0x7f,0x7f),
	rgb(0x7f,0x7f,0x7f), rgb(0x7e,0x7e,0x80), rgb(0x7c,0x7c,0x81), rgb(0x7b,0x7b,0x82), rgb(0x7b,0x7b,0x84), rgb(0x7a,0x7a,0x85), rgb(0x78,0x78,0x86), rgb(0x77,0x77,0x87),
	rgb(0x77,0x77,0x87), rgb(0x76,0x76,0x88), rgb(0x74,0x74,0x89), rgb(0x73,0x73,0x8a), rgb(0x73,0x73,0x8c), rgb(0x72,0x72,0x8d), rgb(0x70,0x70,0x8e), rgb(0x6f,0x6f,0x8f),
	rgb(0x6f,0x6f,0x8f), rgb(0x6e,0x6e,0x90), rgb(0x6c,0x6c,0x91), rgb(0x6b,0x6b,0x92), rgb(0x6b,0x6b,0x94), rgb(0x6a,0x6a,0x95), rgb(0x68,0x68,0x96), rgb(0x67,0x67,0x97),
	rgb(0x67,0x67,0x97), rgb(0x66,0x66,0x98), rgb(0x64,0x64,0x99), rgb(0x63,0x63,0x9a), rgb(0x63,0x63,0x9c), rgb(0x62,0x62,0x9d), rgb(0x60,0x60,0x9e), rgb(0x5f,0x5f,0x9f),
	rgb(0x5f,0x5f,0x9f), rgb(0x5e,0x5e,0xa0), rgb(0x5c,0x5c,0xa1), rgb(0x5b,0x5b,0xa2), rgb(0x5b,0x5b,0xa4), rgb(0x5a,0x5a,0xa5), rgb(0x58,0x58,0xa6), rgb(0x57,0x57,0xa7),
	rgb(0x57,0x57,0xa7), rgb(0x56,0x56,0xa8), rgb(0x54,0x54,0xa9), rgb(0x53,0x53,0xaa), rgb(0x53,0x53,0xac), rgb(0x52,0x52,0xad), rgb(0x50,0x50,0xae), rgb(0x4f,0x4f,0xaf),
	rgb(0x4f,0x4f,0xaf), rgb(0x4e,0x4e,0xb0), rgb(0x4c,0x4c,0xb1), rgb(0x4b,0x4b,0xb2), rgb(0x4b,0x4b,0xb4), rgb(0x4a,0x4a,0xb5), rgb(0x48,0x48,0xb6), rgb(0x47,0x47,0xb7),
	rgb(0x47,0x47,0xb7), rgb(0x46,0x46,0xb8), rgb(0x44,0x44,0xb9), rgb(0x43,0x43,0xba), rgb(0x43,0x43,0xbc), rgb(0x42,0x42,0xbd), rgb(0x40,0x40,0xbe), rgb(0x3f,0x3f,0xbf),
	rgb(0x3f,0x3f,0xbf), rgb(0x3d,0x3d,0xc0), rgb(0x3d,0x3d,0xc1), rgb(0x3b,0x3b,0xc2), rgb(0x3b,0x3b,0xc4), rgb(0x39,0x39,0xc5), rgb(0x39,0x39,0xc6), rgb(0x37,0x37,0xc7),
	rgb(0x37,0x37,0xc7), rgb(0x35,0x35,0xc8), rgb(0x35,0x35,0xc9), rgb(0x33,0x33,0xca), rgb(0x33,0x33,0xcc), rgb(0x31,0x31,0xcd), rgb(0x31,0x31,0xce), rgb(0x2f,0x2f,0xcf),
	rgb(0x2f,0x2f,0xcf), rgb(0x2d,0x2d,0xd0), rgb(0x2d,0x2d,0xd1), rgb(0x2b,0x2b,0xd2), rgb(0x2b,0x2b,0xd4), rgb(0x29,0x29,0xd5), rgb(0x29,0x29,0xd6), rgb(0x27,0x27,0xd7),
	rgb(0x27,0x27,0xd7), rgb(0x25,0x25,0xd8), rgb(0x25,0x25,0xd9), rgb(0x23,0x23,0xda), rgb(0x23,0x23,0xdc), rgb(0x21,0x21,0xdd), rgb(0x21,0x21,0xde), rgb(0x1f,0x1f,0xdf),
	rgb(0x1e,0x1e,0xdf), rgb(0x1d,0x1d,0xe0), rgb(0x1c,0x1c,0xe1), rgb(0x1b,0x1b,0xe2), rgb(0x1a,0x1a,0xe4), rgb(0x19,0x19,0xe5), rgb(0x18,0x18,0xe6), rgb(0x17,0x17,0xe7),
	rgb(0x16,0x16,0xe7), rgb(0x15,0x15,0xe8), rgb(0x14,0x14,0xe9), rgb(0x13,0x13,0xea), rgb(0x12,0x12,0xec), rgb(0x11,0x11,0xed), rgb(0x10,0x10,0xee), rgb(0x0f,0x0f,0xef),
	rgb(0x0e,0x0e,0xef), rgb(0x0d,0x0d,0xf0), rgb(0x0c,0x0c,0xf1), rgb(0x0b,0x0b,0xf2), rgb(0x0a,0x0a,0xf4), rgb(0x09,0x09,0xf5), rgb(0x08,0x08,0xf6), rgb(0x07,0x07,0xf7),
	rgb(0x06,0x06,0xf7), rgb(0x05,0x05,0xf8), rgb(0x04,0x04,0xf9), rgb(0x03,0x03,0xfa), rgb(0x02,0x02,0xfc), rgb(0x01,0x01,0xfd), rgb(0x00,0x00,0xfe), rgb(0x00,0x00,0xff)  };
 return _; }
}}
