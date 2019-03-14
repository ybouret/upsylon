#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {
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
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << " implement [" << ruleName << "]" << std::endl);
            Syntax::Rule     &rule     = parser->getRuleByName(ruleName);    assert(rule.derived);
            Syntax::Compound &cmp      = *(Syntax::Compound*)(rule.derived); assert(rule.uuid==cmp.uuid);
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << ".....found '" << cmp.name << "'" << std::endl);
            switch(cmp.uuid)
            {
                case Syntax::Aggregate::UUID:
                    break;

                case Syntax::Alternate::UUID:
                    break;

                default:
                    throw exception("{%s} unexpected UUID at top level, corrupted code", **(parser->name));
            }
        }


    }

}


