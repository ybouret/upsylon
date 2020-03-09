#include "y/sequence/list.hpp"
#include "y/sequence/vector.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>
#include "y/memory/pooled.hpp"

using namespace upsylon;

namespace
{

    template <typename SEQ>
    static inline void scan( SEQ &seq )
    {
        std::cerr << "scanning #" << seq.size() << std::endl;
        std::cerr << "forward:";
        size_t j = 0;
        for( typename SEQ::iterator i=seq.begin(); i != seq.end(); ++i )
        {
            ++j;
            std::cerr << '[' << j << ']' << '=' << *i << '/';
        }
        std::cerr << "." << std::endl;

        std::cerr << "reverse:";
        j = seq.size();
        for( typename SEQ::reverse_iterator i=seq.rbegin(); i != seq.rend(); ++i )
        {
            std::cerr << '[' << j << ']' << '=' << *i << '/';
            --j;
        }
        std::cerr << "." << std::endl;
        std::cerr << "test access..." << std::endl;
        for(size_t i=seq.size();i>0;--i)
        {
            (void) seq[i];
        }
    }

    template <typename SEQ>
    static inline void scan_const( const SEQ &seq )
    {
        std::cerr << "scanning #" << seq.size() << "/const" << std::endl;
        size_t j=0;
        for( typename SEQ::const_iterator i=seq.begin(); i != seq.end(); ++i )
        {
            ++j;
            std::cerr << '[' << j << ']' << '=' << *i << '/';
        }
        std::cerr << "." << std::endl;

        std::cerr << "reverse:";
        j = seq.size();
        for( typename SEQ::const_reverse_iterator i=seq.rbegin(); i != seq.rend(); ++i )
        {
            std::cerr << '[' << j << ']' << '=' << *i << '/';
            --j;
        }
        std::cerr << "." << std::endl;
        std::cerr << "test access..." << std::endl;
        for(size_t i=seq.size();i>0;--i)
        {
            (void) seq[i];
        }
        std::cerr << "done" << std::endl;
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

            {
                SEQ seq1;
                SEQ seq2(seq);
                seq1 = seq;
            }

            seq.reserve( alea.leq(100) );
            std::cerr << "seq.size     = " << seq.size() << std::endl;
            std::cerr << "seq.capacity = " << seq.capacity() << std::endl;
            {
                const ptrdiff_t sz       = seq.size()/4;
                const size_t    new_size = seq.size() + alea.range<ptrdiff_t>(-sz,sz);
                std::cerr << "seq.new_size=" << new_size << std::endl;
                typename SEQ::const_type pad = support::get<typename SEQ::mutable_type>();
                seq.adjust(new_size,pad);
                Y_ASSERT(seq.size()==new_size);
            }
            scan(seq);
            scan_const(seq);
            while(seq.size())
            {
                (void)seq.front();
                (void)seq.back();
                if( alea.choice() )
                {
                    seq.pop_back();
                }
                else
                {
                    seq.pop_front();
                }
            }
        }

        {
            SEQ seq;
            for(size_t i=1+alea.leq(20);i>0;--i)
            {
                typename SEQ::const_type args = support::get<typename SEQ::mutable_type>();
                seq.push_back(args);
            }
            std::cerr << "org=" << seq << std::endl;
            seq.reverse();
            std::cerr << "rev=" << seq << std::endl;
        }

        {
            SEQ seq1;
            for(size_t i=1+alea.leq(20);i>0;--i)
            {
                typename SEQ::const_type args = support::get<typename SEQ::mutable_type>();
                seq1.push_back(args);
            }
            {
                vector<typename SEQ::mutable_type> seq2;
                for(size_t i=1+alea.leq(20);i>0;--i)
                {
                    typename SEQ::const_type args = support::get<typename SEQ::mutable_type>();
                    seq2.push_back(args);
                }
                const size_t numObjects = seq2.size();
                Y_CHECK( numObjects == seq1.put( *seq2, seq2.size() ) );
            }
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


#include "y/os/confirm.hpp"

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
    
    return 0;

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

    std::cerr << "sizeof(ligthweight_array)=" << sizeof(lightweight_array<int>) << std::endl;
}
Y_UTEST_DONE()

