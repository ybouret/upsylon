

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
            //__________________________________________________________________
            //
            //
            //! compile a parsed grammar description
            //
            //__________________________________________________________________
            class Compiler
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit Compiler();          //!< setup
                virtual ~Compiler() throw();  //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! full compilation
                Jive::Parser *compileAST( XNode::Pointer &ast );


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Compiler);
                static const string *GetName( XNode::Pointer &ast );

            };
        }

    }

}

#endif
