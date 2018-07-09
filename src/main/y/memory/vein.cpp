
#include "y/memory/vein.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{
    namespace memory
    {

        template <size_t NMIN,size_t N> struct nuggets_ops;

        template <size_t N>
        struct nuggets_ops<N,N>
        {
            static inline void build( char *base ) throw()
            {
                new (base) nuggets<N>();
            }
        };

        template <size_t NMIN,size_t N> struct nuggets_ops
        {
            static inline void build(char *base) throw()
            {
                nuggets_ops<NMIN,N-1>::build(base);
                new (base+(N-NMIN)*sizeof(nuggets<N>)) nuggets<N>();
            }
        };


        vein:: vein() throw() :
        entry(0),
        workspace()
        {
            std::cerr << "sizeof(workspace)=" << sizeof(workspace) << std::endl;
            std::cerr << "count=" << count << ": " << min_bits << " -> " << max_bits << std::endl;

            char *addr = &workspace[0][0];
            nuggets_ops<min_bits,max_bits>::build(addr);

            entry  = (proto *)addr;
            entry -= min_bits;

            if(true)
            {
                for(size_t i=min_bits;i<=max_bits;++i)
                {
                    nuggets_manager *p = (nuggets_manager *) &entry[i];
                    std::cerr << p->get_block_bits() << std::endl;
                    assert(p->get_block_bits()==i);
                }
            }
        }

        vein:: ~vein() throw()
        {
        }



        size_t      vein:: bytes_for(const size_t length,
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
                        const size_t szp2 = (1<<ibit);
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
            try
            {
                size_t ibit = 0;
                n           = bytes_for(n,ibit);
                assert(ibit>=min_bits);
                assert(ibit<=max_bits);
                nuggets_manager *mgr = (nuggets_manager *)&entry[ibit];
                assert(mgr->get_block_bits()==ibit);
                return mgr->acquire();
            }
            catch(...)
            {
                n=0;
                throw;
            }
        }

        void  vein:: release(void * &p, size_t &n) throw()
        {
            assert(p);
            assert(is_a_power_of_two(n));
            assert(n>=min_size);
            assert(n<=max_size);
            for(size_t ibit=max_bits;ibit>0;--ibit)
            {
                if( 0 != ( (1<<ibit) & n ) )
                {
                    std::cerr << "n=" << n << " -> " << ibit << std::endl;
                    {
                        size_t bits  = 0;
                        const size_t m = bytes_for(n,bits);
                        assert(m==n);
                        assert(bits==ibit);
                    }
                    nuggets_manager *mgr = (nuggets_manager *)(entry+ibit);
                    assert(mgr->get_block_bits()==ibit);
                    assert(mgr->get_block_size()==n);
                    mgr->release(p);
                    p = 0;
                    n = 0;
                    return;
                }

            }
            // critical error ?
            assert(die("vein.release failure"));
        }


    }
}
