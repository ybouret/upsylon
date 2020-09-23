//! \file
#ifndef Y_JIVE_SYNTAX_AXIOM_INCLUDED
#define Y_JIVE_SYNTAX_AXIOM_INCLUDED 1

#include "y/jive/syntax/node.hpp"
#include "y/type/fourcc.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {

            class Axiom : public CountedObject, public inode<Axiom>
            {
            public:
                static const uint32_t TerminalUUID = Y_FOURCC('T','E','R','M');
                virtual ~Axiom() throw();

                bool   isTerminal() const throw();
                bool   isInternal() const throw();

                const uint32_t    uuid;
                const Tag         name;
                const void *const self;


            protected:
                explicit Axiom(const uint32_t, const string *) throw();
                void     I_am(const void *)    throw();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Axiom);
            };

            //! helper to link derived class to self
#define Y_JIVE_AXIOM(CLASS) I_am( static_cast<const CLASS *>(this) )
        }
    }
}

#endif

