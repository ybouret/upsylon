//! \file
#ifndef Y_LANG_SYNTAX_RULE_PROBE_INCLUDED
#define Y_LANG_SYNTAX_RULE_PROBE_INCLUDED 1

#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/joker.hpp"
#include "y/lang/syntax/compound.hpp"
#include "y/lang/syntax/rrs.hpp"
#include "y/exception.hpp"
namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            class RuleProbe : public RuleReferenceSet
            {
            public:
                int              depth;
                bool             verbose;

                explicit RuleProbe()   throw() : RuleReferenceSet( ), depth(0), verbose(false) {}

                virtual ~RuleProbe() throw() {}

                inline void reset() throw()
                {
                    free();
                    depth = 0;
                }

                inline std::ostream & indent(std::ostream &os, const char *context) const
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

                //! check visited
                inline bool visited( const Rule *r ) { return !declare(r); }

                //! do nothing callback example
                inline static void DoNothing(const Rule *) { }

                //! ok
                template <typename CALLBACK>
                inline void visit(const Rule *r,
                                  CALLBACK   &proc,
                                  const char *context)
                {
                    assert(NULL!=r);
                    if(visited(r)) return;

                    try
                    {
                        Y_LANG_SYNTAX_VERBOSE( indent(std::cerr,context) << r->typeName() << ' ' << '<' << r->name  << '>' << std::endl );
                        switch(r->uuid)
                        {
                            case Terminal::UUID:
                                proc(r);
                                break;

                            case Aggregate:: UUID:
                            case Alternate:: UUID:
                                assert(r->derived);

                                ++depth;
                                for(const Operand *op = r->as<Compound>().head;op;op=op->next)
                                {
                                    visit( &(op->rule), proc, context);
                                }
                                --depth;
                                break;

                            case Optional  ::UUID:
                            case OneOrMore ::UUID:
                            case ZeroOrMore::UUID:
                                assert(r->derived);

                                ++depth;
                                visit( &(r->as<Joker>().jk), proc,context );
                                --depth;
                                break;

                            default:
                                throw exception("Syntax::RuleProbe: unexpected  UUID [%04x] for <%s>",unsigned(r->uuid), *(r->name) );
                        }
                    }
                    catch(...)
                    {
                        reset();
                        throw;
                    }
                }

                inline void visit_from(const Rule *top, const char *context)
                {
                    reset();
                    visit(top,DoNothing,context);
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(RuleProbe);
            };

        }
    }
}

#endif

