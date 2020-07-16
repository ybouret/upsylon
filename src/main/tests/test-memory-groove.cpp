#include "y/memory/groove.hpp"
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
            Y_ASSERT(g.is_built_from( typeid(T) ));
            T       &first       = g.get<T>();
            const T &const_first = f.get<T>();

            for(size_t i=0;i<g.count;++i)
            {
                Y_ASSERT( aliasing::anonymous( & g.get<T>(i) ) );
                Y_ASSERT( aliasing::anonymous( & f.get<T>(i) ) );
            }

            g.free();
            Y_ASSERT(!g.is_built_from(typeid(T)));
            Y_ASSERT(g.count<=0);
            Y_ASSERT(g.label==0);
            Y_ASSERT( & g.get<T>() == &first );
            Y_ASSERT( & g.get<T>() == &const_first );
        }
    }

    template <typename T>
    static inline void doTest( memory::groove &g, const size_t n )
    {
        if(1==n)
        {
            std::cerr << "\tmake <" << type_name_of<T>() << ">[" << n << "]:";
            g.make<T>(memory::storage::shared);std::cerr << ' ' << g; doTestOf<T>(g);
            g.make<T>(memory::storage::pooled);std::cerr << ' ' << g; doTestOf<T>(g);
            g.make<T>(memory::storage::global);std::cerr << ' ' << g; doTestOf<T>(g);
            {
                const T tmp = support::get<T>();
                std::cerr << "\tmake <" << type_name_of<T>() << ">[" << n << "](" << tmp << "):";
                g.make<T,T>(memory::storage::shared,tmp); std::cerr << ' ' << g;doTestOf<T>(g);
                g.make<T,T>(memory::storage::pooled,tmp); std::cerr << ' ' << g;doTestOf<T>(g);
                g.make<T,T>(memory::storage::global,tmp); std::cerr << ' ' << g;doTestOf<T>(g);

                std::cerr << "\tcopy <" << type_name_of<T>() << ">[" << n << "](" << tmp << "):";
                g.copy<T>(memory::storage::shared,tmp); std::cerr << ' ' << g;doTestOf<T>(g);
                g.copy<T>(memory::storage::pooled,tmp); std::cerr << ' ' << g;doTestOf<T>(g);
                g.copy<T>(memory::storage::global,tmp); std::cerr << ' ' << g;doTestOf<T>(g);
            }
            std::cerr << std::endl;
        }

        std::cerr << "\tvmake<" << type_name_of<T>() << ">[" << n << "]:";
        g.vmake<T>(memory::storage::shared,n); std::cerr << ' ' << g; doTestOf<T>(g);
        g.vmake<T>(memory::storage::pooled,n); std::cerr << ' ' << g; doTestOf<T>(g);
        g.vmake<T>(memory::storage::global,n); std::cerr << ' ' << g; doTestOf<T>(g);
        std::cerr << std::endl;

        {
            const T tmp = support::get<T>();
            std::cerr << "\tvmake<" << type_name_of<T>() << ">[" << n << "](" << tmp << "):";

            g.vmake<T,T>(memory::storage::shared,n,tmp); std::cerr << ' ' << g;doTestOf<T>(g);
            g.vmake<T,T>(memory::storage::pooled,n,tmp); std::cerr << ' ' << g;doTestOf<T>(g);
            g.vmake<T,T>(memory::storage::global,n,tmp); std::cerr << ' ' << g;doTestOf<T>(g);

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
        std::cerr << "Testing groove_of" << std::endl;
        memory::groove g;
        {
            g.vmake<float>(memory::storage::pooled,7);
            std::cerr << "g=" << g << std::endl;

            memory::groove_of<float> G(g);
            std::cerr << "size=" << G.size() << std::endl;
            std::cerr << G << std::endl;
            support::fill1D(G);
            std::cerr << G << std::endl;
        }

        {
            g.vmake<string>(memory::storage::pooled,3);
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




}
Y_UTEST_DONE()
