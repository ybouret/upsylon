#include "y/lang/dynamo/generator.hpp"

namespace upsylon
{

    namespace Lang
    {
        void DynamoGenerator:: walkDown( const Node *node )
        {
            assert(node);
            if(node->terminal)
            {
                if(verbose) { indent() << "@term " << node->rule.name << std::endl; }
            }
            else
            {
                if(verbose) { indent() << "@walk " << node->rule.name << std::endl; }
                ++level;
                for(const Node *child = node->children.head; child; child=child->next )
                {
                    walkDown(child);
                }
                --level;
            }

        }

    }

}
