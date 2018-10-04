
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

            Compound:: Compound( const uint32_t t, const string &id, const Type flag) :
            Rule(t,id),
            Operand::List(),
            type(flag)
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
                    viz_id(fp); fp << " -> "; op->rule.viz_id(fp);
                    if(size>1)
                    {
                        fp(" [label=\"%u\"];\n",i);
                    }
                    else
                    {
                        fp(";\n");
                    }
                }
            }

            void Compound:: setActing() throw()
            {
                assert(Normal==type);
                (Type &)type = Acting;
            }


            void Compound:: setDesign() throw()
            {
                assert(Normal==type);
                (Type &)type = Design;
            }

            bool Rule:: isCompound() const throw()
            {
                return Aggregate::UUID == uuid || Alternate::UUID == uuid;
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

            Aggregate:: Aggregate(  const string &id, const Type flag) :
            Compound(UUID,id,flag)
            {
            }


            bool Aggregate:: accept( Y_LANG_SYNTAX_RULE_ARGS ) const
            {
                Node * subTree = Node::Create(*this);
                if(verbose) { display(std::cerr,depth,'?') << std::endl; }
                //______________________________________________________________
                //
                // try to accept every operand
                //______________________________________________________________
                {
                    ++depth;
                    auto_ptr<Node> guard(subTree);
                    for(const Operand *op=head;op;op=op->next)
                    {
                        if(!op->rule.accept(subTree, lexer, source, depth) )
                        {
                            guard.dismiss();
                            Node::BackTo(lexer, subTree);
                            --depth;
                            if(verbose) { display(std::cerr,depth,'-') << std::endl; }
                            return false;
                        }
                    }
                    guard.dismiss();
                }

                // success, subTree may be empty but never NULL!
                Grow(tree,subTree);
                //std::cerr << "..accepted " << name << std::endl;
                --depth;
                if(verbose) { display(std::cerr,depth,'+') << std::endl; }
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
            Compound(UUID,id,Acting)
            {
            }


            bool Alternate:: accept( Y_LANG_SYNTAX_RULE_ARGS ) const
            {
                if(verbose) { display(std::cerr,depth,'?') << std::endl; }
                ++depth;

                for(const Operand *op=head;op;op=op->next)
                {
                    Node *tmp = NULL;
                    if(op->rule.accept(tmp, lexer, source, depth) )
                    {
                        // success
                        if(tmp) Grow(tree,tmp);
                        //std::cerr << "..accepted " << op->rule.name << " in " << name << std::endl;
                        --depth;
                        if(verbose) { display(std::cerr,depth,'+') << std::endl; }
                        return true;
                    }
                }

                --depth;
                if(verbose) { display(std::cerr,depth,'-') << std::endl; }
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

