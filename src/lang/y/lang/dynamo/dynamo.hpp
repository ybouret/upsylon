//! \file
#ifndef Y_LANG_DYNAMO_INCLUDED
#define Y_LANG_DYNAMO_INCLUDED 1

#include "y/lang/compiler.hpp"

namespace upsylon {

    namespace Lang
    {

        //! Dynamic Langage Compiler
        struct Dynamo
        {
            class Parser : public Syntax::Parser
            {
            public:
                //! define the generator parser
                explicit Parser();

                //! destructor
                virtual ~Parser() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Parser);
            };


            class Generator : public Lang::Compiler
            {
            public:
                explicit Generator();
                virtual ~Generator() throw();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Generator);
            };
        };

    }
}


#endif

