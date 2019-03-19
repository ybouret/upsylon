#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {
        Syntax::Terminal &DynamoGenerator:: fillSTR( Syntax::Compound &parent, DynamoNode *node )
        {
            assert(node);
            const string &name     = parent.name;
            const string &nodeName = node->name; assert(nodeName=="rs"||nodeName=="rx");

            if(node->type!=DynamoTerminal) throw exception("{%s} unexpected internal node <%s> @<%s>", **(parser->name),*nodeName,*name);
            const bool     fromRX  = ("rx"==node->name);
            const unsigned info    = fromRX ? DynamoInfo::FromRX : DynamoInfo::FromRS;
            const string & strName = node->content();
            const string   strExpr = fromRX ? strName : StringToRegExp(strName);

            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[" << nodeName << "]<-'" << strName << "'=[" << strExpr << "]" << std::endl);

            if(symbols.search(strName))
            {
                throw exception("{%s} expression '%s' collides with a symbol name!!", **(parser->name),*strName);
            }


            DynamoTerm *pTerm = literals.search(strName);
            if(pTerm)
            {
                //______________________________________________________________
                //
                // check same kind of literal
                //______________________________________________________________
                if(0==(pTerm->info&info)) throw exception("{%s} expression '%s' already declared as a different kind of string",**(parser->name),*strName);
                Syntax::Terminal &t = pTerm->derived;
                parent << t;
                return t;
            }
            else
            {
                //______________________________________________________________
                //
                // create new standard literal
                //______________________________________________________________
                Syntax::Terminal &t = parser->term(strName,strExpr);
                const DynamoTerm  symb(modules.back(),t,info);
                Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_new literal '" << symb.from << "_" << symb.rule.name << "'" << std::endl);
                if(!literals.insert(symb))
                {
                    throw exception("{%s} unexpected failure to insert  '%s' !!", **(parser->name),*strName);
                }
                parent << t;
                return t;
            }

        }

        void DynamoGenerator:: fillOP(  Syntax::Compound &parent, DynamoNode *node )
        {
            assert(node);
            assert(node->name=="op");
            if(node->type!=DynamoInternal)                               throw exception("{%s} unexpected internal <op>@<%s>", **(parser->name),*parent.name);
            DynamoList &ch = node->children(); if(1!=ch.size)            throw exception("{%s} corrupted <op>@<%s> level-1", **(parser->name),*parent.name);
            node=ch.head;                      if(!node)                 throw exception("{%s} corrupted <op>@<%s> level-2", **(parser->name),*parent.name);
            const string &id = node->name;     if( id!="rs" && id!="rx") throw exception("{%s} corrupted <op>@<%s> level-3", **(parser->name),*parent.name);
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "<operator>" << std::endl);
            ++level;
            fillSTR(parent,node).op();
            --level;
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "<operator/>" << std::endl);

        }


    }

}

