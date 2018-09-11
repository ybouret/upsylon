//! \file

#ifndef Y_LANG_LEXICAL_RULE_INCLUDED
#define Y_LANG_LEXICAL_RULE_INCLUDED 1

#include "y/lang/lexical/unit.hpp"
#include "y/lang/pattern.hpp"
#include "y/functor.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
            //! Result of a lexical Action
            enum Result
            {
                Forward, //!< will produce an unit
                Discard, //!< will discard the unit
                Control  //!< a control unit for translator
            };

            //! lexical action to undertake for a given token
            typedef functor<Result,TL1(const Token &)> Action;

            //! lexical rule motif->action
            class Rule : public Object
            {
            public:
                typedef core::list_of_cpp<Rule> List;

                Rule                 *next;   //!< for List
                Rule                 *prev;   //!< for List
                const Origin          label;  //!< identifier
                const Pattern::Handle motif;  //!< matching pattern
                Action                action; //!< what to do upon it

                virtual ~Rule() throw(); //!< destructor
                Rule(const Rule &other); //!< copy, for list if necessary
                Rule(const Origin &, const Pattern::Handle &, const Action &);


            private:
                Y_DISABLE_ASSIGN(Rule);
            };

        }
    }
}

#endif
