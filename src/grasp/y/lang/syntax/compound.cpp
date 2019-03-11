
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

            Compound:: Compound( const uint32_t u, const string &n, const Behavior b, const HollowPolicy p) :
            Internal(u,n,typeid(Compound)),
            Operand::List(),
            behavior(b),
            hollowPolicy(p)
            {
                derived = static_cast<Compound*>(this);
            }

            void Compound:: append( const Rule &r )
            {
                switch(hollowPolicy)
                {
                    case AcceptHollowOperand:
                        break;

                    case RejectHollowOperand:
                        if(r.isHollow())
                        {
                            throw exception("'%s' does not accept hollow operand '%s'", *name, *(r.name));
                        }
                        break;
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
            Compound(UUID,id,SubGroup,AcceptHollowOperand)
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
                return "Aggregate ";
            }

            Y_LANG_SYNTAX_ACCEPT_START(Aggregate)
            {
                assert(NULL==tree||tree->internal);

                Node          *subTree = Node::Create(*this);
                auto_ptr<Node> guard(subTree);
                Y_LANG_SYNTAX_VERBOSE(std::cerr << "|_" << typeName() << " '" << name << "' #operands=" << size << std::endl);

                // scan for full acceptance
                for(Operand *op=head;op;op=op->next)
                {
                    if(!op->rule.accept(source, lexer, subTree))
                    {
                        // rejected=>restore
                        Y_LANG_SYNTAX_VERBOSE(std::cerr << "|_rejected " << typeName() << " '" << name << "'" << std::endl);
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
                Y_LANG_SYNTAX_VERBOSE(std::cerr << "|_accepted " << typeName() << " '" << name << "'" << std::endl);
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
                    case SubGroup: return "bold,filled";
                    case MergeOne: return "bold,dashed,filled";
                    case MergeAll: return "bold,dashed";
                    default:
                        break;
                }
                return "solid";
            }

            Aggregate & Aggregate:: design()  throw() { (Behavior &)behavior = MergeOne; return *this; }
            Aggregate & Aggregate:: bundle()  throw() { (Behavior &)behavior = MergeAll; return *this; }


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
            Compound(UUID,id,MergeAll,RejectHollowOperand)
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
                return "Alternate ";
            }

            Y_LANG_SYNTAX_ACCEPT_START(Alternate)
            {
                assert(NULL==tree||tree->internal);
                Y_LANG_SYNTAX_VERBOSE(std::cerr << "|_" << typeName() << " '" << name << "' #operands=" << size << std::endl);
                // scan for ONE acceptance
                for(Operand *op=head;op;op=op->next)
                {
                    Node *leaf = 0;
                    if(op->rule.accept(source, lexer, leaf))
                    {
                        Node::Grow(tree,leaf);
                        Y_LANG_SYNTAX_VERBOSE(std::cerr << "|_accepted " << typeName() << " '" << name << "'" << std::endl);
                        return true;
                    }
                }
                Y_LANG_SYNTAX_VERBOSE(std::cerr << "|_rejected " << typeName() << " '" << name << "'" << std::endl);
                return false;
            }

            const char * Alternate:: graphVizShape() const throw()
            {
                return "egg";
            }


        }

    }

}

#if 0
#include "y/lang/syntax/terminal.hpp"

namespace upsylon
{

    namespace Lang
    {

        namespace Syntax
        {

            void Aggregate:: autoUpgrade() throw()
            {
                // std::cerr << "*** upgrading <" << name << ">" << std::endl;
                if(SubGroup==behavior)
                {
                    size_t count = 0;
                    for(const Operand *op=head;op;op=op->next)
                    {
                        if(op->rule.uuid == Terminal::UUID)
                        {
                            assert(op->rule.derived);
                            const Terminal &t = *static_cast<const Terminal *>(op->rule.derived);
                            if(Semantic==t.attr) continue;
                        }
                        ++count;
                    }
                    //std::cerr << "*** count=" << count << std::endl;
                    if(1==count)
                    {
                        (void) design();
                    }
                }
            }

        }
    }
}
#endif
