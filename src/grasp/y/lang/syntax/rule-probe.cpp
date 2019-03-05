#include "y/lang/syntax/rule-probe.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            const char RuleProbe:: VisitFunction[] = "Syntax.RuleProbe.visit";

            void RuleProbe:: throwUUID(const char *fn, const Rule *r) const
            {
                assert(fn);
                assert(r);
                throw exception("%s(unexpected  UUID [%04x] for <%s>)", fn, unsigned(r->uuid), *(r->name) );
            }

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

            int RuleProbe:: recursivity(const Rule *r)
            {
                assert(r!=NULL);
                reset();

                switch(r->uuid)
                {
                    case Terminal::UUID:
                        break;

                    case Optional::  UUID:
                    case ZeroOrMore::UUID:
                    case OneOrMore:: UUID:
                        visit( & (r->as<Joker>().jk), DoNothing, NULL);
                        if( search(r->name) ) return 0;
                        break;

                    default:
                        throwUUID("Syntax.RuleProbe.recursivity",r);
                }

                return -1;
            }


        }

    }
}


