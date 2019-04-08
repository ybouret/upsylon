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
}

#include "y/ios/null-ostream.hpp"
namespace upsylon
{

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
    


#define MPN_INIT_PROBE 5

    static const uint8_t probe_p = MPN_INIT_PROBE;
    static const uint8_t probe_q = MPN_INIT_PROBE*MPN_INIT_PROBE;

    MPN:: MPN() :
    plist(2,as_capacity),
    probe(5),
    _0(0), _1(1), _2(2), _3(3), _4(4), _5(5), _6(6), _10(10)
    {
        PrimeList &prm = (PrimeList &)plist;
        { const PrimeInfo __2(_2); prm.push_back(__2); }
        { const PrimeInfo __3(_3); prm.push_back(__3); }
    }

    MPN:: ~MPN() throw()
    {
    }

    void MPN:: initProbe() throw()
    {
        PrimeList &prm = (PrimeList &)plist;
        assert(prm.size()>=2);
        while(prm.size()>2) prm.pop_back();
        ((mpn &)(probe.p)).set_byte(probe_p);
        ((mpn &)(probe.q)).set_byte(probe_q);
    }


    void MPN:: checkList() const
    {
        if( plist.size()<2 ) throw exception("MPN: not enough primes!");
        const mpn & last_prime = plist.back().p;
        const mpn & probe_p    = probe.p;
        if( probe_p <= last_prime ) throw exception("MPN: invalid probe/last prime");
        if( probe_p < _5 )          throw exception("MPN: invalid probe/5");
        const mpn probe_minus_5 = probe_p-_5;
        if( !probe_minus_5.is_divisible_by(_6) ) throw exception("MPN: invalid probe scale");
    }

    size_t MPN:: nextProbe()
    {
        checkList();
        PrimeList    &prm        = (PrimeList&)plist;
        const size_t  org        = prm.size();

        try
        {
            //__________________________________________________________________
            //
            // initialize search
            //__________________________________________________________________
            mpn       next_probe = probe.p + _6;
            PrimeList added;
            mpn       last_prime = prm.back().p;

            //__________________________________________________________________
            //
            // loop
            //__________________________________________________________________
            while(true)
            {
                last_prime += _2;
                if(! isPrime_(last_prime)) continue;
                if(last_prime>=next_probe) break;
                const PrimeInfo tmp(last_prime);
                added.push_back(tmp);
            }

            //__________________________________________________________________
            //
            // update, added may be 0
            //__________________________________________________________________
            mpn next_prsqr = mpn::square_of(next_probe);

            ((mpn &)(probe.p)).xch(next_probe);
            ((mpn &)(probe.q)).xch(next_prsqr);
            const size_t ans = added.size();
            prm.merge_back(added);
            return ans;

        }
        catch(...)
        {
            while(prm.size()>org) prm.pop_back();
            throw;
        }

    }

    void MPN:: createPrimes( const size_t count )
    {
        size_t sum = 0;
        while(sum<count) sum += nextProbe();
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



#define MPN_NEXT_PRIME(METHOD) \
mpn p=n; if(p.is_even()) ++p; assert(p.is_odd()); while( !METHOD(p) ) p += _2; return p

    mpn MPN:: nextPrime_(const mpn &n) const
    {
        MPN_NEXT_PRIME(isPrime_);
    }

    mpn MPN:: nextPrime(const mpn &n) const
    {
        MPN_NEXT_PRIME(isPrime);
    }


    bool MPN:: isPrime( const mpn &n) const
    {
        if(n<=_1)
            return false; //!< for 0 and 1
        else if(n<=_3)
            return true;  //!< for 2 and 3
        else
        {
            //--- list usage
            {
                size_t                    np  = plist.size();
                PrimeList::const_iterator it  = plist.begin();
                while(np-->0)
                {
                    const PrimeInfo &I = *it;
                    if(I.q>n)                  return true;
                    if(n.is_divisible_by(I.p)) return false;
                    ++it;
                }
            }

            //--- fallback
            mpn i = probe.p;
            for(;;)
            {
                const mpn isq = mpn::square_of(i);
                if(isq>n)
                {
                    break;
                }
                if( n.is_divisible_by(i) )  return false;
                const mpn j=i+_2;
                if( n.is_divisible_by(j) )  return false;
                i +=_6;
            }
            return true;
        }
    }


    size_t MPN:: recordPrimes(ios::ostream &fp) const
    {
        Hasher H;
        H.set();
        assert(plist.size()>=2);
        size_t len = 0;
        {
            size_t shift = 0;
            fp.emit_upack<size_t>(plist.size()-2,&shift);
            len += shift;
        }
        PrimeList::const_iterator i=plist.begin();
        assert(_2==(*i).p); ++i; //!< skip 2
        assert(_3==(*i).p); ++i; //!< skip 3
        mpn last = _3;
        for(;i!=plist.end();++i)
        {
            const mpn curr = (*i).p;
            mpn       code = curr-last; assert(code.is_even());
            --(code.shr());
            len += code.save(fp);
            last = curr;
            H(code);
        }
        len += probe.p.save(fp);
        H(probe.p);
        const digest md = H.md();
        return len+md.save(fp);
    }

    size_t MPN:: recordLength() const
    {
        ios::null_ostream nil;
        return recordPrimes(nil);
    }



}
