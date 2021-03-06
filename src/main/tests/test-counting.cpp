#include "y/counting/mloop.hpp"
#include "y/counting/comb.hpp"
#include "y/counting/perm.hpp"
#include "y/counting/permutations.hpp"
#include "y/counting/symm-pair.hpp"
#include "y/type/point3d.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/container/matrix.hpp"

using namespace upsylon;

namespace {
 
    static inline void infoOn(counting &loop)
    {
        Y_CHECK(1==loop.index);
        std::cerr << "index=" << loop.index << "/count=" << loop.count << "/space=" << loop.space << std::endl;
        std::cerr << loop << std::endl;

        bool has_endl = false;
        for(loop.boot();loop.good();loop.next())
        {
            std::cerr << ' ' << loop << " @" << loop.index;
            if(!(loop.index%4))
            {
                std::cerr << std::endl;
                has_endl = true;
            }
            else
            {
                has_endl = false;
            }
        }
        if(!has_endl) std::cerr << std::endl;

        for(size_t size=1;size<=4;++size)
        {
            std::cerr << "#" << size;
            for(size_t rank=0;rank<size;++rank)
            {
                size_t todo = loop.boot(size,rank);
                std::cerr << " " << size << "." << rank << ":@" << loop.index << "+" << todo;
                //std::cerr << '[';
                while(todo>0)
                {
                    Y_ASSERT(loop.good());
                    --todo;
                    loop.next();
                    //std::cerr << '.';
                }
                //std::cerr << ']';
            }
            std::cerr << std::endl;
        }

    }

    template <typename COUNTING> static inline
    void fill_all( COUNTING &self )
    {
        matrix<typename COUNTING::type> frame( self.count, self.space );
        counting::fill_frame(frame,self);
        std::cerr << frame << std::endl;
        std::cerr << std::endl;
    }
    
}

Y_UTEST(counting)
{
    
    {
        std::cerr << "mloop3D" << std::endl;
        const point3d<int> lo(1,2,3);
        const point3d<int> up(3,4,5);
        mloop< int,point3d<int> > loop(lo,up);
        infoOn(loop);
        fill_all(loop);
    }

    {
        combination comb(7,2);
        std::cerr << "comb" << std::endl;
        infoOn(comb);
        fill_all(comb);
    }
    
    {
        std::cerr << "perm" << std::endl;
        permutation perm(6);
        infoOn(perm);
        fill_all(perm);
    }

    {
        std::cerr << "symm" << std::endl;
        symm_pair symm(8);
        infoOn(symm);
        fill_all(symm);
    }


    {
        const size_t n = 1+alea.leq(5);
        vector<int>  I(n,as_capacity);
        for(size_t i=n;i>0;--i)
        {
            const int tmp = int(alea.leq(n));
            I << tmp;
        }
        std::cerr << "I=" << I << std::endl;
        permutations<int> Perm(I);
        infoOn(Perm);
        fill_all(Perm);
    }
    
}
Y_UTEST_DONE()

