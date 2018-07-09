
#include "y/memory/vein.hpp"

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
                std::cerr << "build N=" << N << std::endl;
                new (base+N*sizeof(nuggets<N>)) nuggets<N>();
            }
        };

        template <size_t NMIN,size_t N> struct nuggets_ops
        {
            static inline void build(char *base) throw()
            {
                nuggets_ops<NMIN,N-1>::build(base);
                std::cerr << "build N=" << N << "/NMIN=" << NMIN << std::endl;
                new (base+N*sizeof(nuggets<N>)) nuggets<N>();
            }
        };


        vein:: vein() throw() :
        entry(0),
        workspace()
        {
            std::cerr << "sizeof(workspace)=" << sizeof(workspace) << std::endl;
            std::cerr << "count=" << count << std::endl;
            //char toto[16384];
            nuggets_ops<min_bits,max_bits>::build(workspace);

            //entry = (nuggets_proto *)(addr-delta);
            const ptrdiff_t delta = min_bits * sizeof(nuggets_proto);

            if(false)
            {
                for(size_t i=min_bits;i<=max_bits;++i)
                {
                    //nuggets_proto *p = entry+i;
                    //std::cerr << p->block_bits() << std::endl;
                }
            }
        }

        vein:: ~vein() throw()
        {
        }

    }
}
