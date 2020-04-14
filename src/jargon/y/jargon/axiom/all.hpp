
//! \file

#ifndef Y_JARGON_AXIOM_ALL_INCLUDED
#define Y_JARGON_AXIOM_ALL_INCLUDED 1

#include "y/jargon/axiom/terminal.hpp"
#include "y/jargon/axiom/operator.hpp"
#include "y/jargon/axiom/compound/aggregate.hpp"
#include "y/jargon/axiom/compound/alternate.hpp"
#include "y/jargon/axiom/wildcard/option.hpp"
#include "y/jargon/axiom/wildcard/repeat.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! const char for classes
        struct TextFor
        {
            static const char *Terminal;   //!< Terminal
            static const char *Operator;   //!< Operator
            static const char *Aggregate;  //!< Aggregate
            static const char *Alternate;  //!< Alternate
            static const char *OneOrMore;  //!< OneOrMore
            static const char *ZeroOrMore; //!< ZeroOrMore

        };
    }
}

#endif

