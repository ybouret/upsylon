
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

            Operand:: Operand(const Rule &r)  throw(): rule(r), next(0), prev(0) {}

            Operand::List::  List() throw() : ListType() {}
            Operand::List:: ~List() throw()  {}

            void Operand::List:: add(const Rule &r)
            {
                push_back( new Operand(r) );
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
            Compound:: ~Compound() throw()
            {
            }

            Compound:: Compound( const uint32_t t, const string &id ) :
            Rule(t,id),
            Operand::List()
            {
                Compound *self = this;
                data = self;
            }

            void Compound:: epilog( ios::ostream &fp ) const
            {
                unsigned i=0;
                for(const Operand *op=head;op;op=op->next)
                {
                    ++i;
                    viz_id(fp); fp << " -> "; op->rule.viz_id(fp); fp(" [label=\"%u\"];\n",i);
                }
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
            Aggregate:: ~Aggregate() throw()
            {
            }

            Aggregate:: Aggregate(  const string &id ) :
            Compound(UUID,id)
            {
            }


            bool Aggregate:: accept( Y_LANG_SYNTAX_RULE_ARGS ) const
            {
                Node * subTree = Node::Create(*this);

                // try to accept every operand
                {
                    auto_ptr<Node> guard(subTree);
                    for(const Operand *op=head;op;op=op->next)
                    {
                        if(!op->rule.accept(subTree, lexer, source) )
                        {
                            guard.dismiss();
                            Node::BackTo(lexer, subTree);
                            return false;
                        }
                    }
                    guard.dismiss();
                }

                // success, subTree may be empty but never NULL!
                Grow(tree,subTree);
                return true;
            }

            bool Aggregate:: hollow() const throw()
            {
                return false;
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

            Alternate:: Alternate(  const string &id ) :
            Compound(UUID,id)
            {
            }


            bool Alternate:: accept( Y_LANG_SYNTAX_RULE_ARGS ) const
            {
                for(const Operand *op=head;op;op=op->next)
                {
                    Node *tmp = NULL;
                    if(op->rule.accept(tmp, lexer, source) )
                    {
                        // success
                        if(tmp) Grow(tree,tmp);
                        return true;
                    }
                }

                return false;
            }

            bool Alternate:: hollow() const throw()
            {
                for(const Operand *op=head;op;op=op->next)
                {
                    if(op->rule.hollow()) return true;
                }
                return false;
            }
        }

    }

}

