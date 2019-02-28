
#include "y/lang/syntax/compound.hpp"
#include "y/exception.hpp"

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

            Compound:: Compound( const uint32_t u, const string &n, bool accept_hollow) :
            Rule(u,n), Operand::List(),
            acceptHollow(accept_hollow)
            {
            }

            void Compound:: append( const Rule &r )
            {
                if(!acceptHollow)
                {
                    if(r.isHollow())
                    {
                        throw exception("'%s' does not accept hollow operand '%s'", *name, *(r.name));
                    }
                }
                push_back( new Operand(r) );
            }

            Compound & Compound:: operator<<( const Rule &r )
            {
                append(r);
                return *this;
            }


        }
    }
}

#include "y/ptr/auto.hpp"
namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Aggregate:: ~Aggregate() throw()
            {
            }

            Aggregate:: Aggregate(const string &id ) :
            Compound(UUID,id,true)
            {
            }

            Aggregate & Aggregate:: operator += ( const Rule &r )
            {
                append(r);
                return *this;
            }

            bool Aggregate:: isHollow() const throw()
            {
                for(const Operand *op = head; op; op=op->next)
                {
                    if( ! op->rule.isHollow() ) return false;
                }
                return true;
            }

            const char * Aggregate:: typeName() const throw()
            {
                return "Aggregate";
            }

            Y_LANG_SYNTAX_ACCEPT_START(Aggregate)
            {
                assert(NULL==tree||tree->internal);

                Node          *subTree = Node::Create(*this);
                auto_ptr<Node> guard(subTree);

                // scan for full acceptance
                for(Operand *op=head;op;op=op->next)
                {
                    if(!op->rule.accept(source, lexer, subTree))
                    {
                        // rejected=>restore
                        guard.dismiss();
                        Node::Unget(subTree,lexer);
                        return false;
                    }
                }

                // all are accepted
                guard.dismiss();
                Node::Grow(tree,subTree);
                return true;
            }

        }

    }

}


namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Alternate:: ~Alternate() throw()
            {
            }

            Alternate:: Alternate(const string &id) :
            Compound(UUID,id,false)
            {
            }

            Alternate & Alternate:: operator |= ( const Rule &r )
            {
                append(r);
                return *this;
            }

            bool Alternate:: isHollow() const throw()
            {
                
                return false;
            }

            const char * Alternate:: typeName() const throw()
            {
                return "Alternate";
            }

            Y_LANG_SYNTAX_ACCEPT_START(Alternate)
            {

                return false;
            }

        }

    }

}

