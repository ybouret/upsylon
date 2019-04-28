#include "y/sequence/pipe.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/memory/pooled.hpp"

using namespace upsylon;

namespace
{

    template <typename PIPE>
    void run_test( PIPE & p, const char *cls, const char *tid )
    {
        std::cerr << cls << "<" << tid << ">" << std::endl;
        std::cerr << p << std::endl;
        for(size_t i=1+alea.leq(10);i>0;--i)
        {
            typename PIPE::const_type tmp = support::get<typename PIPE::type>();
            p.push(tmp);
            std::cerr << p << "/peek=" << p.peek() << std::endl;
        }
        while(p.size()>1)
        {
            p.pop();
            std::cerr << p << "/peek=" << p.peek() << std::endl;
        }
        p.pop();
        std::cerr << p << std::endl;

    }

    template <typename T>
    void do_test(const char *tid)
    {
        queue<T>                 Q;  run_test( Q,  "queue", tid);
        vstack<T,memory::global> gS; run_test( gS, "global stack", tid);
        vstack<T,memory::pooled> pS; run_test( pS, "pooled stack", tid);
        lstack<T>                lS; run_test( lS, "stacked list", tid);
    }
}

#define DO_TEST(CLASS) do_test<CLASS>( #CLASS )
Y_UTEST(pipe)
{
    DO_TEST(int);
    DO_TEST(double);
    DO_TEST(string);


}
Y_UTEST_DONE()

