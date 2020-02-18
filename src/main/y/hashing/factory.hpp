//! \file
#ifndef Y_HASHING_FACTORY_INCLUDED
#define Y_HASHING_FACTORY_INCLUDED 1

#include "y/concurrent/singleton.hpp"
#include "y/container/xfactory.hpp"

namespace upsylon {

    namespace hashing {

        class factory : public xfactory<function>, public singleton<factory>
        {
        public:

            static const at_exit::longevity life_time = 0;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(factory);
            friend class singleton<factory>;
            virtual ~factory() throw();
            explicit factory();

            void     initialize();
        };

    }

}


#endif

