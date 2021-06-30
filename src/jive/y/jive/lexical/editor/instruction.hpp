
//! \file


#ifndef Y_JIVE_LEXICAL_EDITOR_INSTRUCTION_INCLUDED
#define Y_JIVE_LEXICAL_EDITOR_INSTRUCTION_INCLUDED 1

#include "y/jive/pattern.hpp"
#include "y/functor.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {

            namespace Editor {

                //______________________________________________________________
                //
                //
                //! alias to process a matched token to an output stream
                //
                //______________________________________________________________
                typedef functor<void,TL2(ios::ostream&,const Token&)> Code;

                //______________________________________________________________
                //
                //
                //! pattern and code holder
                //
                //______________________________________________________________
                class Instruction : public Object, public dnode<Instruction>
                {
                public:
                    virtual ~Instruction() throw();                    //!< cleanup
                    explicit Instruction(const Motif &, const Code &); //!< setup

                    const Motif motif; //!< compiled pattern
                    const Code  code;  //!< what to do with the accepted token

                    typedef core::list_of_cpp<Instruction> List; //!< alias
                    
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Instruction);

                };


            }
        }

    }

}

#endif

