#include "y/ipso/ghosts.hpp"

namespace upsylon
{
    namespace ipso
    {
        ghosts:: ~ghosts() throw()
        {
        }

        void ghosts:: ensure( const size_t block_size )
        {
            source.ensure(block_size);
            target.ensure(block_size);
        }

        void ghosts::exchange(const ghosts &a,
                              const ghosts &b,
                              field_info   &F) throw()
        {
            assert(a.target.indices.size()==b.source.indices.size());
            assert(a.source.indices.size()==b.target.indices.size());
            assert(a.target.indices.size()==a.source.indices.size());

            uint8_t     *p = static_cast<uint8_t *>(F.address());
            const size_t w = F.item_size;

            const coord1D *a_target = *a.target.indices;
            const coord1D *b_source = *b.source.indices;

            const coord1D *b_target = *b.target.indices;
            const coord1D *a_source = *a.source.indices;

            for(size_t i=a.target.indices.size();i>0;--i)
            {
                memcpy( &p[ w * a_target[i]], &p[ w * b_source[i] ], w );
                memcpy( &p[ w * b_target[i]], &p[ w * a_source[i] ], w );
            }
        }

    }
}
