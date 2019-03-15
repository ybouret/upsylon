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
            unsigned      indx = 1;
            for(;node;node=node->next,++indx)
            {
                const string &id = node->name;
                switch( fillH(id) )
                {
                        //------------------------------------------------------

                    case 0: assert("rid"==id); {
                        const string symbolName = getRID(node,"symbol name");
                        Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[" << name << "]<-" << id << "=[" << symbolName << "]" << std::endl);
                        parent << findSymbol(symbolName);
                    } break;


                    case 1: assert("jk"==id); {
                        const string      jokerName = name + vformat("#jk@%u",indx);
                        if(node->type!=DynamoInternal) throw exception("{%s} unexpected terminal joker in <%s>", **(parser->name),*name);
                        const DynamoList &ch = node->children();
                        if(ch.size!=2)                    throw exception("{%s} mismatching joker size in <%s>", **(parser->name),*name);
                        if(ch.tail->type!=DynamoTerminal) throw exception("{%s} mismatching joker type in <%s>", **(parser->name),*name);
                        const string     &jokerType = ch.tail->name;
                        std::cerr << "Joker Type=[" << jokerType << "]" << std::endl;
                        Syntax::Compound &jokerRule = parser->bundle(jokerName);
                        fill(jokerRule,ch.head);
                        switch( jokerType[0] )
                        {
                            case '*': parent << parser->zeroOrMore(jokerRule); break;
                            case '+': parent << parser->oneOrMore(jokerRule);  break;
                            case '?': parent << parser->optional(jokerRule);   break;
                            default: throw exception("{%s} unknown joker type in <%s>",**(parser->name),*name);
                        }

                    } break;

                    default:
                        Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[" << name << "]<-" << id << std::endl);

                }

            }
            --level;
        }



    }

}


