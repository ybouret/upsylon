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
            //__________________________________________________________________
            //
            // data
            //__________________________________________________________________
            const Tag    origin;  //!< origin from Module
            const int    line;    //!< line  in origin
            const int    column;  //!< column origin

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            explicit CharInfo(const Tag &org,               //|
                              const int l,                  //|
                              const int c) throw();         //!< initialize
            virtual ~CharInfo() throw();                    //!< destructor
            CharInfo( const CharInfo & ) throw();           //!< no-throw copy
            CharInfo & operator=(const CharInfo &) throw(); //!< no-throw assign by copy/swap
            void swapWith(CharInfo &other) throw();         //!< no throw swap
        };

    }
}

#endif
