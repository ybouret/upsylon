//! \file
#ifndef Y_LANG_SYNTAX_RULE_PROBE_INCLUDED
#define Y_LANG_SYNTAX_RULE_PROBE_INCLUDED 1

#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/joker.hpp"
#include "y/lang/syntax/compound.hpp"
#include "y/lang/syntax/rrs.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            //! probing rules
            class RuleProbe : public RuleReferenceSet
            {
            public:
                int              depth;      //!< call depth
                bool             verbose;    //!< verbose flag

                explicit RuleProbe() throw(); //!< setup
                virtual ~RuleProbe() throw(); //!< destruct

                void reset() throw(); //!< free and depth=0

                //! indent on verbose flag
                std::ostream & indent(std::ostream &, const char *) const;

                //! check visited
                bool visited( const Rule *r );

                //! do nothing callback example
                static bool DoNothing(const Rule *);

                //! start visit from r
                template <typename CALLBACK>
                inline bool visit(const Rule *r,
                                  CALLBACK   &stop,
                                  const char *context)
                {
                    assert(NULL!=r);
                    if(visited(r)) return true;

                    try
                    {
                        Y_LANG_SYNTAX_VERBOSE( indent(std::cerr,context) << r->typeName() << ' ' << '<' << r->name  << '>' << std::endl );
                        if(stop(r)) return false;

                        switch(r->uuid)
                        {
                            case Terminal::UUID:
                                break;

                            case Aggregate:: UUID:
                            case Alternate:: UUID:
                                assert(r->derived);

                                ++depth;
                                for(const Operand *op = r->as<Compound>().head;op;op=op->next)
                                {
                                    if(!visit<CALLBACK>( &(op->rule), stop, context))
                                    {
                                        return false;
                                    }
                                }
                                --depth;
                                break;

                            case Optional  ::UUID:
                            case OneOrMore ::UUID:
                            case ZeroOrMore::UUID:
                                assert(r->derived);

                                ++depth;
                                if(!visit<CALLBACK>( &(r->as<Joker>().jk),stop,context ))
                                {
                                    return false;
                                }
                                --depth;
                                break;

                            default:
                                throwUUID(VisitFunction,r);
                        }
                        return true;
                    }
                    catch(...)
                    {
                        reset();
                        throw;
                    }
                }

                //! full recursive visit
                void visitFrom(const Rule *top, const char *context);

                //! finding recursivity index and depth
                int recursivity(const Rule *top);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(RuleProbe);
                static const char VisitFunction[];
                void throwUUID( const char *fn, const Rule *r) const;

            };

        }
    }
}

#endif

