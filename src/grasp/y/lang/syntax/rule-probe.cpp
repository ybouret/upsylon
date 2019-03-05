#include "y/lang/syntax/rule-probe.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            RuleProbe:: RuleProbe()   throw() : RuleReferenceSet( ), depth(0), verbose(false) {}

            RuleProbe:: ~RuleProbe() throw()
            {
            }

            void RuleProbe:: reset() throw()
            {
                free();
                depth = 0;
            }

            std::ostream & RuleProbe:: indent(std::ostream &os, const char *context) const
            {
                os << '|';
                if(context)
                {
                    os << '{' << context << '}' << '.';
                }
                for(int i=depth;i>0;--i)
                {
                    os << '.' << '.';
                }
                return os;
            }

            bool RuleProbe:: visited( const Rule *r ) { return !declare(r); }

            void RuleProbe:: DoNothing(const Rule *) { }

            void RuleProbe:: visitFrom(const Rule *top, const char *context)
            {
                reset();
                visit(top,DoNothing,context);
            }

        }

    }
}


