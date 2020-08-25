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
        pstart(_5),
        primes(),
        launch(pstart),
        p2( new prime(2) ),
        p3( new prime(3) )
        {

        }

        void library:: reset_primes() throw()
        {
            aliasing::_(primes).release();
            aliasing::_(launch).make(5);
            assert(5==launch);
        }

        bool library:: is_prime_(const natural &n) const
        {
            switch(n.word[0])
            {
                case 0:
                case 1:
                    return false;
                case 2:
                case 3:
                    return true;
                default: if( n.is_divisible_by(_2) || n.is_divisible_by(_3) ) return false;

            }
            assert(n>=5);

            for(const prime *node = primes.head; node; node=node->next)
            {
                const prime &p = *node;
                if( (p.squared>n) ) return true;
                if(n.is_divisible_by(p) || n.is_divisible_by(p.add_two) ) return false;
            }


            natural p = launch; assert(p.is_odd());
            while(true)
            {
                { const natural q = natural::square_of(p); if(q>n) return true; }
                if( n.is_divisible_by(p) ) return false;
                const natural   t = p+_2;
                if( n.is_divisible_by(t) ) return false;
                p += _6;
            }
        }

        bool library:: is_prime_(const number::utype u) const
        {
            const natural _(u); return is_prime_(_);
        }

        const prime &library:: prefetch()
        {
            assert(launch>=_5);
            assert(launch.is_odd());
            natural guess = primes.tail ? (*primes.tail)+_2 : launch;
            while(!is_prime_(guess))
            {
                guess += _2;
            }
            natural start = launch;
            while(start<=guess) start += _6;
            
            aliasing::_(primes).push_back( new prime(guess) );
            aliasing::_(launch).xch(start);

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
            
            natural top = pstart;
            if(available)
            {
            FIND_TOP:
                top += _2;
                const natural del = top - pstart;
                if(!del.is_divisible_by(_6))
                {
                    top += _2;
                    goto FIND_TOP;
                }
            }
            
            // ok
            aliasing::_(launch).xch(top);
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

        bool library:: prune()
        {
            switch(primes.size)
            {
                case 0: return false;
                case 1: aliasing::_(launch)=pstart; aliasing::_(primes).release(); return true;
                default:
                    break;
            }
            assert(primes.size>=2);
            natural top = *(primes.tail->prev); assert(top.is_odd());
            top += _2;
            FIND_TOP:
            {
                const natural delta = top-_5;
                if(!delta.is_divisible_by(_6))
                {
                    top += _2;
                    goto FIND_TOP;
                }
            }
            aliasing::_(launch).xch(top);
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

