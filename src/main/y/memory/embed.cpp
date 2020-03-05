
#include "y/memory/embed.hpp"
#include "y/code/round.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace memory
    {
        embed:: ~embed() throw()
        {
        }

        embed:: embed(void **pp, const size_t nb) throw() :
        length( nb ),
        offset( 0  ),
        ppHead( pp )
        {
            assert( ppHead );
            assert( NULL == *ppHead );
            assert( length>0 );
        }

        embed:: embed( const embed &other ) throw() :
        length( other.length ),
        offset( other.offset ),
        ppHead( other.ppHead )
        {
        }

        void * embed:: create(embed             *emb,
                              const size_t       num,
                              memory::allocator &mem,
                              size_t            &bytes,
                              size_t            *data)
        {
            assert( emb != NULL );
            assert( num >  0    );

            // compute structure
            bytes         = 0;
            for(size_t i=0;i<num;++i)
            {
                embed &em = emb[i];
                aliasing::_(em.offset) = bytes;
                const size_t alen = memory::align(em.length);
                bytes += alen;
                //std::cerr << "emb: @" << em.offset << "+" << alen << "/" << em.length << " -> " << bytes << std::endl;
            }

            // allocated memory
            if(data) *data = bytes;
            void     *addr = mem.acquire(bytes);

            //link
            {
                char *p = static_cast<char *>(addr);
                for(size_t i=0;i<num;++i)
                {
                    embed &em    = emb[i];
                    *(em.ppHead) = &p[ em.offset ];
                }
            }

            // done
            return addr;
        }
    }

}

#include "y/memory/global.hpp"

namespace upsylon
{
    namespace memory
    {
        void *embed:: create_global(embed *emb, const size_t num, size_t &bytes, size_t *data)
        {
            static memory::allocator &mem = memory::global::instance();
            return embed::create(emb,num,mem,bytes,data);
        }
    }
}
