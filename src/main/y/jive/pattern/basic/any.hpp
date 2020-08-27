//! \file

#ifndef Y_PATTERN_BASIC_ANY_INCLUDED
#define Y_PATTERN_BASIC_ANY_INCLUDED 1

#include "y/jive/pattern/basic/basic.hpp"

namespace upsylon {

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! Accept any char
        //
        //______________________________________________________________________
        class Any : public Basic
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const char     CLID[];
            static const uint32_t UUID = Y_FOURCC('A','N','Y','1');

            //__________________________________________________________________
            //
            // pattern API
            //__________________________________________________________________
            static  Any     *Create();
            virtual Pattern *clone() const;

            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            virtual const char *className() const throw();
            virtual size_t      serialize(ios::ostream&fp) const;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Any() throw();

        private:
            Y_DISABLE_ASSIGN(Any);
            explicit Any() throw();
            explicit Any(const Any&) throw();
            virtual bool isValid(const uint8_t) const throw();
        };
    }
}


#endif
