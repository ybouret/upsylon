
#include "y/jive/lexical/editor/instruction.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {

            namespace Editor {

                Instruction:: ~Instruction() throw()
                {

                }

                Instruction:: Instruction(const Motif &m,const Code &c) :
                motif(m),
                code(c)
                {
                }
                

            }
        }

    }

}
