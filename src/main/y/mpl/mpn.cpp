#include "y/mpl/mpn.hpp"
#include "y/exception.hpp"

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
    q( mpn::square_of(p) )
    {
    }

    MPN:: PrimeInfo:: PrimeInfo( const uint64_t n) :
    p( n ),
    q( mpn::square_of(p) )
    {
    }

}

namespace upsylon
{
    MPN:: ListOfPrimeInfo::  ListOfPrimeInfo() throw()       : PrimeInfo::List()              {}
    MPN:: ListOfPrimeInfo::  ListOfPrimeInfo(const size_t n) : PrimeInfo::List(n,as_capacity) {}
    MPN:: ListOfPrimeInfo:: ~ListOfPrimeInfo() throw() {}

    const mpn & MPN:: ListOfPrimeInfo:: lower() const throw()
    {
        assert( size() >= 2 );
        return front().p;
    }

    const mpn & MPN:: ListOfPrimeInfo:: upper() const throw()
    {
        assert( size() >= 2 );
        return back().p;
    }


}

namespace upsylon
{
    MPN:: MetaPrimeVector:: ~MetaPrimeVector() throw()
    {
        mpl::manager::location().release_as<Slot>(++slot,capacity,bytes);
    }

    MPN:: MetaPrimeVector:: MetaPrimeVector(const size_t n) :
    slot(0),
    size(0),
    capacity(max_of<size_t>(n,2)),
    bytes(0)
    {
        slot = mpl::manager::instance().acquire_as<Slot>(capacity,bytes);
        --slot;
    }

    void MPN:: MetaPrimeVector:: reserve(size_t n)
    {
        if(n>1)
        {
            mpl::manager &mgr          = mpl::manager::location();
            size_t        new_capacity = capacity+n;
            size_t        new_bytes    = 0;
            Slot         *new_slot     = mgr.acquire_as<Slot>(new_capacity, new_bytes);
            ++slot;
            memcpy( new_slot,slot,sizeof(Slot)*size);
            mgr.release_as(slot,capacity,bytes);
            --new_slot;
            slot     = new_slot;
            capacity = new_capacity;
            bytes    = new_bytes;
        }
    }


    void MPN:: MetaPrimeVector:: ensure( const size_t n)
    {
        if(n>capacity)
        {
            reserve(n-capacity);
            assert(capacity>=n);
        }
    }

    void MPN:: MetaPrimeVector:: record( const mpn &prime_ref )
    {
        if(size>=capacity)
        {
            reserve( container::next_increase(capacity) );
        }
        assert(size<capacity);
        slot[++size] = &prime_ref;
    }

}

namespace upsylon
{


#define MPN_INIT_PROBE 5

    static const uint8_t probe_p = MPN_INIT_PROBE;

    MPN:: MPN() :
    plist(2),
    mpvec(2),
    probe(5),
    _0(), _1(1), _2(2), _3(3), _4(4), _5(5), _6(6), _10(10)
    {
        ListOfPrimeInfo &prm = (ListOfPrimeInfo &)plist;
        MetaPrimeVector &mpv = (MetaPrimeVector &)mpvec;
        { const PrimeInfo __2(_2); prm.push_back(__2); mpv.record(prm.back().p); }
        { const PrimeInfo __3(_3); prm.push_back(__3); mpv.record(prm.back().p); }
    }

    MPN:: ~MPN() throw()
    {
    }



    void MPN:: reset() throw()
    {
        assert(plist.size()==mpvec.size);


        //----------------------------------------------------------------------
        //
        // reset list
        //
        //----------------------------------------------------------------------
        {
            ListOfPrimeInfo &prm = (ListOfPrimeInfo &)plist;
            assert(prm.size()>=2);
            while(prm.size()>2) prm.pop_back();
        }

        //----------------------------------------------------------------------
        //
        // reset probe
        //
        //----------------------------------------------------------------------
        ((mpn &)(probe)).set_byte(probe_p);

        //----------------------------------------------------------------------
        //
        // reset vector
        //
        //----------------------------------------------------------------------
        {
            MetaPrimeVector &mpv = (MetaPrimeVector &)mpvec;
            memset( & mpv.slot[1], 0, mpv.bytes );
            mpv.size = 2;
            mpv.slot[1] = & plist.lower();
            mpv.slot[2] = & plist.upper();
        }

    }

    

