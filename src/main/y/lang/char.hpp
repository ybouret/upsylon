//! \file
#ifndef Y_LANG_CHAR_INCLUDED
#define Y_LANG_CHAR_INCLUDED 1

#include "y/lang/char-info.hpp"
#include "y/core/node.hpp"
#include "y/core/list.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! full fledge class, a code and its position
        class Char : public CharInfo, public core::inode<Char>
        {
        public:
            typedef core::inode<Char>       Base; //!< alias
            typedef core::list_of_cpp<Char> List; //!< list for I/O or buffer

            uint8_t code; //!< current char value

            //! constructor
            inline explicit Char(const uint8_t c, const CharInfo &info) throw() : CharInfo(info), Base(), code(c) {}
            //! destructor
            inline virtual ~Char() throw() {}
            //! copy
            inline Char(const Char &other) throw() : CharInfo(other), Base(), code(other.code) {}

        private:
            Y_DISABLE_ASSIGN(Char);
        };
        

    }

}
#endif

