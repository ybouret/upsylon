#include "y/yap/library.hpp"


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
            primes.push_back( new prime(_2) );
            primes.push_back( new prime(_3) );
        }

        void library:: reset_primes() throw()
        {
            while(primes.size>2) delete primes.pop_back();
            launch.make(5);
            assert(5==launch);
        }
        

    }

}

