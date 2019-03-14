#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {


        void DynamoGenerator:: storeDecl( Syntax::Terminal &t )
        {
            assert( parser.is_valid() );
            assert( modules.size()>0);

            const DynamoTerm symb(modules.back(),t);
            if( !symbols.insert(symb) || !terminals.insert(symb))
            {
                throw exception("{%s} multiple terminal <%s>",**(parser->name), *(t.name));
            }
        }

        
        void DynamoGenerator:: declModule(DynamoNode &dynamo)
        {
            static const char fn[] = "DynamoGenerator.declModule";

            //__________________________________________________________________
            //
            // sanity check
            //__________________________________________________________________
            if(dynamo.name!="dynamo")       throw exception("%s(expecting <dynamo> and not <%s>)",fn,*(dynamo.name));
            if(dynamo.type!=DynamoInternal) throw exception("%s(<dynamo> is not internal)",fn);

            DynamoList   &self = dynamo.children();
            if(self.size<=0)              throw exception("%s(Empty <dynamo> node)",fn);
            if(self.head->name!="module") throw exception("%s(Invalid Module Name='%s')",fn,*(self.head->name));

            //__________________________________________________________________
            //
            // build a tag
            //__________________________________________________________________
            const Tag moduleID = new string( self.head->content() );
            modules.push_back(moduleID);
            delete self.pop_front();

            //__________________________________________________________________
            //
            // and declare the new module
            //__________________________________________________________________
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[DECL '" << moduleID << "']" << std::endl);

            if(1==modules.size())
            {
                //______________________________________________________________
                //
                // would create parser
                //______________________________________________________________
                assert( parser.is_empty() );
                parser = new Syntax::Parser( *moduleID );
            }

            //__________________________________________________________________
            //
            // decl for sub items
            //__________________________________________________________________
            ++level;
            DynamoList temp;
            while(self.size)
            {
                auto_ptr<DynamoNode> sub  = self.pop_front();
                const string         id   = sub->name;
                bool                 keep = true;
                switch( declH(id) )
                {
                    case 0: assert("dynamo"==id); declModule(*sub);              break;
                    case 1: assert("aka"==id);    declAlias(*sub);   keep=false; break;
                    case 2: assert("plg"==id);    declPlugin(*sub);  keep=false; break;
                    case 3: assert("lxr"==id);    declLexical(*sub); keep=false; break;
                    case 4: assert("eol"==id);    declEOL(*sub);     keep=false; break;

                    default:
                        Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[unprocessed '" << id << "']" << std::endl);
                        break;
                }

                if(keep) temp.push_back( sub.yield() );
            }
            --level;
            temp.swap_with(self);
            modules.pop_back();
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[DONE '" << moduleID << "']" << std::endl);
        }


      

    }

}