    size_t MPN:: nextProbe()
    {
        assert(plist.size()>=2);
        assert(plist.size()==mpvec.size);
        assert(probe>plist.back().p);
        assert(probe>=_5);

        ListOfPrimeInfo &prm = (ListOfPrimeInfo &)plist;
        MetaPrimeVector &mpv = (MetaPrimeVector &)mpvec;
        const size_t     org = prm.size();

        try
        {
            //__________________________________________________________________
            //
            // initialize search
            //__________________________________________________________________
            mpn              next_probe = probe + _6;
            ListOfPrimeInfo  added;
            mpn              last_prime = prm.back().p;

            //__________________________________________________________________
            //
            // loop
            //__________________________________________________________________
            while(true)
            {
                last_prime += _2;
                if(! isPrime(last_prime) ) continue;
                if(last_prime>=next_probe) break;
                const PrimeInfo tmp(last_prime);
                added.push_back(tmp);
                mpv.record(added.back().p);
            }

            //__________________________________________________________________
            //
            // update, added may be 0
            //__________________________________________________________________

            ((mpn &)(probe)).xch(next_probe);
            const size_t ans = added.size();
            prm.merge_back(added);
            return ans;

        }
        catch(...)
        {
            while(prm.size()>org) prm.pop_back();
            mpv.size = org;
            throw;
        }

    }

    void MPN:: createPrimes( const size_t count)
    {
        size_t sum = 0;
        ((MetaPrimeVector &)mpvec).reserve(count);
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
if(n<=_2) return _2; else { \
mpn p=n; if(p.is_even()) ++p; assert(p.is_odd()); while( !METHOD(p) ) p += _2; return p; \
}

    mpn MPN:: nextPrime_(const mpn &n) const
    {
        MPN_NEXT_PRIME(isPrime_)
    }

    mpn MPN:: nextPrime(const mpn &n) const
    {
        MPN_NEXT_PRIME(isPrime)
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
                size_t                          np  = plist.size();
                ListOfPrimeInfo::const_iterator it  = plist.begin();
                while(np-->0)
                {
                    const PrimeInfo &I = *it;
                    if(I.q>n)                  return true;
                    if(n.is_divisible_by(I.p)) return false;
                    ++it;
                }
            }

            //--- fallback
            mpn i = probe;
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

}

#include "y/ios/ostream.hpp"
namespace upsylon
{
    const char   MPN:: CLASS_NAME[] = "MPN";

    const char * MPN:: className() const throw()
    {
        return CLASS_NAME;
    }

    size_t MPN:: serialize(ios::ostream &fp) const
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
        ListOfPrimeInfo::const_iterator i=plist.begin();
        assert(_2==(*i).p); ++i; //!< skip 2
        assert(_3==(*i).p); ++i; //!< skip 3
        mpn last = _3;
        for(;i!=plist.end();++i)
        {
            const mpn curr = (*i).p;
            mpn       code = curr-last; assert(code.is_even());
            --(code.shr());
            len += code.serialize(fp);
            last = curr;
            H(code);
        }
        len += probe.serialize(fp);
        H(probe);
        const digest md = H.md();
        return len+md.serialize(fp);
    }



    void MPN:: reloadPrimes( ios::istream &fp)
    {
        reset();
        try
        {
            // list
            {
                ListOfPrimeInfo &prm = (ListOfPrimeInfo &)plist;
                const size_t     sz  = fp.read_upack<size_t>();
                Hasher           H;  H.set();

                // loop
                mpn curr = _3;
                for(size_t i=0;i<sz;++i)
                {
                    mpn code = mpn::read(fp);
                    H(code);
                    curr += ( (++code).shl() );
                    const PrimeInfo tmp(curr);
                    prm.push_back(tmp);
                }
                // probe
                curr = mpn::read(fp);
                ( (mpn&) probe ).xch(curr);
                H(probe);

                // check
                const digest md0 = digest::load(fp);
                const digest md1 = H.md();
                if(md0!=md1) throw exception("MPN.reload(currupted data)");
            }

            // vector
            MetaPrimeVector &mpv = (MetaPrimeVector &)mpvec;
            mpv.ensure( plist.size() );
            for(ListOfPrimeInfo::const_iterator i=plist.begin();i!=plist.end();++i)
            {
                mpv.record( i->p );
            }

        }
        catch(...)
        {
            reset();
            throw;
        }
    }

