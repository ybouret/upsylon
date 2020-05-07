#include "y/oxide/field/io.hpp"
#include "y/exception.hpp"

#include "y/string.hpp"
#include "y/mpl/rational.hpp"
#include "y/type/complex.hpp"
#include "y/type/point3d.hpp"

namespace upsylon
{

    namespace Oxide
    {
        IO:: ~IO() throw()
        {
        }

        IO:: IO() : singleton<IO>(), ios::plugins()
        {
            ensure(32);
            
            declare_raw<char>();
            declare_raw<unsigned char>();
            declare_raw<int8_t>();
            declare_raw<uint8_t>();

            declare_nbo<short>();
            declare_nbo<unsigned short>();
            declare_nbo<int16_t>();
            declare_nbo<uint16_t>();

            declare_nbo<int>();
            declare_nbo<unsigned int>();

            declare_nbo<long>();
            declare_nbo<unsigned long>();

            declare_nbo<long long>();
            declare_nbo<unsigned long long>();

            declare_nbo<int32_t>();
            declare_nbo<uint32_t>();

            declare_nbo<int64_t>();
            declare_nbo<uint64_t>();

            declare_raw<double>();
            declare_raw<float>();

            declare_raw< complex<double> >();
            declare_raw< complex<float> >();

            declare_raw< point2d<double> >();
            declare_raw< point2d<float> >();

            declare_raw< point3d<double> >();
            declare_raw< point3d<float> >();

            declare_srz<string>();
            declare_srz<mpn>();
            declare_srz<mpz>();
            declare_srz<mpq>();


        }


    }

}

