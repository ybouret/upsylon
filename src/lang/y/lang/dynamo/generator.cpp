
#include "y/lang/dynamo/generator.hpp"
#include "y/string/convert.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {


        static const char *declKW[] =
        {
            "dynamo"
        };

        DynamoGenerator:: DynamoGenerator() :
        parser(0),
        declH( Y_MPERF_FOR(declKW) ),
        modules(),
        level(0),
        verbose(true)
        {
        }

        DynamoGenerator:: ~DynamoGenerator() throw()
        {
        }

        Syntax::Parser * DynamoGenerator:: build( DynamoNode &top )
        {
            modules.free();
            parser = 0;
            level  = 0;

            decl(top);

            return 0;
        }




        void DynamoGenerator:: declTerminal(const DynamoNode &node)
        {
            Y_LANG_SYNTAX_VERBOSE(node.display(std::cerr << "@gen",level));
            if( parser.is_empty() )
            {
                throw exception("DynamoGenerator: empty parser for terminal <%s>", *node.name);

            }
        }

        void DynamoGenerator:: declInternal( DynamoNode &node)
        {

            DynamoList   &self = node.children();
            const string &name = node.name;

            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "internal <" << node.name << ">/" << self.size << std::endl);

            switch( declH(name) )
            {
                case 0: assert("dynamo"==name);
                    declModule(node);
                    break;

                default:
                    ++level;
                    for(DynamoNode *sub  = self.head; sub; sub=sub->next )
                    {
                        decl(*sub);
                    }
                    --level;
                    break;
            }


        }

        void DynamoGenerator:: decl( DynamoNode &node )
        {
            switch(node.type)
            {
                case DynamoInternal:
                    declInternal(node);
                    break;

                case DynamoTerminal:
                    declTerminal(node);
                    break;
            }
        }



        void DynamoGenerator:: declModule(DynamoNode &dynamo)
        {
            static const char fn[] = "DynamoGenerator.declModule";
            assert("dynamo"==dynamo.name);

            //__________________________________________________________________
            //
            // sanity check
            //__________________________________________________________________
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
            for(DynamoNode *sub  = self.head; sub; sub=sub->next )
            {
                decl(*sub);
            }
            --level;
            modules.pop_back();
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[DONE '" << moduleID << "']" << std::endl);


        }

    }

}

