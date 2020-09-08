//! \file


#ifndef Y_JIVE_LEXICAL_EDITOR_PROGRAM_INCLUDED
#define Y_JIVE_LEXICAL_EDITOR_PROGRAM_INCLUDED 1

#include "y/jive/lexical/editor/instruction.hpp"
#include "y/jive/scatter.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Lexical
        {

            namespace Editor
            {

                class Program : public CountedObject
                {
                public:
                    typedef Instruction::List          Instructions;
                    typedef suffix_tree<Instruction *> InstrDB;

                    const Instructions exe; //!< EXEcutable instructions
                    const InstrDB      idb; //!< Instruction DataBase
                    const Leading      xfc; //!< eXecutable First Chars
                    const Scatter      tbl;

                    virtual ~Program() throw();
                    explicit Program();

                    void run(ios::ostream &, Source &);
                    

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Program);
                };

            }
        }

    }

}

#endif

