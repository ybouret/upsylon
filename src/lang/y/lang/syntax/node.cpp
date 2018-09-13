#include "y/lang/syntax/node.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            Node:: List:: ~List() throw()
            {

            }

            Node:: List:: List() throw() : Object(), ListType()
            {
            }
            

            Node:: ~Node() throw() {}


            Node:: Node( const Rule &r, Lexeme *lx ) throw() :
            next(0), prev(0),
            terminal(true), internal(false),
            impl(lx),
            lexeme(   *static_cast<Lexeme *>(impl) ),
            children( *static_cast<List   *>(impl) ),
            rule(r)
            {
                assert(lx);
            }

            Node:: Node( const Rule &r ) throw() :
            next(0), prev(0),
            terminal(false), internal(true),
            impl( new List() ),
            lexeme(   *static_cast<Lexeme *>(impl) ),
            children( *static_cast<List   *>(impl) ),
            rule(r)
            {
            }

        }
    }
}

