
//! \file

#ifndef Y_JIVE_LEXICAL_RULE_INCLUDED
#define Y_JIVE_LEXICAL_RULE_INCLUDED 1

#include "y/jive/lexical/event.hpp"
#include "y/jive/pattern.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {

            //------------------------------------------------------------------
            //
            //! base class for rules
            /**
             trigger and event uppon motif
             */
            //------------------------------------------------------------------

            class Rule : public Object, public dnode<Rule>
            {
            public:

                const Tag           label; //!< shared label for Lexemes
                const Motif         motif; //!< motif to match
                const Event::Handle event; //!< event to trigger

                //! cleanup
                virtual ~Rule() throw();

                //! setup
                explicit Rule(const Tag           &ruleLabel,
                              const Motif         &ruleMotif,
                              const Event::Handle &ruleEvent);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rule);
            };

            typedef core::list_of_cpp<Rule> Rules; //!< alias
        }
    }
}

#endif

