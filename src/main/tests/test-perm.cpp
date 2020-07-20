
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
#include "y/ios/ovstream.hpp"
#include "y/ios/imstream.hpp"
#include "y/container/task.hpp"

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
        Y_ASSERT( data.size() == perm.size() );
        Y_ASSERT( data.size() == perm.dims );
        Y_ASSERT( data.size() == perm.current.size() );

        std::cerr << "data  = " << data << std::endl;
        std::cerr << "perm  = " << (counting &)perm << std::endl;
        std::cerr << "curr  = " << perm.current     << std::endl;
        std::cerr << "count = " << perm.count << "/" << countMax << std::endl;

        perm.next();
        std::cerr << "perm1 = " << (counting &)perm << std::endl;
        std::cerr << "curr1 = " << perm.current     << std::endl;
        perm.unwind();

        std::cerr << "required nodes: " << perm.store.required() << std::endl;
        perm.store.free_cache();
        std::cerr << "              : " << perm.store.required() << std::endl;
        perm.boot();
        std::cerr << "              : " << perm.store.required() << std::endl;

        const double ratio = double(perm.store.required()) / perm.count;
        std::cerr << "ratio: " << ratio << std::endl;
        std::cerr << "check copy consistency" << std::endl;

        if(n<=6)
        {
            for(size_t steps=1;steps<=perm.count;++steps)
            {
                perm.boot();
                for(size_t i=0;i<steps;++i)
                {
                    assert(perm.good());
                    perm.next();
                }
                permuter<T> temp(perm);
                Y_ASSERT(temp.has_same_state_than(perm));
                while( perm.good() )
                {
                    Y_ASSERT(temp.good());
                    Y_ASSERT(temp.has_same_state_than(perm));
                    perm.next();
                    temp.next();
                }
            }
        }
        else
        {
            std::cerr << "[";
            for(size_t iter=0;iter<4;++iter)
            {
                std::cerr << ".";
                std::cerr.flush();
                perm.boot();
                for(size_t i=alea.leq(perm.count);i>0;--i)
                {
                    perm.next();
                }
                permuter<T> temp(perm);
                Y_ASSERT(temp.has_same_state_than(perm));
                while( perm.good() )
                {
                    Y_ASSERT( temp.good() );
                    Y_ASSERT(temp.has_same_state_than(perm));
                    perm.next();
                    temp.next();
                }
            }
            std::cerr << "]" << std::endl;
        }

        std::cerr << "checking I/O" << std::endl;
        {
            ios::ovstream dest;
            permuter<T>   temp(data);
            for( perm.boot(); perm.good(); perm.next() )
            {
                dest.free();
                const size_t nw = perm.save(dest);
                ios::imstream from(dest);
                const size_t nr = temp.load(from);
                Y_ASSERT(nw==nr);
                Y_ASSERT(temp.has_same_state_than(perm));
            }
        }

        std::cerr << "checking frames" << std::endl;
        {
            vector<size_t> frames;
            for(perm.boot(); perm.good(); perm.next())
            {
                frames.put( &perm.current[1], perm.size() );
            }
            Y_CHECK( frames.size() == perm.count * perm.dims );

            const size_t *q = *frames;
            for(perm.boot(); perm.good(); perm.next())
            {
                for(size_t i=1;i<=perm.dims;++i)
                {
                    Y_ASSERT(perm.current[i]==*(q++));
                }
            }

        }


        std::cerr << std::endl;
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
            P.unwind();
            const size_t xnodes = P.store.required();
            std::cerr << " => #nodes="   << xnodes;
            std::cerr << " => #created=" << P.store.created;
            std::cerr << std::endl;
            count.push_back(P.count);
            nodes.push_back(xnodes);
        }
        while( pb.build_next() );
        hsort(count, nodes, comparison::increasing<size_t> );

        const string  fn = vformat("mperm%u.dat", unsigned(n));
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
