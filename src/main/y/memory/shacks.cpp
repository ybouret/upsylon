
#include "y/memory/shacks.hpp"


namespace upsylon {

    namespace memory
    {

        shacks:: ~shacks() throw()
        {
        }

        shacks:: shacks(const size_t n, const construct_mode ctor) :
        slots_type(n)
        {
            switch (ctor) {
                case construct_vacant:
                    break;

                case construct_filled:
                    fill();
                    break;

            }
        }

        void shacks:: fill()
        {
            while(size()<count)
            {
                const arc_ptr<shack> tmp = new shack();
                push(tmp);
            }
        }

    }

}


