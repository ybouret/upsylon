

//! \file

#ifndef Y_JIVE_DIALECT_COMPILER_INCLUDED
#define Y_JIVE_DIALECT_COMPILER_INCLUDED 1

#include "y/jive/dialect/parser.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Dialect
        {
            class Compiler
            {
            public:
                explicit Compiler();
                virtual ~Compiler() throw();

                Jive::Parser *compileAST( XNode::Pointer &ast );


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Compiler);
                static const string *GetName( XNode::Pointer &ast );

            };
        }

    }

}

#endif
