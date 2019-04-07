#include "y/mpl/natural.hpp"
#include "y/exception.hpp"

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

    MPN:: MPN() :
    plist(2,as_capacity),
    probe(5),
    _0(0), _1(1), _2(2), _3(3), _4(4), _5(5), _6(6), _10(10)
    {
        findProbe();
    }

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

#define __ADD_PRIME(X) do { const PrimeInfo p(X); prm.push_back(p); } while(false)

    void MPN:: findProbe()
    {
        initProbe();
        try
        {
            //------------------------------------------------------------------
            //
            // minimal list sanity check
            //
            //------------------------------------------------------------------
            {
                PrimeList &prm = (PrimeList &)plist;
                switch( plist.size() )
                {
                    case 0: __ADD_PRIME(_2); __ADD_PRIME(_3); break;
                    case 1: __ADD_PRIME(_3); break;
                    default:
                        assert( plist.size() >= 2 );
                        break;
                }
            }
            if(plist.size()<2) throw exception("MPN: invalid prime list");
            PrimeList::const_iterator i = plist.begin();
            if( (*i).p != _2 ) throw exception("MPN: invalid first prime");
            ++i;
            if( (*i).p != _3 ) throw exception("MPN: invalid second prime");

            //------------------------------------------------------------------
            //
            // starting point
            //
            //------------------------------------------------------------------
            const PrimeInfo &last_info = plist.back();
            mpn              p         = last_info.p;
            assert(probe.p==_5);

            if(p>=probe.p)
            {
                p -= _5;
                p /= _6;
                ++p;
                p *= _6;
                p += _5;
            }
            else
            {
                p = probe.p;
            }
            //std::cerr << "guess=" << p << std::endl;
            mpn q = mpn::square_of(p);
            //std::cerr << "gusqr=" << q << std::endl;
            ((mpn &)(probe.p)).xch(p);
            ((mpn &)(probe.q)).xch(q);
        }
        catch(...)
        {
            failSafe();
            throw;
        }
    }

    bool MPN:: isPrime_( const mpn &n ) const
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
