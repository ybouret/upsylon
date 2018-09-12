//! \file
#ifndef Y_LANG_CHAR_INCLUDED
#define Y_LANG_CHAR_INCLUDED 1

#include "y/lang/char-info.hpp"

namespace upsylon
{
    namespace Lang
    {
        //! full fledge class
        class Char : public CharInfo
        {
        public:
            Char   *next; //!< for list
            Char   *prev; //!< for list
            uint8_t code; //!< current char value

            //! constructor
            inline          Char(const uint8_t c, const CharInfo &info) throw() : CharInfo(info), next(0), prev(0), code(c) {}

            //! destructor
            inline virtual ~Char() throw() {}
            //! copy
            inline          Char(const Char &other) throw() : CharInfo(other), next(0), prev(0), code(other.code) {}


            typedef core::list_of_cpp<Char> List; //!< list for I/O or buffer

        private:
            Y_DISABLE_ASSIGN(Char);
        };
        

    }

}
#endif

