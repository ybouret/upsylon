
#include "y/memory/embed.hpp"
#include "y/type/aliasing.hpp"
#include "y/code/round.hpp"

namespace upsylon
{
    namespace memory
    {
        embed:: ~embed() throw()
        {
        }

        embed:: embed(void **pp, const size_t nb) throw() :
        params(0,nb),
        policy(by_hook)
        {

            hook = pp;
            assert(NULL!=  hook);
            assert(NULL== *hook);
            assert( params.length>0 );
        }

        embed:: embed(const size_t nb) throw() :
        params(0,nb),
        policy(by_addr)
        {

            addr=NULL;
            assert( params.length>0 );
        }



        embed:: embed( const embed &other ) throw() :
        params( other.params ),
        policy( other.policy )
        {
            addr = other.addr;
        }

        void * embed:: create(embed             *emb,
                              const size_t       num,
                              memory::allocator &mem,
                              size_t            &bytes,
                              size_t            *data)
        {
            assert( emb != NULL );
            assert( num >  0    );

            //------------------------------------------------------------------
            // compute structure
            //------------------------------------------------------------------
            bytes         = 0;
            for(size_t i=0;i<num;++i)
            {
                const marker &mk        = emb[i].params;
                aliasing::_(mk.offset)  = bytes;
                bytes                  += memory::align(mk.length);
            }

            //------------------------------------------------------------------
            // allocated memory
            //------------------------------------------------------------------
            if(data) *data = bytes;              // exact count
            void     *addr = mem.acquire(bytes); // now may be greater

            //------------------------------------------------------------------
            // link
            //------------------------------------------------------------------
            {
                char *p = static_cast<char *>(addr);
                for(size_t i=0;i<num;++i)
                {
                    embed &em    = emb[i];
                    assert(0==(em.params.offset%Y_MEMORY_ALIGNED));
                    void  *field  = &p[ em.params.offset ];
                    switch(em.policy)
                    {
                        case by_hook: (*em.hook) = field; break;
                        case by_addr:   em.addr  = field; break;
                    }
                }
            }

            //------------------------------------------------------------------
            // done
            //------------------------------------------------------------------
            return addr;
        }
    }

}


