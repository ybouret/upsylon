
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


#include "y/counting/permuter.hpp"
#include "y/ios/ocstream.hpp"
#include "y/counting/part.hpp"

namespace {
 
    template <typename T>
    void doPerm(const size_t n, const size_t m)
    {
        const mpn    countMaxMP = mpn::factorial(n);
        const size_t countMax   = countMaxMP.cast_to<size_t>();
        std::cerr << "countMax=" << countMax << std::endl;
        
        vector<T> data(n,0);
        for(size_t i=1;i<=n;++i)
        {
            data[i] = T( alea.leq(m) );
        }
        
        permuter<T> perm( data );
        std::cerr << "data  = " << data << std::endl;
        std::cerr << "perm  = " << (counting &)perm << std::endl;
        std::cerr << "count = " << perm.count << "/" << countMax << std::endl;
        
        for( perm.boot(); perm.good(); perm.next() )
        {
        }
        std::cerr << "required nodes: " << perm.required_nodes() << std::endl;
        perm.trim();
        std::cerr << "              : " << perm.required_nodes() << std::endl;
        perm.boot();
        std::cerr << "              : " << perm.required_nodes() << std::endl;

        const double ratio = double(perm.required_nodes()) / perm.count;
        std::cerr << "ratio: " << ratio << std::endl;

    }


    template <typename T>
    void doPermMemory(const size_t n)
    {
        std::cerr << "-- dims=" << n << std::endl;
        integer_partition pb(n);
        std::cerr << "|_" << pb.outcomes() << " outcomes" << std::endl;
        pb.initialize();
        vector<T> data(n,as_capacity);
        vector<size_t> count;
        vector<size_t> nodes;
        do
        {
            data.free();
            const accessible<size_t> &part = pb;
            std::cerr << " |_using " << part << " => ";
            const size_t m = part.size();
            for(size_t j=1;j<=m;++j)
            {
                for(size_t k=part[j];k>0;--k)
                {
                    data.push_back( T(j) );
                }
            }
            Y_ASSERT(data.size()==n);
            std::cerr << data;
            permuter<T> P(data);
            Y_ASSERT(P.classes==part.size());
            std::cerr << " => #count=" << P.count;
            for( P.boot(); P.good(); P.next() )
            {

            }
            const size_t xnodes = P.required_nodes();
            std::cerr << " => #nodes= " << xnodes;
            std::cerr << std::endl;
            count.push_back(P.count);
            nodes.push_back(xnodes);
        }
        while( pb.build_next() );
        hsort(count, nodes, comparison::increasing<size_t> );

        const string fn = vformat("mperm%u.dat", unsigned(n));
        ios::ocstream fp(fn);
        permutation   p(n);
        const double den = double(p.count);
        for(size_t i=1;i<=count.size();++i)
        {
            fp("%g %g\n", double(count[i])/den, double(nodes[i])/den );
        }

    }
    
}

Y_UTEST(permuter)
{
    size_t n = 5;
    if(argc>1)
    {
        n = string_convert::to<size_t>(argv[1],"n");
    }
    
    size_t m=10;
    if(argc>2)
    {
        m = string_convert::to<size_t>(argv[2],"m");
    }

    doPerm<uint16_t>(n,m);
    doPerm<int16_t>(n,m);

    for(size_t i=1;i<=n;++i)
    {
        doPermMemory<int>(i);
    }


}
Y_UTEST_DONE()
