//! \file
#ifndef Y_LANG_CHAR_INFO_INCLUDED
#define Y_LANG_CHAR_INFO_INCLUDED 1

#include "y/lang/types.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! origin and position of a Character
        class CharInfo : public Object
        {
        public:
            virtual ~CharInfo() throw();

            const Tag    origin;  //!< origin from Module
            const int    line;    //!< line  in origin
            const int    column;  //!< column origin

            //! initialize
            explicit CharInfo(const Tag &org, const int l, const int c) throw();

            //! no-throw copy
            CharInfo( const CharInfo &other ) throw();

            //! no-throw assign by copy/swap
            CharInfo & operator=( const CharInfo &other ) throw();

            //! no throw swap
            void swapWith(CharInfo &other) throw();
        };

    }
}

#endif
