
#include "y/lang/dynamo/compiler.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {


        void DynamoCompiler:: decl(XNode &node)
        {
            const string id = node.rule.name;
            std::cerr << "decl: " << id << std::endl;
            const int h = top_h(id);
            std::cerr << "h=" << h << std::endl;
            switch(h)
            {
                case 0: assert("dynamo"==id); {
                    declModule(node);
                }  break;

                case 1: assert("rule"==id); {
                    declRule(node);
                } break;

                default:
                    break;
            }
        }

        string * DynamoCompiler:: getModuleName( const XNode *node ) const
        {
            assert(node);
            static const char fn[] = "DynamoCompiler::getModuleName";
            const  string    &id   = node->rule.name;
            if("module" != id)  throw exception("%s(<%s>!=<module>)",    fn,*id);
            if(!node->terminal) throw exception("%s(internal node <%s>)",fn,*id);
            const string s = node->lexeme().to_string(1,0);
            return new string(s);
        }

        void DynamoCompiler:: declModule(XNode &node)
        {
            static const char fn[] = "DynamoCompiler::declModule";
            assert("dynamo"==node.rule.name);
            if(!node.internal) throw exception("%s(terminal <dynamo>)", fn );
            XNode::List &self = node.children();
            if(self.size<=0)   throw exception("%s(empty node <dynamo>)", fn);
            XNode       *sub  = self.head;       assert(sub);
            const Tag    mid  = getModuleName(sub);
            std::cerr << "ENTER: moduleID=" << mid << std::endl;
            modules.push_back(mid);
            for(sub=sub->next;sub;sub=sub->next)
            {
                decl(*sub);
            }
            modules.pop_back();
            std::cerr << "LEAVE: moduleID=" << mid << std::endl;
        }

        void DynamoCompiler:: declRule(XNode &node)
        {
            static const char fn[] = "DynamoCompiler::declRule";
            assert(node.rule.name=="rule");
            if(!node.internal) throw exception("%s(terminal <rule>)", fn );
            XNode::List &self = node.children();
            if(self.size<=0)   throw exception("%s(empty node <rule>",fn);
            
        }

    }

}
