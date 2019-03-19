#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {

        void DynamoGenerator:: fillRID( Syntax::Compound &parent, DynamoNode *node )
        {
            assert(node);
            const string symbolName = getRID(node,"symbol name");
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[" << parent.name << "]<-" << node->name << "=[" << symbolName << "]" << std::endl);
            parent << findSymbol(symbolName);
        }
        
    }

}


