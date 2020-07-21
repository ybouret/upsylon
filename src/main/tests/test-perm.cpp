
#include "y/counting/permutations.hpp"
#include "y/counting/perm-ops.hpp"
#include "y/counting/ops.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"
#include "y/sequence/vector.hpp"
#include "y/type/ints-display.hpp"
#include "y/mpl/natural.hpp"
#include "y/memory/io.hpp"

#include <iomanip>

using namespace upsylon;

namespace {
    
    
    
    template <size_t N>
    void test_permops()
    {
        std::cerr << "<permops " << N << ">" << std::endl;
        size_t       wksp[N];
        size_t       addr[N];
        size_t      *P     = memory::io::__force<size_t>(wksp)-1;
        const mpn    np    = mpn::factorial(N);
        const size_t count = np.cast_to<size_t>("#perm");
        
        std::cerr << "count=" << count << std::endl;
        core::counting::init(P,N);
        display_int::to(std::cerr << "@init   $ ", wksp, N, ":") << std::endl;
        
        core::counting::init(P,N,addr);
        display_int::to(std::cerr << "@init   $ ", wksp, N, ":") << std::endl;
        display_int::to(std::cerr << "@init   $ ", addr, N, ",") << std::endl;
        
        for(size_t i=2;i<=count;++i)
        {
            Y_ASSERT(core::permutation::next(P,N));
            core::counting::to_C(addr,P,N);
            display_int::to(std::cerr << "@" << std::setw(6) << i << " $ ",wksp,N,":");
            display_int::to(std::cerr << " | @" << std::setw(6) << i << " $ ",addr,N,",") << std::endl;
        }
        
        Y_ASSERT(!core::permutation::next(P,N));
        
        
        std::cerr << "<permops/>" << std::endl << std::endl;
    }
}

Y_UTEST(perm)
{
    
    {
        test_permops<1>();
        test_permops<2>();
        test_permops<3>();
        test_permops<4>();
        test_permops<5>();
        test_permops<6>();
        test_permops<7>();
        //test_permops<8>();
    }
    
    
    for(int iarg=1;iarg<argc;++iarg)
    {
        const size_t n =  string_convert::to<size_t>(argv[iarg]);
        
        permutation         perm(n);
        {
            vector<permutation> Perm( perm.count, as_capacity );

            std::cerr << "#perm(" << n << ")=" << perm.count << std::endl;
            for( perm.boot(); perm.good(); perm.next() )
            {
                std::cerr << "\t" << (counting &)perm;
                std::cerr << " (";
                for(size_t i=0;i<n;++i) std::cerr << ' ' << perm(i) ;
                std::cerr << " )" << std::endl;
                Perm.push_back_(perm);
            }
            std::cerr << "#perm(" << n << ")=" << perm.count << std::endl;
            std::cerr << "checking generated.." << std::endl;
            for(size_t i=1;i<=Perm.size();++i)
            {
                for(size_t j=i;j<=Perm.size();++j)
                {
                    permutation::memchk(Perm[i],Perm[j]);
                }
            }
        }

        std::cerr << "checking reload..." << std::endl;
        {
            permutation temp(n);
            for(perm.boot();perm.good();perm.next())
            {
                temp.reload(perm);
                Y_ASSERT( permutation::are_equal(temp,perm) );
                permutation::memchk(temp,perm);
            }
        }


    }
    
}
Y_UTEST_DONE()


#include "y/counting/permutations.hpp"

Y_UTEST(permutations)
{

    string data = "abcde";
    if(argc>1)
    {
        data=argv[1];
    }

    if(data.size()>0)
    {
        permutations<char>   perms(*data,data.length());
        permutations<int>    iperm(*data,data.length());
        permutations<long>   lperm( iperm );

        for( perms.boot(); perms.good(); perms.next() )
        {
            std::cerr << "perm=" << (counting&)(*perms) << " => " << (accessible<char>&)perms << std::endl;
            const permutations<int64_t> uperm( perms );
            Y_ASSERT(perms.has_same_state_than(uperm));
        }
    }

    

}
Y_UTEST_DONE()

