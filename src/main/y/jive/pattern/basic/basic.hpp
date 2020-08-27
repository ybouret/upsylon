
//! \file

#ifndef Y_PATTERN_BASIC_INCLUDED
#define Y_PATTERN_BASIC_INCLUDED 1

#include "y/jive/pattern.hpp"

namespace upsylon {

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! Basic pattern: accept 0 or 1 char, by isValid
        //
        //______________________________________________________________________
        class Basic : public Pattern
        {
        public:

            //__________________________________________________________________
            //
            // common pattern API
            //__________________________________________________________________
            //! accept 0 or 1 char
            /**
             return Finished if no more char!
             */
            virtual  Result accept(Y_PATTERN_ACCEPT_ARGS) const;
            virtual  bool   feeble() const throw(); //!< false, always strong
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Basic()               throw(); //!< cleanup
        protected:
            explicit Basic(const uint32_t) throw(); //!< setup
            explicit Basic(const Basic  &) throw(); //!< copy

        private:
            Y_DISABLE_ASSIGN(Basic);
            virtual bool isValid(const uint8_t code) const throw() = 0;
        };
    }
}


#endif
