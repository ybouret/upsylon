
//! \file

#ifndef Y_JIVE_LEXICAL_EDITOR_INCLUDED
#define Y_JIVE_LEXICAL_EDITOR_INCLUDED 1

#include "y/jive/lexical/rule.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {

            class Editor
            {
            public:
                typedef functor<void,TL2(ios::ostream&,const Token &)> Instruction;

                typedef suffix_tree<Rule*> RulesDB; //!< rules database

                explicit Editor();
                virtual ~Editor() throw();

                const Rules    rules;
                const RulesDB  hoard;
                const Leading  intro;
                const Leading  outro;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Editor);
            };
        }

    }

}

#endif

