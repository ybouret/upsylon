
#include "y/oxide/field1d.hpp"
#include "y/memory/global.hpp"

namespace upsylon
{

    namespace Oxide
    {

        namespace Kernel
        {

            Block1D:: ~Block1D() throw()
            {
                memory::global::location().release(dataAddress,privateMemory);
            }



            Block1D:: Block1D( const size_t items, const size_t item_size) :
            privateMemory( items * item_size ),
            dataAddress( memory::global::instance().acquire(privateMemory) )
            {
            }


            Block1D:: Block1D() throw() :
            privateMemory(0),
            dataAddress(0)
            {
            }



        }

    }

}

