//! \file
#ifndef Y_LANG_CHAR_INFO_INCLUDED
#define Y_LANG_CHAR_INFO_INCLUDED 1

#include "y/lang/types.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! origin and position of a Char
        class CharInfo : public Object
        {
        public:
            virtual ~CharInfo() throw();

            const Origin origin;  //!< origin
            const int    line;    //!< line in stamp
            const int    column;  //!< column in stamp

            //! initialize
            explicit CharInfo(const Origin &org, const int l=0, const int c=0) throw();
            //! copy
            CharInfo( const CharInfo &other ) throw();
            
            //! assign
            CharInfo & operator=( const CharInfo &other ) throw();
            //! no throw swap
            void swapWith(CharInfo &other) throw();
        };

    }
}

#endif
