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

            //! walk an AST
            class Analyzer : public Object
            {
            public:
                explicit Analyzer();         //!< initialize
                virtual ~Analyzer() throw(); //!< destructor

                void walk( const Node &root ); //!< default walk

                virtual void onTerminal( const string &name, const string &data ); //!< terminal name and data
                virtual void onInternal( const string &name, const size_t  size ); //!< internal name and children

                int level; //!< internal level

                std::ostream & space( std::ostream &) const; //!< output helper

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Analyzer);
                void __walk( const Node *node );

            };

        }
    }
}

#endif

