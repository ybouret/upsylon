
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

            Compound:: Compound( const uint32_t u, const string &n, const Behavior b, bool accept_hollow) :
            Internal(u,n),
            Operand::List(),
            behavior(b),
            acceptHollow(accept_hollow)
            {
                derived = static_cast<Compound*>(this);
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

            void Compound:: graphVizEpilog(ios::ostream &fp) const
            {
                unsigned idx=0;
                if(size>1) ++idx;
                for(const Operand *op=head;op;op=op->next)
                {
                    graphVizLink(fp, &(op->rule), idx++);
                }
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
            Compound(UUID,id,Group,true)
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
                Y_LANG_SYNTAX_VERBOSE("|_" << typeName() << " '" << name << "' #operands=" << size << std::endl);

                // scan for full acceptance
                for(Operand *op=head;op;op=op->next)
                {
                    if(!op->rule.accept(source, lexer, subTree))
                    {
                        // rejected=>restore
                        Y_LANG_SYNTAX_VERBOSE("|_rejected " << typeName() << " '" << name << "'" << std::endl);
                        guard.dismiss();
                        //std::cerr << "*** Unget..." << std::endl;
                        Node::Unget(subTree,lexer);
                        //std::cerr << "*** done" << std::endl;
                        return false;
                    }
                }

                // all are accepted
                guard.dismiss();
                Node::Grow(tree,subTree);
                Y_LANG_SYNTAX_VERBOSE("|_accepted " << typeName() << " '" << name << "'" << std::endl);
                return true;
            }

            const char * Aggregate:: graphVizShape() const throw()
            {
                return "house";
            }

            const char * Aggregate:: graphVizStyle() const throw()
            {
                switch (behavior)
                {
                    case Group: return "bold,filled";
                    case Merge: return "bold,dashed";
                    default:
                        break;
                }
                return "solid";
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
            Compound(UUID,id,Merge,false)
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
                assert(NULL==tree||tree->internal);
                Y_LANG_SYNTAX_VERBOSE("|_" << typeName() << " '" << name << "' #operands=" << size << std::endl);
                // scan for ONE acceptance
                for(Operand *op=head;op;op=op->next)
                {
                    Node *leaf = 0;
                    if(op->rule.accept(source, lexer, leaf))
                    {
                        Node::Grow(tree,leaf);
                        Y_LANG_SYNTAX_VERBOSE("|_accepted " << typeName() << " '" << name << "'" << std::endl);
                        return true;
                    }
                }
                Y_LANG_SYNTAX_VERBOSE("|_rejected " << typeName() << " '" << name << "'" << std::endl);
                return false;
            }

            const char * Alternate:: graphVizShape() const throw()
            {
                return "egg";
            }


        }

    }

}

