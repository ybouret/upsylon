#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {
        void DynamoGenerator:: fillJK(  Syntax::Compound &parent, DynamoNode *node, const unsigned indx )
        {
            assert(node);
            const string &name      = parent.name;
            const string  jokerName = MakeSubName(parent,indx);
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[" << name << "]<-" << node->name << "=[" << jokerName << "]" << std::endl);

            if(node->type!=DynamoInternal)    throw exception("{%s} unexpected terminal joker in <%s>", **(parser->name),*name);
            DynamoList &ch = node->children();
            if(ch.size!=2)                    throw exception("{%s} mismatching joker size in <%s>", **(parser->name),*name);
            if(ch.tail->type!=DynamoTerminal) throw exception("{%s} mismatching joker type in <%s>", **(parser->name),*name);
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_joker[" << ch.tail->name << "]" << std::endl);

            Syntax::Compound &jokerRule = parser->bundle(jokerName);
            const char        jokerChar = static_cast<const string &>(ch.tail->name)[0];
            delete ch.pop_back(); assert(ch.head);
            fill(jokerRule,ch.head);
            switch( jokerChar )
            {
                case '*': parent << parser->zeroOrMore(jokerRule); break;
                case '+': parent << parser->oneOrMore(jokerRule);  break;
                case '?': parent << parser->optional(jokerRule);   break;
                default : throw exception("{%s} unknown joker type '%c' in <%s>",**(parser->name),jokerChar,*name);
            }
        }
        
    }

}


