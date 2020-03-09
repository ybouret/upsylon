//! \file
#ifndef Y_LANG_CHAR_INCLUDED
#define Y_LANG_CHAR_INCLUDED 1

#include "y/lang/char-info.hpp"
#include "y/core/inode.hpp"
#include "y/core/list.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! full fledged class, a code and its position
        class Char : public CharInfo, public inode<Char>
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef inode<Char>             Base; //!< alias
            typedef core::list_of_cpp<Char> List; //!< list for I/O or buffer

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            explicit Char(const uint8_t,const CharInfo &) throw();//!< constructor
            virtual ~Char() throw();                              //!< destructor
            Char(const Char &other) throw();                      //!< hard copy

            //__________________________________________________________________
            //
            // data
            //__________________________________________________________________
            uint8_t code; //!< current char value

        private:
            Y_DISABLE_ASSIGN(Char);
        };
        

    }

}
#endif

