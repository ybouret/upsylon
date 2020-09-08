
//! \file


#ifndef Y_JIVE_LEXICAL_EDITOR_INSTRUCTION_INCLUDED
#define Y_JIVE_LEXICAL_EDITOR_INSTRUCTION_INCLUDED 1

#include "y/jive/pattern.hpp"
#include "y/functor.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {

            namespace Editor {

                typedef functor<void,TL2(ios::ostream&,const Token&)> Code;

                class Instruction : public Object, public inode<Instruction>
                {
                public:
                    typedef core::list_of_cpp<Instruction> List;
                    
                    virtual ~Instruction() throw();
                    explicit Instruction(const Motif &m, const Code &c);

                    const Motif motif;
                    const Code  code;

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Instruction);

                };


            }
        }

    }

}

#endif

