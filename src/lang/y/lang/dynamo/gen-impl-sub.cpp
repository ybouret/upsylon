#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {
        
        void DynamoGenerator:: fillALT(  Syntax::Compound &parent, DynamoNode *node, const unsigned indx )
        {
            assert(node);
            const string &name     = parent.name;
            const string  altName  = MakeSubName(parent,indx);
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[" << name << "]<-" << node->name << "=[" << altName << "]" << std::endl);

            if(node->type!=DynamoInternal) throw exception("{%s} unexpected terminal alternation in <%s>", **(parser->name),*name);

            Syntax::Compound &altRule = parser->alternate(altName);
            fill(altRule,node->children().head);
            parent << altRule;

        }

        void DynamoGenerator::  fillGRP( Syntax::Compound &parent, DynamoNode *node, const unsigned indx )
        {
            const string &name     = parent.name;
            const string  grpName  = MakeSubName(parent,indx);
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[" << name << "]<-" << node->name << "=[" << grpName << "]" << std::endl);

            if(node->type!=DynamoInternal) throw exception("{%s} unexpected terminal alternation in <%s>", **(parser->name),*name);

            Syntax::Compound &grpRule = parser->bundle(grpName);
            fill(grpRule,node->children().head);
            parent << grpRule;
        }


    }

}


