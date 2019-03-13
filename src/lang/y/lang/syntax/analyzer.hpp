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

                //! called when a terminal is met
                virtual void onTerminal( const string &id, const Lexeme &lx );

                //! called when an internal is met
                virtual void onInternal( const string &id, const size_t  sz, const string *data);


                void walk( const Node &node );     //!< initialize depth and walkDown( node )
                void walkDown( const Node &node ); //!< walk from this node

                int depth; //!< current depth

                std::ostream & indent( std::ostream & ) const; //!< indentation helper

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Analyzer);
            };



        }
    }
}

#endif

