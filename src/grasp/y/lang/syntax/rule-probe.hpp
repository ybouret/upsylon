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

                explicit RuleProbe() throw();
                virtual ~RuleProbe() throw();

                void reset() throw();

                std::ostream & indent(std::ostream &os, const char *context) const;

                //! check visited
                bool visited( const Rule *r );

                //! do nothing callback example
                static void DoNothing(const Rule *);

                //! start visit from r
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

                //! full recursive visit
                void visitFrom(const Rule *top, const char *context);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(RuleProbe);
            };

        }
    }
}

#endif

