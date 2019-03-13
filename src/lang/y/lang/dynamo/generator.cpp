
#include "y/lang/dynamo/generator.hpp"
#include "y/string/convert.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {


        static const char *declKW[] =
        {
            "dynamo",
            "aka"
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
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr<< "@gen",0) << "<Building Parser>" << std::endl);
            modules.free();
            parser = 0;
            level  = 0;

            declModule(top);
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr<< "@gen",0) << "<Building Parser/>" << std::endl);

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

#if 0
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
#endif

#if 0
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
#endif


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
                    case 0: assert("dynamo"==id); declModule(*sub); break;
                    case 1: assert("aka"==id);    declAlias(*sub);  keep=false; break;
                    default:
                        break;
                }

                if(keep) temp.push_back( sub.yield() );
            }
            --level;
            temp.swap_with(self);
            modules.pop_back();
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[DONE '" << moduleID << "']" << std::endl);


        }

        void DynamoGenerator:: declAlias( const DynamoNode &alias )
        {
            assert( "aka" == alias.name );
            assert( parser.is_valid()   );
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[ALIAS]" << std::endl);
            if( alias.type != DynamoInternal ) throw exception("{%s} unexpected terminal alias", **(parser->name));
            const DynamoList &args = alias.children();
            for(const DynamoNode *node=args.head;node;node=node->next)
            {
                std::cerr << node->name << std::endl;
            }
        }

    }

}

