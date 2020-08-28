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
            static const char     CLID[];                           //!< CLID
            static const uint32_t UUID = Y_FOURCC('A','N','Y','1'); //!< UUID

            //__________________________________________________________________
            //
            // pattern API
            //__________________________________________________________________
            static  Any     *Create();                          //!< new
            virtual Pattern *clone()             const;         //!< copy
            virtual void     start(FirstChars &) const;         //!< complete!
            virtual void     update(Entropy &)   const throw(); //!< do nothing

            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char *className() const throw();         //!< CLID
            virtual size_t      serialize(ios::ostream&fp) const;  //!< only id

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Any() throw(); //!< cleanup

            //! display
            friend std::ostream & operator<<(std::ostream &, const Any&);

        private:
            Y_DISABLE_ASSIGN(Any);
            explicit Any() throw();
            explicit Any(const Any&) throw();
            virtual bool isValid(const uint8_t) const throw();
            virtual void vizCore(ios::ostream &) const;
        };
    }
}


#endif
