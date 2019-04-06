#include "y/mpl/natural.hpp"

namespace upsylon
{
    namespace mpl
    {
        manager:: manager() :
        IO()
        {
        }

        manager:: ~manager() throw()
        {
        }

        natural:: natural(const size_t nbit, randomized::bits &gen ) : Y_MPN_CTOR(0,0)
        {
            static const MPN &_ = MPN::instance();
            if(nbit)
            {
                natural tmp = _._1;
                for(size_t i=nbit;i>1;--i)
                {
                    tmp <<= 1;
                    if(gen.choice())
                    {
                        tmp |= _._1;
                    }
                }
                xch(tmp);
            }
        }

    }

    MPN:: MPN() : primes(), _probe(5),
    _0(0), _1(1), _2(2), _3(3), _4(4), _5(5), _6(6), _10(10)
    {}

    MPN:: ~MPN() throw()
    {
    }

    void MPN:: append_primes( const size_t count )
    {
        primeseq &prm = (primeseq &)primes;
        prm.reserve(count);
        mpn p = 1;
        if( prm.size() )
        {
            p = prm.back();
        }
        for(size_t i=0;i<count;++i)
        {
            p = mpn::next_prime(++p);
            prm.push_back(p);
        }
        
        find_probe();
    }

    void MPN:: load_primes( ios::istream &fp )
    {
        primeseq &prm = (primeseq &)primes;
        prm.free();
        const size_t count = fp.read<uint32_t>();
        prm.ensure(count);
        try
        {
            for(size_t i=0;i<count;++i)
            {
                mpn p = mpn::read(fp);
                prm.push_back(p);
            }
            find_probe();
        }
        catch(...)
        {
            prm.free();
            throw;
        }
    }
    
    void MPN:: find_probe()
    {
#if 0
        mpn &target = (mpn&)_probe;
        mpn  pmax   = _3;
        if( primes.size() > 0 )
        {
            const mpn &back = primes.back();
            if( back > pmax )
            {
                pmax = back;
            }
        }
        std::cerr << "pmax=" << pmax << std::endl;
        mpn p = _5;
        if(p<=pmax)
        {
            p.xch(pmax);
            p -= _5;
            p /= _6;
            ++p;
            p *= _6;
            p += _5;
        }
        std::cerr << "p=" << p << std::endl;

        target.xch(p);
#endif
    }

}
