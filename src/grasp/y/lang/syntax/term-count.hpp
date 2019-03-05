
//! \file
#ifndef Y_LANG_SYNTAX_TERM_COUNT_INCLUDED
#define Y_LANG_SYNTAX_TERM_COUNT_INCLUDED 1

#include "y/os/platform.hpp"
#include <iostream>

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            //! count the number of possibly emitted terminals
            class TermCount
            {
            public:
                unsigned nmin; //!< minimal number of emitted terminals
                int      nmax; //!< maximal number of emitter terminals, negatif means infinite...

                TermCount(const unsigned lo=0, const int up=0) throw(); //!< setup
                TermCount(const TermCount &) throw();                   //!< copy
                TermCount&operator=(const TermCount&) throw();          //!< assign
                ~TermCount() throw();                                   //!< destruct

                friend std::ostream & operator<<( std::ostream &, const TermCount &);
            };

        }
    }
}

#endif

