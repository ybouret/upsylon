
#include "y/oxide/field/io.hpp"
#include "y/mpl/rational.hpp"
#include "y/string.hpp"

namespace upsylon
{
    namespace Oxide
    {

        IO::Block:: Block( const size_t n) :
        Bytes(n,as_capacity)
        {
        }

        IO::Block:: ~Block() throw()
        {
        }

        void IO::Block:: write(char C)
        {
            push_back( C );
        }

        void IO::Block:: flush()
        {
            
        }

        IO::Block &   IO::Block:: setFast(const size_t n)
        {
            adjust(n,0);
            return *this;
        }

        IO::Block &  IO::Block:: setZero(const size_t n)
        {
            make(n,0);
            return *this;
        }

        


    }

}

