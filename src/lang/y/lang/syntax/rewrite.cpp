
#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/compound.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

#if 0
            static inline
            bool is_valid_operator( const Terminal *term, const char *g )
            {
                assert(term);
                if(term->isOperator)
                {
                    switch (term->attr)
                    {
                        case Terminal::Standard:
                            throw exception("{%s} standard terminal %s declared as operator, should be univocal",g, *(term->name));

                        case Terminal::Univocal:
                            //std::cerr << "..new parent from " << term->name << std::endl;
                            break;

                        case Terminal::Semantic:
                            throw exception("{%s} unexpected semantic terminal %s declared as operator",g, *(term->name));
                    }
                    return true;
                }
                else
                {
                    return false;
                }

            }

            Node *Node:: Rewrite(Node *node, const string &g)
            {
                assert(node);

                if( node->terminal )
                {
                    //__________________________________________________________
                    //
                    // terminal, do not touch
                    //__________________________________________________________
                    return node;
                }
                else
                {
                    //__________________________________________________________
                    //
                    // recursively go over children
                    //__________________________________________________________
                    Node::List &source = node->children;
                    Node::List  target;
                    try
                    {

                        while(source.size)
                        {
                            auto_ptr<Node> child = 0;
                            {
                                auto_ptr<Node> guard( source.pop_front() );
                                child  = Rewrite(&*guard,g);
                                guard.dismiss();
                            }
                            const Rule     &rule  = child->rule;
                            if(Terminal::UUID == rule.uuid && is_valid_operator(static_cast<const Terminal *>(rule.data),*g) )
                            {
                                auto_ptr<Node> parent = Node::Create(rule); // a funky terminal ID with content!

                                //______________________________________________
                                //
                                // get LHS operator
                                //______________________________________________
                                if(target.tail)
                                {
                                    parent->children.push_front(target.pop_back());
                                }

                                //______________________________________________
                                //
                                // get RHS operator
                                //______________________________________________
                                if(source.head)
                                {
                                    auto_ptr<Node> guard( source.pop_front() );
                                    parent->children.push_back( Rewrite(&*guard,g) );
                                    guard.dismiss();
                                }

                                //______________________________________________
                                //
                                // and replace
                                //______________________________________________
                                target << parent.yield();
                            }
                            else
                            {
                                target << child.yield();
                            }

                        }

                        source.swap_with(target);
                    }
                    catch(...)
                    {
                        while(target.size) delete target.pop_back();
                        throw;
                    }

                    return node;
                }
            }
#endif
            
        }

    }

}

