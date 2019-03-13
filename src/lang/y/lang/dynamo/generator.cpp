#include "y/lang/dynamo/generator.hpp"
#include "y/string/convert.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {

        DynamoInfo:: DynamoInfo(const Tag &moduleID, const Syntax::Rule &r ) throw() :
        from(moduleID),
        rule(r)
        {
        }

        DynamoInfo:: ~DynamoInfo() throw()
        {
        }

        DynamoInfo:: DynamoInfo( const DynamoInfo &other) throw() :
        from(other.from), rule(other.rule)
        {
        }

        const string & DynamoInfo:: key() const throw()
        {
            return rule.name;
        }


    }

}



namespace upsylon
{
    namespace Lang
    {


        static const char *declKW[] =
        {
            "dynamo",
            "aka",
            "plg"
        };

        DynamoGenerator:: DynamoGenerator() :
        parser(0),
        declH( Y_MPERF_FOR(declKW) ),
        modules(),
        level(0),
        verbose(true)
        {
            registerPlugin("jstring",  this, & DynamoGenerator::_jstring );
            registerPlugin("rstring",  this, & DynamoGenerator::_rstring );

        }

        void DynamoGenerator:: registerPlugin(const string &id, const DynamoPlugin &dp)
        {
            if(!plugins.insert(id,dp)) throw exception("DynamoGenerator: muliple plugins '%s'", *id);
        }

        DynamoPlugin & DynamoGenerator:: findPlugin( const string &id )
        {
            DynamoPlugin *p = plugins.search(id);
            if(!p) throw exception("DynamoGenerator: no registered plugin '%s'", *id);
            return *p;
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

            parser->graphViz( *(parser->name) + ".dot" );
            return 0;
        }

    }

}

namespace upsylon
{

    namespace Lang
    {

        string DynamoGenerator:: getContent( const DynamoNode *node, const char *id, const char *context) const
        {
            assert(context);
            assert(id);
            if(!node)                      throw exception("{%s} %s missing <%s>",         **(parser->name),context,id);
            if(node->name!=id)             throw exception("{%s} %s invalid <%s>!=<%s>",   **(parser->name),context, *(node->name),id);
            if(node->type!=DynamoTerminal) throw exception("{%s} %s <%s> is not terminal!",**(parser->name),context,id);
            return node->content();
        }


        string DynamoGenerator:: getRID( const DynamoNode *node, const char *context ) const
        {
            return getContent(node, "rid", context);
        }

        string DynamoGenerator:: getLID( const DynamoNode *node, const char *context ) const
        {
            return getContent(node, "lid", context);
        }


        string DynamoGenerator:: getSTR( const DynamoNode *node, const char *context ) const
        {
            assert(context);
            if(!node)                              throw exception("{%s} %s missing <rs|rx>",         **(parser->name),context);
            const bool isRS = (node->name=="rs");
            const bool isRX = (node->name=="rx");
            if(!(isRS||isRX))                      throw exception("{%s} %s invalid <%s>!=<rs|rx>",   **(parser->name),context, *(node->name));
            if(node->type!=DynamoTerminal)         throw exception("{%s} %s <rs|rx> is not terminal!",**(parser->name),context);
            if(isRX)
            {
                return node->content();
            }
            else
            {
                const string cnt = node->content();
                return StringToRegExp(cnt);
            }
        }
    }

}


#include "y/lang/lexical/plugin/strings.hpp"

namespace upsylon
{

    namespace Lang
    {

        Syntax::Terminal & DynamoGenerator:: _jstring( const string &termID, Syntax::Parser &p )
        {
            return p.plug<Lexical::jString>(termID);
        }


        Syntax::Terminal & DynamoGenerator:: _rstring( const string &termID, Syntax::Parser &p )
        {
            return p.plug<Lexical::rString>(termID);
        }


    }

}



