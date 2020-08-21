#include "y/yap/library.hpp"
#include <iostream>

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
            primes.release();
            launch.make(5);
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

            assert(launch==5||(primes.tail&&(launch== 6+*(primes.tail) )));

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

        void library:: prefetch()
        {
            natural guess = launch;
            while(!is_prime_(guess))
            {
                guess += _2;
            }
            natural next_launch = guess + _6;
            primes.push_back( new prime(guess) );
            launch.xch(next_launch);
            std::cerr << "--> guess=" << guess << " | launch=" << launch << std::endl;
        }


    }

}

