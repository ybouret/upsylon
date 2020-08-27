
//! \file

#ifndef Y_PATTERN_BASIC_SINGLE_INCLUDED
#define Y_PATTERN_BASIC_SINGLE_INCLUDED 1

#include "y/jive/pattern/basic/basic.hpp"

namespace upsylon {

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! Accept one matching char
        //
        //______________________________________________________________________
        class Single : public Basic
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const char     CLID[];
            static const uint32_t UUID = Y_FOURCC('S','N','G','L');

            //__________________________________________________________________
            //
            // pattern API
            //__________________________________________________________________
            static  Single  *Create(const uint8_t);
            virtual Pattern *clone() const;

            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char *className() const throw();
            virtual size_t      serialize(ios::ostream&fp) const;

            //__________________________________________________________________
            //
            // vizible
            //__________________________________________________________________
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Single() throw();

            const uint8_t code;

        private:
            Y_DISABLE_ASSIGN(Single);
            explicit Single(const uint8_t ) throw();
            explicit Single(const Single&) throw();
            virtual bool isValid(const uint8_t)  const throw();
            virtual void vizCore(ios::ostream &) const;
        };
    }
}


#endif
