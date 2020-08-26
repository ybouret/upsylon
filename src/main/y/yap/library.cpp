#include "y/yap/library.hpp"
#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/xnumeric.hpp"
#include "y/exception.hpp"

namespace upsylon {

    Y_SINGLETON_IMPL_WITH(object::life_time-1,yap::library);

    namespace yap {

        library:: ~library() throw()
        {
        }

        library:: library()  :
        _0(0),
        _1(1),
        _2(2),
        _3(3),
        _4(4),
        _5(5),
        _6(6),
        _7(7),
        _8(8),
        _9(9),
        _10(10),
        _11(11),
        _12(12),
        _13(13),
        _14(14),
        _15(15),
        _16(16),
        _z0(),
        _q0(),
        primes(),
        mul6m1(_5),
        mul6p1(_7),
        p2( new prime(2) ),
        p3( new prime(3) )
        {

        }

        void library:: reset_primes() throw()
        {
            aliasing::_(primes).release();
            aliasing::_(mul6m1).make(5);   assert(_5==mul6m1);
            aliasing::_(mul6p1).make(7);   assert(_7==mul6p1);
        }

        bool library:: is_prime_(const natural &n) const
        {

            if(n<=_3)
            {
                // may be 0,1,2,3
                return (n>_1);
            }
            else
            {
                //if( n.is_divisible_by(_2) || n.is_divisible_by(_3) )
                if( n.is_even() || n.is_divisible_by(_3) )
                {
                    return false;
                }
                else
                {
                    assert(n>=5);

                    for(const prime *prm = primes.head;prm;prm=prm->next)
                    {
                        if(prm->squared>n) return true;
                        if(n.is_divisible_by(*prm)) return false;
                    }


                    natural p = mul6m1;
                    natural q = mul6p1;
                    while(true)
                    {
                        { const natural s = natural::square_of(p); if(s>n) return true; }
                        if( n.is_divisible_by(p) ) return false;
                        if( n.is_divisible_by(q) ) return false;
                        p += _6;
                        q += _6;
                    }

                }
            }


        }

        bool library:: is_prime_(const number::utype u) const
        {
            const natural _(u); return is_prime_(_);
        }

        const prime &library:: prefetch()
        {
            natural guess = primes.tail ? (*primes.tail+_2) : _5;
            while( !is_prime_(guess) )
            {
                guess += _2;
            }
            aliasing::_(primes).push_back( new prime(guess) );
            try
            {
                configure(guess);
                return *primes.tail;
            }
            catch(...)
            {
                delete aliasing::_(primes).pop_back();
                throw;
            }

            return *primes.tail;
        }

        const prime & library:: prefetch(size_t n)
        {
            while(n-- > 0)
            {
                (void) prefetch();
            }
            return *primes.tail;

        }

        natural library:: next_prime_(const natural &n) const
        {
            if(n<=_1)
            {
                return _2;
            }
            else
            {
                natural p = n;
                ++p |= _1; assert(p>n); assert(p.is_odd());
                while(!is_prime_(p)) p += _2;
                return p;
            }
        }

        const char library:: CLASS_NAME[] = "yapl";

        const char *library:: className() const throw() { return CLASS_NAME; }

        size_t library:: serialize(ios::ostream &os) const
        {
            // write available primes
            size_t  ans = os.write_upack(primes.size);
            natural old = _3;
            for(const prime *p=primes.head;p;p=p->next)
            {
                const natural &cur = *p;
                natural        tmp = cur-old; assert(tmp>0); assert(tmp.is_even());
                tmp.shr(1); assert(tmp>0);
                --tmp;
                assert(old+2*(tmp+1)==cur);
#if 0
                ans += tmp.serialize(os);
#else
                ans += os.write_upack( tmp.cast_to<size_t>("prime code") );
#endif
                old = cur;
            }
            return ans;
        }
        
        
        
        void library:: load(ios::istream &fp,const size_t nmax)
        {
            static const char fn[]= "yap::library::load: ";
            size_t available = 0;
            size_t shift     = 0;
            if( !fp.query_upack(available,shift) ) throw exception("%smissing #available",fn);
            if(nmax>0&&nmax<available) available=nmax;
            
            
            prime::list_type plist;
            natural          guess = *p3;
            for(size_t i=1;i<=available;++i)
            {
                size_t delta = 0;
                size_t bytes = 0;
                if( !fp.query_upack(delta,bytes) )
                {
                    throw exception("%smissing prime #%lu",fn,(unsigned long)i);
                }
                shift += bytes;
                ++delta;
                delta <<= 1;
                guess += delta;
                plist.push_back( new prime(guess) );
            }
            

            // find mul6xx, may throw
            configure(plist.size? *(plist.tail) : _3);

            // no-throw swap list
            aliasing::_(primes).swap_with(plist);
            
        }

        

        const prime & library:: upper() const throw()
        {
            return (primes.size<=0) ? (*p3) : *(primes.tail);
        }

        size_t library:: size()  const throw()
        {
            return 2+primes.size;
        }

        void library:: configure(const natural &from)
        {
            assert(from>=_3); assert(from.is_odd());

            natural lo = from + _2; assert(lo.is_odd());
        FIND_LO:
            const natural del = lo - _5; assert(del.is_even());
            if(!del.is_divisible_by(_6))
            {
                lo += _2;
                goto FIND_LO;
            }
            natural up = lo + _2;
            assert( (lo-_5)%_6 == 0 );
            assert( (up-_7)%_6 == 0 );

            aliasing::_(mul6m1).xch(lo);
            aliasing::_(mul6p1).xch(up);

        }

        bool library:: prune()
        {
            switch(primes.size)
            {
                case 0: return false;
                case 1: reset_primes(); return true;
                default:
                    break;
            }
            assert(primes.size>=2);
            configure( *(primes.tail->prev) );
            delete aliasing::_(primes).pop_back();
            return true;
        }

        
    }

    template <> const apn & xnumeric<apn>:: abs_minimum()
    {
        const yap::library &apl = yap::library::instance();
        return apl._0;
    }
    
    template <> const apz & xnumeric<apz>:: abs_minimum()
    {
        const yap::library &apl = yap::library::instance();
        return apl._z0;
    }
    
    template <> const apq & xnumeric<apq>:: abs_minimum()
    {
        const yap::library &apl = yap::library::instance();
        return apl._q0;
    }
    
    


}

