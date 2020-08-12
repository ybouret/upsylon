//! \file
#ifndef Y_HASHING_FACTORY_INCLUDED
#define Y_HASHING_FACTORY_INCLUDED 1

#include "y/concurrent/singleton.hpp"
#include "y/container/xfactory.hpp"

namespace upsylon {

    namespace hashing {

        //! a repository of implemented hash functions and more
        class factory : public xfactory<function>, public singleton<factory>
        {
        public:

        private:
            Y_DISABLE_COPY_AND_ASSIGN(factory);
            virtual ~factory() throw();
            explicit factory();
            void     initialize();
            Y_SINGLETON_DECL(factory);
        };

    }

}


#endif

