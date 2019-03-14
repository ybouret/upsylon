#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {

        const Syntax::Rule  & DynamoGenerator:: findSymbol( const string &id )
        {
            assert(parser.is_valid());
            DynamoInfo *pInf = symbols.search(id);
            if(!pInf)
            {
                throw exception("{%s} unregistered symbol '%s'", **(parser->name), *id);
            }
            return pInf->rule;
        }


        void DynamoGenerator:: implModule(  DynamoNode &dynamo )
        {
            static const char fn[] = "DynamoGenerator.implModule";

            //__________________________________________________________________
            //
            // sanity check
            //__________________________________________________________________
            assert( parser.is_valid() );

            if(dynamo.name!="dynamo")       throw exception("%s(expecting <dynamo> and not <%s>)",fn,*(dynamo.name));
            if(dynamo.type!=DynamoInternal) throw exception("%s(<dynamo> is not internal)",fn);

            DynamoList     &self = dynamo.children();
            while(self.size)
            {
                auto_ptr<DynamoNode> node = self.pop_front();
                const string &id = node->name;
                switch( implH(id) )
                {
                    case 0: assert("dynamo"==id);
                        implModule(*node);
                        break;

                    case 1:
                        assert("rule"  ==id);
                        implRule(*node);
                        break;

                    default:
                        throw exception("%s(unexpected top-level <%s>)", **(parser->name), *id);

                }


            }

        }

        void DynamoGenerator:: implRule( DynamoNode &r )
        {
            //__________________________________________________________________
            //
            // sanity check
            //__________________________________________________________________
            assert( parser.is_valid() );
            assert( "rule" == r.name  );
            if(r.type!=DynamoInternal) throw exception("{%s} rule is not internal", **(parser->name));

            //__________________________________________________________________
            //
            // get the rule name
            //__________________________________________________________________
            const DynamoList &self     = r.children();
            const DynamoNode *node     = self.head;
            const string      ruleName = getRID(node, "rule name");

            //__________________________________________________________________
            //
            // get the parent rule
            //__________________________________________________________________
            Syntax::Rule     &rule     = parser->getRuleByName(ruleName);    assert(rule.derived);
            Syntax::Compound &cmp      = *(Syntax::Compound*)(rule.derived); assert(rule.uuid==cmp.uuid);
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[" << ruleName << "]/" << rule.typeName() << std::endl);

            //__________________________________________________________________
            //
            // start recursion depending on parent UUID
            //__________________________________________________________________
            switch(cmp.uuid)
            {
                case Syntax::Aggregate::UUID:
                    fill(cmp,node->next);
                    break;

                case Syntax::Alternate::UUID:
                    node=node->next;
                    if(self.size!=2||node->name!="alt") throw exception("{%s} unexpected top level alternate <%s>",**(parser->name),*ruleName);
                    if(node->type!=DynamoInternal)      throw exception("{%s} unexpected terminal 'alt' for  <%s>",**(parser->name),*ruleName);
                    fill(cmp,node->children().head);
                    break;

                default:
                    throw exception("{%s} unexpected UUID for <%s> at top level, corrupted code", **(parser->name),*ruleName);
            }
        }

        void DynamoGenerator:: fill( Syntax::Compound &parent, DynamoNode *node )
        {
            ++level;
            assert(node);
            const string &name = parent.name;
            for(;node;node=node->next)
            {
                const string &id = node->name;
                switch( fillH(id) )
                {
                    case 0: assert("rid"==id); {
                        const string symbolName = getRID(node,"symbol name");
                        Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[" << name << "]<-" << id << "=[" << symbolName << "]" << std::endl);
                        parent << findSymbol(symbolName);
                    } break;

                    default:
                        Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[" << name << "]<-" << id << std::endl);

                }

            }
            --level;
        }



    }

}


