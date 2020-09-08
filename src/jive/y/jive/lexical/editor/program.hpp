//! \file


#ifndef Y_JIVE_LEXICAL_EDITOR_PROGRAM_INCLUDED
#define Y_JIVE_LEXICAL_EDITOR_PROGRAM_INCLUDED 1

#include "y/jive/lexical/editor/instruction.hpp"
#include "y/jive/scatter.hpp"
#include "y/jive/regexp.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Lexical
        {

            namespace Editor
            {

                enum EndlPolicy
                {
                    EndlCopy,
                    EndlToLF
                };

                class Program : public CountedObject
                {
                public:

                    typedef Instruction::List          Instructions;

                    const Instructions exe; //!< EXEcutable instructions
                    const Leading      xfc; //!< eXecutable First Chars
                    const Scatter      tbl;

                    virtual ~Program() throw();
                    explicit Program(const EndlPolicy how=EndlCopy);

                    void run(ios::ostream &, Source &);

                    void add(Instruction *instr);

                    template <typename REGEXP,
                    typename OBJECT_POINTER,
                    typename METHOD_POINTER>
                    void on(const REGEXP        &rx,
                            const OBJECT_POINTER hObject,
                            const METHOD_POINTER hMethod,
                            const Dictionary    *dict = 0)
                    {
                        const  Motif motif = RegExp(rx,dict);
                        const  Code  code(hObject,hMethod);
                        return add( new Instruction(motif,code) );
                    }
                    

                protected:
                    Source *org; //!< temporary address of source

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Program);
                    void endl_copy(ios::ostream &, const Token&);
                    void endl_toLF(ios::ostream &, const Token&);
                };

            }
        }

    }

}

#endif

