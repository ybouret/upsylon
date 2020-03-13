
#include "y/counting/permuter.hpp"
#include "y/counting/perm.hpp"
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

    test_permops<1>();
    test_permops<2>();
    test_permops<3>();
    test_permops<4>();
    test_permops<5>();
    test_permops<6>();
    test_permops<7>();

    for(int iarg=1;iarg<argc;++iarg)
    {
        const size_t n =  string_convert::to<size_t>(argv[iarg]);

        permutation         perm(n);
        vector<permutation> Perm( perm.count, as_capacity );

        std::cerr << "#perm(" << n << ")=" << perm.count << std::endl;
        for( perm.start(); perm.valid(); perm.next() )
        {
            std::cerr << "\t" << perm;
            std::cerr << " (";
            for(size_t i=0;i<n;++i) std::cerr << ' ' << perm(i) ;
            std::cerr << " )" << std::endl;
            Perm.push_back_(perm);
        }
        std::cerr << "#perm(" << n << ")=" << perm.count << std::endl;
        std::cerr << "checking..." << std::endl;
        for(size_t i=1;i<=Perm.size();++i)
        {
            for(size_t j=i;j<=Perm.size();++j)
            {
                permutation::memchk(Perm[i],Perm[j]);
            }
        }
    }

}
Y_UTEST_DONE()

#include "y/sort/unique.hpp"
Y_UTEST(anagram)
{
    if(argc>1)
    {
        const string org = argv[1];
        const size_t num = org.length();
        permutation  perm(num);
        vector<string> all( perm.count, as_capacity );
        for( perm.start(); perm.valid(); perm.next() )
        {
            string ana;
            for(size_t i=0;i<perm.n;++i)
            {
                ana << org[ perm(i) ];
            }
            all << ana;
        }
        unique(all);
        for(size_t i=1;i<=all.size();++i)
        {
            std::cerr << all[i] << std::endl;
        }
    }

}
Y_UTEST_DONE()

#include "y/counting/permuter.hpp"

Y_UTEST(permuter)
{
    size_t n = 5;
    if(argc>1)
    {
        n = string_convert::to<size_t>(argv[1],"n");
    }
    vector<uint16_t> data(n,5);
    for(size_t i=data.size();i>0;--i)
    {
        data[i] = uint16_t( alea.leq(10) );
    }

    std::cerr << "data=" << data << std::endl;
    permuter<uint16_t> P( data );
    std::cerr << "P =" << P << std::endl;
    std::cerr << "#P=" << P.count << "/" << mpn::factorial(n) << std::endl;

    

}
Y_UTEST_DONE()
