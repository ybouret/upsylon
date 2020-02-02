
#include "y/lang/syntax/analyzer.hpp"
#include "y/lang/syntax/rule.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            Analyzer:: ~Analyzer() throw()
            {
                
            }

            Analyzer:: Analyzer() throw() : depth(0)
            {
                
            }

            void Analyzer:: walk(const Node &node)
            {
                depth = 0;
                walkDown(node);
            }

            void Analyzer:: walkDown(const Node &node )
            {
                if(node.terminal)
                {
                    onTerminal(node.rule.name,node.lexeme());
                }
                else
                {
                    const Node::List &ch  = node.children();
                    ++depth;
                    for(const Node   *sub = ch.head; sub; sub=sub->next )
                    {
                        walkDown(*sub);
                    }
                    --depth;
                    onInternal(node.rule.name,ch.size,node.data());
                }
            }

            std::ostream & Analyzer:: indent( std::ostream &os ) const
            {
                const string lvl = vformat("x%02x",unsigned(depth));
                os << '|' << lvl << '|';
                for(int i=depth;i>0;--i)
                {
                    os << '-' << '-';
                }
                return os;
            }


            void Analyzer:: onTerminal(const string &id, const Lexeme &lx)
            {
                indent(std::cerr) << "[push] <" << id << ">";
                if(lx.size) {
                    const string s = lx.toVisible();
                    std::cerr << ' ' << '=' << ' ' << '\'' << s << '\'';
                }
                std::cerr << std::endl;
            }

            void Analyzer:: onInternal(const string &id, const size_t sz, const string *data)
            {
                indent(std::cerr) << "[call] <" << id << ">/" << sz;
                if(data)
                {
                    std::cerr << " $'" << *data << "'";
                }
                std::cerr << std::endl;
            }
        }
    }
}

