
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

            Analyzer:: Analyzer() :
            level(0)
            {
            }

            std::ostream & Analyzer:: space( std::ostream &os ) const
            {
                const string lvl = vformat("<%02d>",level);
                os << lvl;
                for(int i=0;i<level;++i) os << ' ' << ' ';
                return os;
            }

            void Analyzer:: __walk(const Node *node )
            {
                const string &name = node->rule.name;
                if(node->terminal)
                {
                    const string data = node->lexeme.to_string();
                    onTerminal(name,data);
                }
                else
                {
                    ++level;
                    for(const Node *sub=node->children.head;sub;sub=sub->next)
                    {
                        __walk(sub);
                    }
                    --level;
                    onInternal(name,node->children.size);
                }

            }

            void Analyzer:: walk( const Node &root )
            {
                level = 0;
                __walk( &root );
            }

            void Analyzer:: onTerminal(const string &name, const string &data)
            {
                space(std::cerr) << "push[" << name << "]='" << data << "'" << std::endl;
            }


            void Analyzer:: onInternal(const string &name, const size_t size)
            {
                space(std::cerr) << "call[" << name << "]/" << size << std::endl;
            }
        }
    }
}
