#include "y/sequence/list.hpp"
#include "y/sequence/vector.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>

using namespace upsylon;

namespace
{

    template <typename SEQ>
    static inline void scan( SEQ &seq )
    {
        std::cerr << "scanning #" << seq.size() << std::endl;
        std::cerr << "forward:";
        for( typename SEQ::iterator i=seq.begin(); i != seq.end(); ++i )
        {
            std::cerr << *i << "/";
        }
        std::cerr << "." << std::endl;

        std::cerr << "reverse:";
        for( typename SEQ::reverse_iterator i=seq.rbegin(); i != seq.rend(); ++i )
        {
            std::cerr << *i << "/";
        }
        std::cerr << "." << std::endl;
    }

    template <typename SEQ>
    static inline void scan_const( const SEQ &seq )
    {
        std::cerr << "scanning #" << seq.size() << "/const" << std::endl;
        for( typename SEQ::const_iterator i=seq.begin(); i != seq.end(); ++i )
        {
            std::cerr << *i << "/";
        }
        std::cerr << "." << std::endl;

        std::cerr << "reverse:";
        for( typename SEQ::const_reverse_iterator i=seq.rbegin(); i != seq.rend(); ++i )
        {
            std::cerr << *i << "/";
        }
        std::cerr << "." << std::endl;
    }

    template <typename SEQ>
    static inline void do_test()
    {
        std::cerr << std::endl;
        std::cerr << typeid(SEQ).name() << std::endl;
        { SEQ seq0; scan(seq0);scan_const(seq0); }
        { SEQ seqn(alea.leq(100),as_capacity); }
        {
            SEQ seq;
            for(size_t i=1+alea.leq(100);i>0;--i)
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
            scan(seq);
            scan_const(seq);
        }
    }

    static inline void do_capa(size_t n)
    {
        std::cerr << n;
        do
        {
            n = container::next_capacity(n);
            std::cerr << "/" << n ;
        } while(n<16000000);
        std::cerr << std::endl;
    }
}



Y_UTEST(sequence)
{
    std::cerr << "-- next_capacity" << std::endl;
    do_capa(0);
    for(size_t iter=0;iter<10;++iter)
    {
        do_capa( alea.leq(1000) );
    }

    do_test< list<uint16_t> >();
    do_test< list<string> >();

    {
        vector<uint16_t,memory::global> cvg(1,as_capacity); std::cerr << "cvg.capacity=" << cvg.capacity() << std::endl;
        vector<uint16_t,memory::pooled> cvp(1,as_capacity); std::cerr << "cvp.capacity=" << cvp.capacity() << std::endl;
        vector<string> vs1(10,as_capacity);
        vector<string,memory::pooled> vs2(12);
        cvg.reserve(10);
        cvp.reserve(10);
        vs1.reserve(10);
        vs2.reserve(10);
    }

    do_test< vector<uint16_t> >();
    do_test< vector<string>   >();
    do_test< vector<uint16_t,memory::pooled> >();
    do_test< vector<string,memory::pooled>   >();

}
Y_UTEST_DONE()

