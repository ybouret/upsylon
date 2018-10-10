
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

            Analyzer:: Analyzer() throw():
            depth(0),
            aligned(20)
            {
            }

            std::ostream & Analyzer:: space( std::ostream &os ) const
            {
                const string lvl = vformat("%02d|",depth);
                os << lvl;
                for(int i=0;i<2*depth;++i) os << '_';
                return os;
            }

            void Analyzer:: __walk(const Node *node )
            {
                const string &name = node->rule.name;
                if(node->terminal)
                {
                    //const string data = node->lexeme.to_string();
                    onTerminal(node->lexeme);
                }
                else
                {
                    ++depth;
                    for(const Node *sub=node->children.head;sub;sub=sub->next)
                    {
                        __walk(sub);
                    }
                    --depth;
                    onInternal(name,node->children.size);
                }

            }

            void Analyzer:: walk( const Node &root )
            {
                depth = 0;
                __walk( &root );
            }

            void Analyzer:: onTerminal(const Lexeme &data)
            {
                space(std::cerr) << "push " << data.label << " ";
                if(data.size)
                {
                    for(size_t i=data.label->size()+depth*2;i<=aligned;++i) std::cerr << ' ';
                    std::cerr << "<" << data << ">";
                }
                std::cerr << std::endl;
            }


            void Analyzer:: onInternal(const string &name, const size_t size)
            {
                space(std::cerr) << "call <" << name << ">/" << size << std::endl;
            }
        }
    }
}
