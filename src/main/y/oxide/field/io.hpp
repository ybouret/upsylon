//! \file
#ifndef Y_OXIDE_FIELD_IO_INCLUDED
#define Y_OXIDE_FIELD_IO_INCLUDED 1

#include "y/ios/plugins.hpp"
#include <typeinfo>

namespace upsylon
{

    namespace Oxide
    {

        //! input/output plugins for fields
        class IO : public singleton<IO>, public ios::plugins
        {
        private:
            Y_DISABLE_COPY_AND_ASSIGN(IO);
            explicit IO();
            virtual ~IO() throw();
            friend class singleton<IO>;
            static const at_exit::longevity life_time = 0;

        public:

            //! use a multi loop to fill a sub layout with the same value
            template <typename FIELD>static inline
            void LD(FIELD                            &F,
                    const typename FIELD::LayoutType &L,
                    typename FIELD::param_type        V )
            {
                assert(F.contains(L));
                typename FIELD::LayoutType::Loop loop(L.lower,L.upper);
                for( loop.start(); loop.valid(); loop.next() )
                {
                    F( loop.value ) = V;
                }
            }

        };

    }
}

#endif

