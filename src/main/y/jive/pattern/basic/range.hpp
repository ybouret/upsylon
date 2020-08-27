//! \file

#ifndef Y_PATTERN_BASIC_RANGE_INCLUDED
#define Y_PATTERN_BASIC_RANGE_INCLUDED 1

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
        class Range : public Basic
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const char     CLID[];
            static const uint32_t UUID = Y_FOURCC('R','N','G','E');

            //__________________________________________________________________
            //
            // pattern API
            //__________________________________________________________________
            static  Range   *Create(const uint8_t,const uint8_t);
            virtual Pattern *clone() const;
            virtual void     start(FirstChars &) const; //!< insert range

            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char *className() const throw();
            virtual size_t      serialize(ios::ostream&fp) const;
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Range() throw();

            const uint8_t lower;
            const uint8_t upper;

        private:
            Y_DISABLE_ASSIGN(Range);
            explicit Range(const uint8_t, const uint8_t) throw();
            explicit Range(const Range&) throw();
            virtual bool isValid(const uint8_t)  const throw();
            virtual void vizCore(ios::ostream &) const;
        };
    }
}


#endif
