//! \file
#ifndef Y_CODE_ILOG2_INCLUDED
#define Y_CODE_ILOG2_INCLUDED 1

#include "y/os/platform.hpp"
namespace upsylon
{

    //! integer log2
    template <size_t> struct ilog2;

    //! sizeof=1
    template <> struct ilog2<1>   { enum { value = 0  }; };
    //! sizeof=2
    template <> struct ilog2<2>   { enum { value = 1  }; };
    //! sizeof=4
    template <> struct ilog2<4>   { enum { value = 2  }; };
    //! sizeof=8
    template <> struct ilog2<8>   { enum { value = 3  }; };
    //! sizeof=16
    template <> struct ilog2<16>  { enum { value = 4  }; };
    //! sizeof=32
    template <> struct ilog2<32>  { enum { value = 5  }; };
    //! sizeof=64
    template <> struct ilog2<64>  { enum { value = 6  }; };
    //! sizeof=128
    template <> struct ilog2<128> { enum { value = 7  }; };
    //! sizeof=256
    template <> struct ilog2<256> { enum { value = 8  }; };
    //! sizeof=512
    template <> struct ilog2<512> { enum { value = 9  }; };
    //! sizeof=1024
    template <> struct ilog2<1024> { enum { value = 10  }; };

    //! proxy to get ilog2 using the type size
    template <typename T>
    struct ilog2_of { enum { value = ilog2<sizeof(T)>::value }; };


}
#endif
