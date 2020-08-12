#include "y/randomized/isaac.hpp"
#include "y/concurrent/singleton.hpp"

namespace upsylon
{

    namespace randomized
    {

        class BitsSource : public singleton<BitsSource>
        {
        public:

            explicit BitsSource() throw() :
            __simple( ISAAC_INIT_RAND ),
            __crypto( ISAAC_INIT_RAND )
            {
            }

            virtual ~BitsSource() throw()
            {
            }

            ISAAC<4> __simple;
            ISAAC<8> __crypto;

            Y_SINGLETON_DECL(BitsSource);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(BitsSource);
        };


        bits & bits:: simple()
        {
            return BitsSource::instance().__simple;
        }

        bits & bits:: crypto()
        {
            return BitsSource::instance().__crypto;
        }

        lockable & bits:: access()
        {
            return BitsSource::access;
        }
    }

    Y_SINGLETON_IMPL_WITH(0,randomized::BitsSource);

}