    digest MPN:: md() const
    {
        Hasher H;
        H.set();
        for( ListOfPrimeInfo::const_iterator i=plist.begin();i!=plist.end();++i)
        {
            H( i->p );
        }
        H(probe);
        return H.md();

    }

    static const char comparison_error_message[] ="MPN.locateNextPrime(bad comparison result)";

    bool MPN:: locateNextPrime( mpn &n ) const
    {
        assert( plist.size() == mpvec.size );

        if(n<=_2)
        {
            //------------------------------------------------------------------
            //
            // ok, easy
            //
            //------------------------------------------------------------------
            n.set_byte(2);
            return true;
        }
        else
        {
            //------------------------------------------------------------------
            //
            // check w.r.t upper
            //
            //------------------------------------------------------------------
            assert(n>_2);
            size_t                       jup  = mpvec.size;
            const MetaPrimeVector::Slot *slot = mpvec.slot;
            {
                const mpn &upper = *slot[jup];
                switch( mpn::compare(n,upper) )
                {
                    case  0: return true;  //! n == upper
                    case  1: return false; //! n > upper, need sieve
                    case -1: break;        //! n < upper
                    default: throw exception(comparison_error_message);
                }
            }

            //------------------------------------------------------------------
            //
            // bissection
            //
            //------------------------------------------------------------------
            size_t jlo = 1;
            while( jup>jlo )
            {
                const size_t jmid = (jlo+jup)>>1;
                const mpn   &pmid = *slot[jmid];
                switch( mpn::compare(n,pmid) )
                {
                    case  0: return true;         // n=pmid
                    case -1: jup = jmid-1; break; // n<pmid
                    case  1: jlo = jmid+1; break; // n>pmid
                    default: throw exception(comparison_error_message);
                }
            }
            n = *slot[jup];
            return true;
        }

    }


    bool MPN:: isComputedPrime(const mpn &n) const
    {

        //----------------------------------------------------------------------
        //
        // check against lower bound
        //
        //----------------------------------------------------------------------
        const MetaPrimeVector::Slot *slot = mpvec.slot;
        {
            const mpn &lower = *slot[1];
            switch( mpn::compare(n,lower) )
            {
                case  0: return true;  //! n == lower
                case  1: break;        //! n > lower, go further
                case -1: return false; //! n < lower
                default: throw exception(comparison_error_message);
            }
        }
        assert( n > plist.lower() );

        //----------------------------------------------------------------------
        //
        // check against upper bound
        //
        //----------------------------------------------------------------------
        size_t jup  = mpvec.size;
        {
            const mpn &upper = *slot[jup];
            //std::cerr << "..test against upper=" << upper << std::endl;
            switch( mpn::compare(n,upper) )
            {
                case  0: return true;  //! n == upper
                case  1: return false; //! n > upper, need sieve
                case -1: break;        //! n < upper
                default: throw exception(comparison_error_message);
            }
        }
        assert( n < plist.upper() );

        //----------------------------------------------------------------------
        //
        // bisection
        //
        //----------------------------------------------------------------------
        size_t jlo  = 1;
        while(jup>=jlo)
        {
            const size_t jmid = (jlo+jup)>>1;
            const mpn   &pmid = *slot[jmid];
            switch( mpn::compare(n,pmid) )
            {
                case  0: return true;         // n=pmid
                case -1: jup = jmid-1; break; // n<pmid
                case  1: jlo = jmid+1; break; // n>pmid
                default: throw exception(comparison_error_message);
            }
        }
        return false;
    }

    namespace mpl
    {
        natural:: natural(const size_t nbit, randomized::bits &gen ) : Y_MPN_CTOR(0,0)
        {
            static const mpn  & one  = MPN::instance()._1;
            if(nbit)
            {
                natural tmp = one;
                for(size_t i=nbit;i>1;--i)
                {
                    tmp <<= 1;
                    if(gen.choice())
                    {
                        tmp |= one;
                    }
                }
                xch(tmp);
            }
        }
    }

}
