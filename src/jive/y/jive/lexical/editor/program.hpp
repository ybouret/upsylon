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
                //______________________________________________________________
                //
                //! policy for end of line
                //______________________________________________________________
                enum EndlPolicy
                {
                    EndlCopy, //!< copy
                    EndlToLF  //!< replace by '\n'
                };

                //______________________________________________________________
                //
                //
                //! set of instructions
                //
                //______________________________________________________________
                class Program : public CountedObject
                {
                public:
                    //__________________________________________________________
                    //
                    // C++
                    //__________________________________________________________
                    virtual ~Program() throw();                      //!< cleanup
                    explicit Program(const EndlPolicy how=EndlCopy); //!< setup

                    //__________________________________________________________
                    //
                    // methods
                    //__________________________________________________________

                    //! apply all instructions by in-order best match
                    void run(ios::ostream &, Source &);
                    
                    //! edit a string
                    string run(const string &);
                    
                    //! add a new instruction
                    void add(Instruction *instr);

                    //! build a new instruction
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

                    //__________________________________________________________
                    //
                    // members
                    //__________________________________________________________
                    const Instruction::List exe; //!< EXEcutable instructions
                    const Leading           xfc; //!< eXecutable First Chars
                    const Scatter           tbl; //!< table of scattered instructions

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Program);
                    Source *org; //!< temporary address of source
                    void endl_copy(ios::ostream &, const Token&);
                    void endl_toLF(ios::ostream &, const Token&);
                };

            }
        }

    }

}

#endif

