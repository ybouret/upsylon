
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
        
        struct TextFor
        {
            static const char *Terminal;
            static const char *Operator;
            static const char *Aggregate;
            static const char *Alternate;
            static const char *OneOrMore;
            static const char *ZeroOrMore;

        };
    }
}

#endif

