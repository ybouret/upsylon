#include "y/sequence/list.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>

using namespace upsylon;

namespace
{
    template <typename SEQ>
    static inline void do_test()
    {
        std::cerr << typeid(SEQ).name() << std::endl;
        { SEQ seq0; }
        { SEQ seqn(alea.leq(100),as_capacity); }
        {
            SEQ seq;
            for(size_t i=1+alea.leq(1000);i>0;--i)
            {
                typename SEQ::const_type args = support::get<typename SEQ::mutable_type>();
                if(alea.choice())
                {
                    seq.push_back(args);
                }
                else
                {
                    seq.push_front(args);
                }
            }
            seq.reserve( alea.leq(32) );
            std::cerr << "seq.size     = " << seq.size() << std::endl;
            std::cerr << "seq.capacity = " << seq.capacity() << std::endl;

        }
    }

    static inline void do_capa(size_t n)
    {
        std::cerr << n;
        do
        {
            n = container::estimate_next_capacity(n);
            std::cerr << "/" << n ;
        } while(n<16000000);
        std::cerr << std::endl;
    }
}



Y_UTEST(sequence)
{

    do_capa(0);
    for(size_t iter=0;iter<10;++iter)
    {
        do_capa( alea.leq(1000) );
    }
    do_test< list<int> >();
    do_test< list<string> >();
}
Y_UTEST_DONE()

