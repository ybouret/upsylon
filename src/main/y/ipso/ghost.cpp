
#include "y/ipso/ghost.hpp"

namespace upsylon
{
    namespace ipso
    {
        ghost:: ~ghost() throw()
        {
            __release();
        }

        void ghost:: __release() throw()
        {
            memory::global::location().release(workspace,allocated);
        }
        

#define Y_IPSO_GHOST_CTOR()        \
data(0),                           \
indices( sub.items, as_capacity ), \
allocated( 0 ),                    \
workspace( NULL )

#define Y_IPSO_SETUP() do {           \
field_io::collect(indices, full, sub);\
ensure( block_size );                 \
} while(false)

        ghost:: ghost( const layout1D &full, const layout1D &sub, const size_t block_size ) :
        Y_IPSO_GHOST_CTOR()
        {
            Y_IPSO_SETUP();
        }

        ghost:: ghost( const layout2D &full, const layout2D &sub, const size_t block_size ) :
        Y_IPSO_GHOST_CTOR()
        {
            Y_IPSO_SETUP();
        }

        ghost:: ghost( const layout3D &full, const layout3D &sub, const size_t block_size ) :
        Y_IPSO_GHOST_CTOR()
        {
            Y_IPSO_SETUP();
        }


        void ghost:: ensure( const size_t block_size )
        {
            size_t required = block_size * indices.size();
            if( required > allocated )
            {
                __release();
                allocated = required;
                workspace = memory::global::instance().acquire(allocated);
            }
        }


        void ghost:: init() throw()
        {
            data = static_cast<uint8_t *>(workspace);
        }

        void ghost:: query( const field_info &F ) throw()
        {
            assert(allocated);
            assert(data);
            assert( data + F.item_size * indices.size() <= static_cast<uint8_t *>(workspace)+allocated );
            field_io::query(data,F.address(),F.item_size,indices);
        }

        void ghost:: store( field_info &F ) throw()
        {
            assert(allocated);
            assert(data);
            assert( data + F.item_size * indices.size() <= static_cast<uint8_t *>(workspace)+allocated );
            field_io::store(data, F.address(), F.item_size, indices);
        }


    }
}
