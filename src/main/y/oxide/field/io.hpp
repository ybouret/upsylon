//! \file
#ifndef Y_OXIDE_FIELD_IO_INCLUDED
#define Y_OXIDE_FIELD_IO_INCLUDED 1

#include "y/ios/plugins.hpp"
#include <typeinfo>

namespace upsylon
{

    namespace Oxide
    {

        class IO : public singleton<IO>, public ios::plugins
        {
        public:
            static const at_exit::longevity life_time = 0;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(IO);
            explicit IO();
            virtual ~IO() throw();
            friend class singleton<IO>;
        };

    }
}

#endif

