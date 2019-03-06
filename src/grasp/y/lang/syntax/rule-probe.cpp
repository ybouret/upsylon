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

            bool RuleProbe:: DoNothing(const Rule *)
            {
                //std::cerr << "DoNothing on " << r->name << std::endl;
                return false;
            }

            void RuleProbe:: visitFrom(const Rule *top, const char *context)
            {
                reset();
                if(!visit(top,DoNothing,context))
                {
                    throw exception("Syntax.RuleProbe.visitFrom(Unexpected Bad Visit)");
                }
            }

            namespace
            {
                struct StopOnID
                {
                    const string *pID;

                    inline bool operator()(const Rule *r) throw()
                    {
                        assert(pID);
                        return (r->name==*pID);
                    }

                };
            }

            int RuleProbe:: recursivity(const Rule *r)
            {
                assert(r!=NULL);
                const char *ctx = 0;
                verbose=false;
                StopOnID stop = { &(r->name) };

                switch(r->uuid)
                {
                    case Terminal::UUID:
                        break;

                    case Optional::  UUID:
                    case ZeroOrMore::UUID:
                    case OneOrMore:: UUID:
                        reset();
                        visit( & (r->as<Joker>().jk), stop, ctx);
                        if( search(r->name) )
                        {
                            return 0;
                        }
                        break;

                    case Alternate::UUID:
                    case Aggregate::UUID: {
                        int i = 0;
                        for(const Operand *op = r->as<Compound>().head;op;op=op->next,++i)
                        {
                            reset();
                            visit( &(op->rule), stop, ctx);
                            if( search(r->name) )
                            {
                                return i;
                            }
                        }
                    } break;

                    default:
                        throwUUID("Syntax.RuleProbe.recursivity",r);
                }

                return -1;
            }


        }

    }
}


