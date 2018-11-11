
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



#define Y_IPSO_GHOST_CHECK() \
assert(allocated); assert(head); assert(tail); assert(last); assert(workspace); \
assert(head<=tail);assert(tail<=last); assert(head>=(uint8_t*)workspace)

#define Y_IPSO_GHOST_CTOR()        \
head(0),\
tail(0),\
last(0),\
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

        size_t ghost:: writable() const throw()
        {
            if(allocated)
            {
                Y_IPSO_GHOST_CHECK();
                return static_cast<size_t>(last-tail);
            }
            else
            {
                return 0;
            }
        }

        size_t ghost:: readable() const throw()
        {
            if(allocated)
            {
                Y_IPSO_GHOST_CHECK();
                return static_cast<size_t>(tail-head);
            }
            else
            {
                return 0;
            }
        }

        void ghost:: ensure( const size_t block_size )
        {
            size_t required = block_size * indices.size();
            if( required > allocated )
            {
                __release();
                allocated = required;
                workspace = memory::global::instance().acquire(allocated);
                head = tail = static_cast<uint8_t*>(workspace);
                last        = static_cast<uint8_t*>(workspace)+allocated;
            }
        }


        void ghost:: initialize() throw()
        {
            assert(allocated);
            head = tail = static_cast<uint8_t*>(workspace);
        }

        void ghost:: query( const field_info &F ) throw()
        {
            Y_IPSO_GHOST_CHECK();
            assert( writable() >= F.item_size * indices.size() );
            field_io::query(tail, F.address(), F.item_size,indices);
        }

        void ghost:: store( field_info &F ) throw()
        {
            Y_IPSO_GHOST_CHECK();
            assert( readable() >= F.item_size * indices.size() );
            field_io::store(head, F.address(), F.item_size, indices);
        }


    }
}
