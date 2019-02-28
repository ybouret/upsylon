
#include "y/lang/syntax/compound.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            Operand:: ~Operand() throw()
            {
            }

            Operand:: Operand( const Rule &r ) throw() :
            Object(), Base(), rule(r)
            {

            }

            Compound:: ~Compound() throw() {}

            Compound:: Compound( const uint32_t u, const string &n ) :
            Rule(u,n), Operand::List()
            {
            }
            

        }
    }
}

