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
                const string        &id   = node->name;

                switch( implH(id) )
                {
                    case 0: assert("dynamo"==id); implModule(*node); break;
                    case 1: assert("rule"  ==id); implRule(*node);   break;
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
            // get the top-level parent rule
            //__________________________________________________________________
            DynamoRule         *pRule = internals.search(ruleName); if(!pRule)     throw exception("{%s} missing top level '%s'", **(parser->name), *ruleName);
            const Syntax::Rule &rule  = pRule->rule;    assert(rule.derived);
            Syntax::Compound   &cmp   = *(Syntax::Compound*)(rule.derived); assert(rule.uuid==cmp.uuid);

            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[" << ruleName << "]/" << rule.typeName() << std::endl);

            modules.push_back(pRule->from);
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
            modules.pop_back();
        }

        static inline
        void isSM( const Tag &parserTag, Syntax::Terminal &t )
        {
            switch(t.attr)
            {
                case Syntax::Standard: t.sm(); break;
                case Syntax::Semantic:         break;
                case Syntax::Operator: throw exception("{%s} operator '%s' cannot be checked as semantic!!!", **parserTag, *t.name);
            }
        }

        void DynamoGenerator:: fill( Syntax::Compound &parent, DynamoNode *node )
        {
            assert(modules.size()>0);
            ++level;
            assert(node);
            const string &name = parent.name;
            unsigned      indx = 1;
            for(;node;node=node->next,++indx)
            {
                const string &id = node->name;
                switch( fillH(id) )
                {
                    case 0: assert("rid"==id); fillRID(parent,node);       break;
                    case 1: assert("jk" ==id); fillJK(parent,node,indx);   break;
                    case 2: assert("alt"==id); fillALT(parent,node,indx);  break;
                    case 3: assert("grp"==id); fillGRP(parent,node,indx);  break;
                    case 4: assert("rs" ==id); isSM(parser->name,fillSTR(parent,node)); break;
                    case 5: assert("rx" ==id); (void)fillSTR(parent,node); break;
                    case 6: assert("op" ==id); fillOP(parent,node);        break;

                    default:
                        throw exception("{%s} unexpected  <%s> while filling '%s'", **(parser->name),*id,*name);
                }
                
            }
            --level;
        }
        
     
        
        string DynamoGenerator:: MakeSubName( const Syntax::Compound &parent, const unsigned indx)
        {
            return parent.name + vformat("@%u",indx);
        }
        
          
    }
    
}


