//! \file
#ifndef Y_LANG_SYNTAX_ANALYZER_INCLUDED
#define Y_LANG_SYNTAX_ANALYZER_INCLUDED 1

#include "y/lang/syntax/node.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            //! Base class to analyze a node
            class Analyzer : public Object
            {
            public:
                virtual ~Analyzer() throw(); //!< constructor
                explicit Analyzer() throw(); //!< desctructor

                virtual void onTerminal( const string &id, const Lexeme &lx );
                virtual void onInternal( const string &id, const size_t  sz, const string *data);
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Analyzer);
            };



        }
    }
}

#endif

