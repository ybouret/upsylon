
#include "y/jive/syntax/analyzer.hpp"
#include "y/jive/syntax/axiom.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {

            Analyzer:: ~Analyzer() throw()
            {
                aliasing::_(depth) = 0;
            }


            int Analyzer:: onTerminal(const string &id, const Lexeme &lexeme)
            {
                indent(std::cerr << "[push]:" ) << id;
                if(lexeme.size)
                {
                    std::cerr << " = '" << lexeme << "'";
                }
                else
                {
                    std::cerr << "     ";
                }
                for(size_t i=lexeme.size;i<=maxLength;++i) std::cerr << ' ';
                std::cerr << " |#" << lexeme.stamp << "|" << std::endl;
                return 0;
            }

            int Analyzer:: onInternal(const string &id, const size_t size)
            {
                indent(std::cerr << "[call]:" ) << id << "[" << size << "]" << std::endl;
                return 0;
            }

            int Analyzer:: walkDownInternal(const Node *node)
            {
                ++aliasing::_(depth);
                const Node::List &leaves = node->leaves();
                for(const Node *child=leaves.head;child;child=child->next)
                {
                    const int res = walkDown(child);
                    if(res!=0) return res;
                }
                --aliasing::_(depth);
                return onInternal(*(node->axiom.name),leaves.size);
            }

            int Analyzer:: walkDown(const Node *node)
            {
                assert(node);
                switch(node->kind)
                {
                    case Node::IsTerminal: return onTerminal(*(node->axiom.name),* (node->lexeme()));
                    case Node::IsInternal: return walkDownInternal(node);
                }
                return -1;
            }

            int Analyzer:: walk(const Node *node)
            {
                assert(node);
                aliasing::_(depth) = 0;
                return walkDown(node);
            }


        }

    }
}

