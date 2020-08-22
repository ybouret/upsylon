#include "y/yap/library.hpp"
#include "y/ios/ostream.hpp"
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
        launch(_5)
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
            natural guess = launch;
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
                ans += tmp.serialize(os);
                old = cur;
            }
            return ans;
        }

        library::iterator:: iterator(const prime *p, library &l) throw() :
        ptr(p),
        lib(l)
        {
        }
        
        library::iterator:: iterator(const iterator &it) throw() :
        ptr(it.ptr),
        lib(it.lib)
        {
        }
        
        library:: iterator:: ~iterator() throw()
        {
            ptr=0;
        }
        
        library::iterator library::begin() throw()
        {
            if(primes.size)
            {
                return iterator(primes.head,*this);
            }
            else
            {
                return iterator(&prefetch(), *this);
            }
        }

        void library::iterator:: next()
        {
            if(!ptr) throw exception("invalid library::iterator::next");
            ptr=ptr->next;
            if(!ptr)
            {
                ptr = & lib.prefetch();
            }
        }
        
        void library::iterator:: next(size_t n)
        {
            while(n-- > 0)
            {
                next();
            }
        }
        
        void library::iterator:: prev()
        {
            if(!ptr) throw exception("invalid library::iterator::prev");
            ptr=ptr->prev;
        }
        
        bool library:: iterator:: is_valid() const throw()
        {
            return NULL!=ptr;
        }
            
        const prime * library::iterator:: operator->() const throw()
        {
            assert(NULL!=ptr);
            return ptr;
        }
        
        const prime & library:: iterator:: operator*() const throw()
        {
            assert(NULL!=ptr);
            return *ptr;
        }

        library::iterator & library::iterator:: operator==( const iterator &other) throw()
        {
            ptr=other.ptr;
            return *this;
        }

        library::iterator  & library::iterator:: operator++()
        {
            next();
            return *this;
        }
        
        library::iterator   library::iterator:: operator++(int)
        {
            const iterator ans(*this);
            next();
            return ans;
        }
        
        library::iterator  & library::iterator::operator +=(const size_t n)
        {
            next(n);
            return *this;
        }
        
        library::iterator operator+( const library::iterator &lhs, const size_t n)
        {
            library::iterator ans(lhs);
            ans.next(n);
            return ans;
        }
        
        library::iterator operator+( const size_t n, const library::iterator &rhs )
        {
            library::iterator ans(rhs);
            ans.next(n);
            return ans;
        }
        
        bool operator==(const library::iterator &a, const library::iterator &b)
        {
            return (a.ptr==b.ptr);
        }
        
        bool operator!=(const library::iterator &a,const library::iterator &b)
        {
            return (a.ptr!=b.ptr);
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

