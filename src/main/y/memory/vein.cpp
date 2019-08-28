
#include "y/memory/vein.hpp"
#include "y/memory/nuggets.hpp"
#include "y/memory/global.hpp"
#include "y/os/static-check.hpp"
#include "y/exceptions.hpp"
#include "y/type/self-destruct.hpp"

#include <cerrno>

namespace upsylon
{
    namespace memory
    {

        namespace
        {
            template <size_t NMIN,size_t N> struct nuggets_ops;

            template <size_t N>
            struct nuggets_ops<N,N>
            {
                static inline void make( char *base ) throw()
                {
                    new (base) nuggets_for<N>();
                }

                static inline void kill( char *base ) throw()
                {
                    self_destruct( *(nuggets_for<N> *)base );
                }
            };

            template <size_t NMIN,size_t N> struct nuggets_ops
            {
                static inline void make(char *base) throw()
                {
                    // pre-order make
                    nuggets_ops<NMIN,N-1>::make(base);
                    new (base+(N-NMIN)*sizeof(nuggets_for<N>)) nuggets_for<N>();
                }

                static inline void kill( char *base ) throw()
                {
                    // post-order kill
                    char *addr = base +(N-NMIN)*sizeof(nuggets_for<N>);
                    self_destruct( *(nuggets_for<N> *)addr );

                    nuggets_ops<NMIN,N-1>::kill(base);
                }
            };

        }

        void vein:: clr() throw()
        {
            memset( &workspace[0][0],0,sizeof(workspace) );
        }

        vein:: vein() throw() :
        workspace()
        {
            Y_STATIC_CHECK(proto_size==sizeof(nuggets_for<min_bits>),workspace_mismatch);
            clr();
            nuggets_ops<min_bits,max_bits>::make(&workspace[0][0]);
        }

        vein:: ~vein() throw()
        {
            nuggets_ops<min_bits,max_bits>::kill(&workspace[0][0]);
            clr();
        }



        size_t vein:: bytes_for(const size_t length,
                                size_t      &ibit )
        {
            if(length<=min_size)
            {
                ibit = min_bits;
                return min_size;
            }
            else
            {
                if(length>max_size)
                {
                    ibit = 0;
                    throw libc::exception(EDOM,"vein.bytes_for(length=%lu>%lu)", (unsigned long)length, (unsigned long)(max_size) );
                }
                else
                {
                    for(ibit=min_bits;ibit<=max_bits;++ibit)
                    {
                        const size_t szp2 = (size_t(1)<<ibit);
                        if( length<=szp2 )
                        {
                            return szp2;
                        }
                    }
                    throw exception("vein.bytes_for(%lu) FAILURE",(unsigned long)length);
                }
            }

        }

        void * vein:: acquire(size_t &n)
        {
            if(n>max_size)
            {
                try
                {
                    n = next_power_of_two(n);
                    return memory::global::instance().acquire(n);
                }
                catch(...)
                {
                    n=0;
                    throw;
                }
            }
            else
            {
                try
                {
                    size_t ibit = 0;
                    n           = bytes_for(n,ibit);
                    assert(ibit>=min_bits);
                    assert(ibit<=max_bits);
                    nuggets *mgr = (nuggets *)&workspace[ibit-min_bits][0];
                    assert(mgr->get_block_bits()==ibit);
                    return mgr->acquire();
                }
                catch(...)
                {
                    n=0;
                    throw;
                }
            }
        }

        void  vein:: release(void * &p, size_t &n) throw()
        {
            assert(p);
            assert(is_a_power_of_two(n));
            assert(n>=min_size);
            if(n<=max_size)
            {
                for(size_t i=max_bits;i>=min_bits;--i)
                {
                    if( 0 != ( (size_t(1)<<i) & n ) )
                    {
                        nuggets *mgr = (nuggets *)&workspace[i-min_bits][0];
                        assert(mgr->get_block_bits()==i);
                        assert(mgr->get_block_size()==n);
                        mgr->release(p);
                        p = 0;
                        n = 0;
                        return;
                    }

                }
                // never get here
                fatal_error("[vein.release] unexpected no matching nugget");
            }
            else
            {
                assert( memory::global::exists() );
                memory::global::location().release(p,n);
            }
        }


    }
}
