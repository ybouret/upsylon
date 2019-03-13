
#include "y/lang/syntax/joker.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            
            Joker:: ~Joker() throw()
            {
            }

            Joker:: Joker(const uint32_t t, const string &id, const Rule &jk ):
            Rule(t,id),
            rule(jk)
            {
                data = &jk;
            }
            

            Optional:: ~Optional() throw()
            {
            }

            Optional:: Optional( const string &id, const Rule &jk ) :
            Joker(UUID,id,jk)
            {
            }

            bool Optional:: accept( Y_LANG_SYNTAX_RULE_ARGS ) const
            {
                if(verbose) { display(std::cerr,depth,'?') << std::endl; }
                Node *node = NULL;
                if( rule.accept(node,lexer,source,depth) )
                {
                    //std::cerr << "...accepted optional " << rule.name << std::endl;
                    Grow(tree,node);
                    --depth;
                    if(verbose) { display(std::cerr,depth,'+') << "/found" << std::endl; }
                }
                else
                {
                    --depth;
                    if(verbose) { display(std::cerr,depth,'+') << "/empty" << std::endl; }
                }
                return true;
            }

            bool Optional:: hollow() const throw()
            {
                return true;
            }

            void Joker:: epilog( ios::ostream &fp ) const
            {
                viz_id(fp); fp << " -> "; rule.viz_id(fp); fp << "[arrowhead=diamond];\n";
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

            Repeating:: Repeating(const string &id, const Rule &jk,const size_t n) :
            Joker(UUID,id,jk),
            nmin(n)
            {
            }

            Repeating:: ~Repeating() throw()
            {
            }
            
            bool Repeating:: accept( Y_LANG_SYNTAX_RULE_ARGS ) const
            {
                if(verbose) { display(std::cerr,depth,'?') << std::endl; }
                ++depth;

                Node  *subTree = Node::Create(*this);
                size_t count   = 0;
                {
                    auto_ptr<Node> guard(subTree);
                    while(true)
                    {
                        Node *node = 0;
                        if( rule.accept(node,lexer,source,depth) )
                        {
                            Grow(subTree,node);
                            ++count;
                        }
                        else
                        {
                            break;
                        }
                    }
                    guard.dismiss();
                }

                if(count>=nmin)
                {
                    //success: transfer data
                    if(tree)
                    {
                        assert(tree->internal);
                        Node::List &ch = subTree->children;
                        while(ch.size)
                        {
                            tree->add( ch.pop_front() );
                        }
                        delete subTree;
                    }
                    else
                    {
                        tree = subTree;
                    }
                    --depth;
                    if(verbose)
                    {
                        display(std::cerr,depth,'+') << '/';
                        if(count>0) std::cerr << "found"; else std::cerr << "empty";
                        std::cerr << std::endl;
                    }
                    return true;
                }
                else
                {
                    //failure
                    Node::BackTo(lexer,subTree);
                    --depth;
                    if(verbose) { display(std::cerr,depth,'-') << std::endl; }
                    return false;
                }

            }

            bool Repeating:: hollow() const throw()
            {
                return (nmin<=0);
            }

        }
    }
}

