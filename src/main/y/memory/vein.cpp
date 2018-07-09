
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
                //std::cerr << "build N=" << N << std::endl;
                new (base) nuggets<N>();
            }
        };

        template <size_t NMIN,size_t N> struct nuggets_ops
        {
            static inline void build(char *base) throw()
            {
                nuggets_ops<NMIN,N-1>::build(base);
                //std::cerr << "build N=" << N << "/NMIN=" << NMIN << std::endl;
                new (base+(N-NMIN)*sizeof(nuggets<N>)) nuggets<N>();
            }
        };


        vein:: vein() throw() :
        entry(0),
        workspace()
        {
            std::cerr << "sizeof(workspace)=" << sizeof(workspace) << std::endl;
            std::cerr << "count=" << count << std::endl;

            char *addr = &workspace[0][0];
            nuggets_ops<min_bits,max_bits>::build(addr);

            const ptrdiff_t delta = min_bits * psize;
            entry = (proto *)(addr-delta);

            if(true)
            {
                for(size_t i=min_bits;i<=max_bits;++i)
                {
                    nuggets_manager *p = (nuggets_manager *) &entry[i];
                    std::cerr << p->get_block_bits() << std::endl;
                }
            }
        }

        vein:: ~vein() throw()
        {
        }

        size_t      vein:: bytes_for( const size_t length, size_t &bits )
        {
            if(length<=min_size)
            {
                bits = min_bits;
                return min_size;
            }
            else
            {
                if(length>max_size)
                {
                    throw libc::exception(EDOM,"vein.bytes_for(length=%lu>%lu)", (unsigned long)length, (unsigned long)(max_size) );
                }
                else
                {
                    for(bits=min_bits;bits<=max_bits;++bits)
                    {
                        const size_t szp2 = (1<<bits);
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
                size_t bits = 0;
                n           = bytes_for(n,bits);
                assert(bits>=min_bits);
                assert(bits<=max_bits);
                nuggets_manager *mgr = (nuggets_manager *)(entry+bits);
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
