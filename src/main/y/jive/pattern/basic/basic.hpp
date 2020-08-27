
//! \file

#ifndef Y_PATTERN_BASIC_INCLUDED
#define Y_PATTERN_BASIC_INCLUDED 1

#include "y/jive/pattern.hpp"

namespace upsylon {

    namespace Jive
    {
        class Basic : public Pattern
        {
        public:
            virtual ~Basic() throw();


            virtual  Result accept(Y_PATTERN_ACCEPT_ARGS) const;

        protected:
            explicit Basic(const uint32_t) throw();
            explicit Basic(const Basic  &) throw();

        private:
            Y_DISABLE_ASSIGN(Basic);
            virtual bool isValid(const uint8_t code) const throw() = 0;

        };
    }
}


#endif
