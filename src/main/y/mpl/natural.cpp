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

    MPN:: PrimeInfo:: ~PrimeInfo() throw()
    {
    }

    std::ostream & operator<<( std::ostream &os, const MPN::PrimeInfo &_ )
    {
        os << _.p;
        return os;
    }

    MPN:: PrimeInfo:: PrimeInfo( const PrimeInfo &other ) :
    p( other.p ),
    q( other.q )
    {
    }

    MPN:: PrimeInfo:: PrimeInfo( const mpn &n ) :
    p( n ),
    q( mpn::square_of(n) )
    {
    }
    


#define MPN_SMALLEST_PROBE 5

    static const uint8_t probe_p = MPN_SMALLEST_PROBE;
    static const uint8_t probe_q = MPN_SMALLEST_PROBE*MPN_SMALLEST_PROBE;

    MPN:: MPN() : plist(),
    probe(5),
    _0(0), _1(1), _2(2), _3(3), _4(4), _5(5), _6(6), _10(10)
    {}

    MPN:: ~MPN() throw()
    {
    }

    void MPN:: initProbe() throw()
    {
        ((mpn &)(probe.p)).set_byte(probe_p);
        ((mpn &)(probe.q)).set_byte(probe_q);
    }

    void MPN:: failSafe() throw()
    {
        PrimeList &prm = (PrimeList &)plist;
        prm.free();
        initProbe();
    }

    void MPN:: createPrimes( const size_t count )
    {
        PrimeList &prm = (PrimeList &)plist;
        prm.reserve(count);
        try
        {
            mpn p = 1;
            if( prm.size() )
            {
                p = prm.back().p;
            }
            for(size_t i=0;i<count;++i)
            {
                p = mpn::next_prime(++p);
                prm.push_back(p);
            }
            findProbe();

        }
        catch(...)
        {
            failSafe();
            throw;
        }

    }

    void MPN:: reloadPrimes( ios::istream &fp )
    {
        failSafe();
        PrimeList &prm = (PrimeList &)plist;
        try
        {
            const size_t count = fp.read<uint32_t>();
            prm.ensure(count);
            for(size_t i=0;i<count;++i)
            {
                mpn p = mpn::read(fp);
                prm.push_back(p);
            }
            findProbe();
        }
        catch(...)
        {
            failSafe();
            throw;
        }
    }
    
    void MPN:: findProbe()
    {
        initProbe();
    }

    bool MPN:: isPrime( const mpn &n ) const
    {
        if(n<=_1)
            return false; //!< for 0 and 1
        else if(n<=_3)
            return true;  //!< for 2 and 3
        else if( n.is_divisible_by_byte(2) || n.is_divisible_by_byte(3) ) //! for 2,4,6,8,9...
            return false;
        else
        {
            assert(n>=_5);
            mpn i = _5;
            for(;;)
            {
                const mpn isq = mpn::square_of(i);
                if(isq>n) break;
                if( n.is_divisible_by(i) )  return false;
                const mpn j=i+_2;
                if( n.is_divisible_by(j) )  return false;
                i +=_6;
            }
            return true;
        }
    }

}
