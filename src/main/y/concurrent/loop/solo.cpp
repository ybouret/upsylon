
#include "y/concurrent/loop/solo.hpp"



namespace upsylon
{

    namespace concurrent
    {

        solo:: solo() throw() :
        sync(),
        here()
        {
        }

        solo:: ~solo() throw()
        {
        }


        void solo:: for_each(runnable &obj) throw()
        {
            obj.run(here,sync);
        }
        
        size_t solo:: size() const throw() { return 1; }

        const context & solo:: operator[](const size_t) const throw()
        {
            return here;
        }

    }

}


