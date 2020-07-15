#include "y/memory/grooves.hpp"
#include "y/memory/groove-of.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/type/spec.hpp"
#include "support.hpp"

using namespace upsylon;

namespace {

    template <typename T>
    static inline void doTestOf( memory::groove &g )
    {
        const memory::groove &f = g;
        if(g.count>0)
        {
            T       &first       = g.as<T>();
            const T &const_first = f.as<T>();

            for(size_t i=0;i<g.count;++i)
            {
                Y_ASSERT( aliasing::anonymous( & g.as<T>(i) ) == aliasing::anonymous( & g.get<T>(i) ) );
                Y_ASSERT( aliasing::anonymous( & f.as<T>(i) ) == aliasing::anonymous( & f.get<T>(i) ) );

            }

            g.free();
            Y_ASSERT(g.count<=0);
            Y_ASSERT(g.label==0);
            Y_ASSERT( & g.get<T>() == &first );
            Y_ASSERT( & g.get<T>() == &const_first );
        }
    }

    template <typename T>
    static inline void doTest( memory::groove &g, const size_t n )
    {
        std::cerr << "\tmake<" << type_name_of<T>() << ">[" << n << "]:";
        g.make<T>(memory::storage::shared,n); std::cerr << ' ' << g; doTestOf<T>(g);
        g.make<T>(memory::storage::pooled,n); std::cerr << ' ' << g; doTestOf<T>(g);
        g.make<T>(memory::storage::global,n); std::cerr << ' ' << g; doTestOf<T>(g);
        std::cerr << std::endl;

        {
            const T tmp = support::get<T>();
            std::cerr << "\tbuild<" << type_name_of<T>() << ">[" << n << "](" << tmp << "):";

            g.build<T,T>(memory::storage::shared,n,tmp); std::cerr << ' ' << g;doTestOf<T>(g);
            g.build<T,T>(memory::storage::pooled,n,tmp); std::cerr << ' ' << g;doTestOf<T>(g);
            g.build<T,T>(memory::storage::global,n,tmp); std::cerr << ' ' << g;doTestOf<T>(g);

            std::cerr << std::endl;
        }



    }

    static inline void doTests( memory::groove &g, const size_t n )
    {
        std::cerr << "test with #" << n << std::endl;
        doTest<double>(g,n);
        doTest<int32_t>(g,n);
        doTest<string>(g,n);
        doTest<mpn>(g,n);
    }

    static inline memory::storage::model alea_storage()
    {
        static const memory::storage::model m[3] = { memory::storage::shared, memory::storage::pooled, memory::storage::global };
        return m[ alea.leq(2) ];
    }

    static inline void doTest(memory::grooves                   &G,
                              const size_t                 n,
                              const memory::storage::model m)
    {
        G.release();
        G.update(m);
        G.make(n);
        for(size_t i=1;i<=n;++i)
        {
            memory::groove &g = G[i];
            Y_ASSERT(g.bytes<=0);
            const size_t    j = alea.leq(8);
            switch( alea.range(1,4) )
            {
                case 1: g.make<int32_t>( alea_storage(), j); break;
                case 2: g.make<double>(  alea_storage(), j); break;
                case 3: g.make<string>(  alea_storage(), j); break;
                case 4: g.make<mpn>(     alea_storage(), j); break;
                default:
                    break;
            }
        }
        std::cerr << ' ' << G;
    }

    static inline void doTests( memory::grooves &G )
    {
        for(size_t n=0;n<=4;++n)
        {
            std::cerr << "grooves[" << n << "]=";
            doTest(G,n,memory::storage::shared);
            doTest(G,n,memory::storage::pooled);
            doTest(G,n,memory::storage::global);
            std::cerr << std::endl;
        }

    }

}

Y_UTEST(groove)
{
    std::cerr << "storage:" << std::endl;
    std::cerr << '\t' << memory::storage::text(memory::storage::unused) << std::endl;
    std::cerr << '\t' << memory::storage::text(memory::storage::shared) << std::endl;
    std::cerr << '\t' << memory::storage::text(memory::storage::pooled) << std::endl;
    std::cerr << '\t' << memory::storage::text(memory::storage::global) << std::endl;
    std::cerr << std::endl;
    
    Y_UTEST_SIZEOF(memory::groove);

    {
        memory::groove g; std::cerr << g << std::endl;

        for(size_t n=1;n<=32;++n)
        {
            std::cerr << '\t';
            g.acquire(memory::storage::shared,n); std::cerr << g << ' ';
            g.acquire(memory::storage::pooled,n); std::cerr << g << ' ';
            g.acquire(memory::storage::global,n); std::cerr << g << ' ';
            std::cerr << std::endl;
        }

        g.release(); std::cerr << g << std::endl;
        std::cerr << std::endl;

        for(size_t n=0;n<=32;++n)
        {
            doTests(g,n);
        }

        std::cerr << std::endl;

        std::cerr << "Test copy..." << std::endl;
        g.copy<float>(memory::storage::shared,1.0f);
        std::cerr << g << std::endl;
        std::cerr << std::endl;
    }

    {
        std::cerr << "Testing grooves..." << std::endl;
        memory::grooves G;
        doTests(G);
        std::cerr << std::endl;
    }

    {
        std::cerr << "Testing groove_of" << std::endl;
        memory::groove g;
        {
            g.make<float>(memory::storage::pooled,7);
            std::cerr << "g=" << g << std::endl;

            memory::groove_of<float> G(g);
            std::cerr << "size=" << G.size() << std::endl;
            std::cerr << G << std::endl;
            support::fill1D(G);
            std::cerr << G << std::endl;
        }

        {
            g.make<string>(memory::storage::pooled,3);
            std::cerr << "g=" << g << std::endl;

            memory::groove_of<string> G(g);
            std::cerr << "size=" << G.size() << std::endl;
            std::cerr << G << std::endl;
            support::fill1D(G);
            std::cerr << G << std::endl;
        }

        {
            memory::groove_of<mpn> G(g,memory::storage::global,9);
            std::cerr << "size=" << G.size() << std::endl;
            std::cerr << G << std::endl;
            support::fill1D(G);
            std::cerr << G << std::endl;
        }

        std::cerr << std::endl;
    }

    {
        std::cerr << "Testing as simple cache..." << std::endl;
        memory::grooves cache(memory::storage::pooled);
        cache.make(1);
        memory::groove_of<float> arr( *cache, memory::storage::pooled, 6 );
        support::fill1D(arr);
        std::cerr << "arr=" << arr << std::endl;
        std::cerr << "handler.bytes=" << cache.handler().bytes << std::endl;
        std::cerr << "handler.model=" << cache.handler().model_text() << std::endl;
    }


}
Y_UTEST_DONE()

