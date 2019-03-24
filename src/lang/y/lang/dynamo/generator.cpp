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
            "aka",
            "plg",
            "lxr",
            "eol",
            "cmd",
            "rule"
        };

        static const char *operKW[] =
        {
            "op",
            "rx",
            "rs"
        };


        static const char *implKW[] =
        {
            "dynamo",
            "rule"
        };

        static const char *fillKW[] =
        {
            //0     1     2      3      4     5     6
            "rid", "jk", "alt", "grp", "rs", "rx", "op"
        };



        DynamoGenerator:: DynamoGenerator() :
        parser(0),
        declH( Y_MPERF_FOR(declKW) ),
        implH( Y_MPERF_FOR(implKW) ),
        operH( Y_MPERF_FOR(operKW) ),
        fillH( Y_MPERF_FOR(fillKW) ),
        modules(),
        symbols(),
        terminals(),
        internals(),
        literals(),
        plugins(4,as_capacity),
        lexicals(4,as_capacity),
        level(0),
        verbose(true)
        {
            // known plugins
            registerPlugin("jstring",  this, & DynamoGenerator::_jstring );
            registerPlugin("rstring",  this, & DynamoGenerator::_rstring );

            // know lexical modifiers
            registerLexical("drop",    this, & DynamoGenerator::lexicalDrop);
            registerLexical("endl",    this, & DynamoGenerator::lexicalEndl);
            registerLexical("comment", this, & DynamoGenerator::lexicalComm);

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



        void DynamoGenerator:: clear() throw()
        {
            level  = 0;
            literals.  release();
            internals. release();
            terminals. release();
            symbols.   release();
            modules.   release();
            parser = 0;
        }


        Syntax::Parser * DynamoGenerator:: build( DynamoNode &top, DynamoSymbols *symbols )
        {
            //__________________________________________________________________
            //
            //
            // Clean up and initialize
            //
            //__________________________________________________________________

            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr<< "@gen",0) << "<Building Parser>" << std::endl);
            clear();

            //__________________________________________________________________
            //
            //
            // first pass : top level declarations and tree clean-up
            //
            //__________________________________________________________________
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr<< "@gen",0) << "<First  Pass: DECL>" << std::endl);
            declModule(top);
            assert(0==modules.size());
            if(parser.is_empty()) throw exception("DynamoGenerator( No Top Level Rule!!! )");

            parser->setVerbose( verbose );

            //__________________________________________________________________
            //
            //
            // second pass: implement rules and literals
            //
            //__________________________________________________________________
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr<< "@gen",0) << "<Second Pass: IMPL>" << std::endl);
            implModule(top);
            assert(0==modules.size());
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr<< "@gen",0) << "<Building Parser/>" << std::endl);


            
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr<< "@gen",0) << " terminals=" << terminals << std::endl);
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr<< "@gen",0) << " internals=" << internals << std::endl);
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr<< "@gen",0) << " literals =" << literals  << std::endl);

            //__________________________________________________________________
            //
            //
            // collect symbols if necessary
            //
            //__________________________________________________________________
            if(symbols)
            {
                symbols->terminals.free();
                symbols->internals.free();

                // dispatching internals
                for( DynamoRule::Set::iterator ir = internals.begin(); ir != internals.end(); ++ir)
                {
                    const DynamoRule &dr = *ir;
                    if(dr.derived.uuid==Syntax::Aggregate::UUID)
                    {
                        if(!symbols->internals.insert(dr))
                        {
                            throw exception("{%s} unexpected multiple internal '%s'", **(parser->name), *(dr.derived.name) );
                        }
                    }
                }

                // dispatching terminals/internals
                dispatchTerminals(terminals, *symbols, "terminal");
                dispatchTerminals(literals,  *symbols, "literal");


            }


            //__________________________________________________________________
            //
            // and validate
            //__________________________________________________________________
            parser->graphViz( *(parser->name) + ".dot" );

            parser->end();

            Syntax::Parser *theParser = parser.yield();
            clear();
            return theParser;
        }

        void DynamoGenerator:: dispatchTerminals( const DynamoTerm::Set &terms, DynamoSymbols &symbols, const char *context ) const
        {
            assert(context);
            for( DynamoTerm::Set::const_iterator it = terms.begin(); it!=terms.end(); ++it )
            {
                const DynamoTerm       &dt = *it;
                const Syntax::Terminal &t  = dt.derived;

                switch(t.attr)
                {
                    case Syntax::Standard: // regular terminal, pugin or alias
                        if(!symbols.terminals.insert(dt))
                        {
                            throw exception("{%s} unexpected multiple standard %s '%s'", **(parser->name), context, *(t.name) );
                        }
                        break;

                    case Syntax::Operator: // an operator becomes an internal
                        if(!symbols.internals.insert(dt))
                        {
                            throw exception("{%s} unexpected multiple operator %s '%s'", **(parser->name), context, *(t.name) );
                        }
                        break;

                    case Syntax::Semantic: // drop
                        break;

                }

            }
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

        string DynamoGenerator:: getEID( const DynamoNode *node, const char *context ) const
        {
            return getContent(node, "eid", context);
        }

        string DynamoGenerator:: getCID( const DynamoNode *node, const char *context ) const
        {
            return getContent(node, "cid", context);
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

namespace upsylon
{

    namespace Lang
    {

        void DynamoGenerator:: registerLexical(const string &id, const DynamoLexical &dl)
        {
            if(!lexicals.insert(id,dl)) throw exception("DynamoGenerator: muliple lexical '%s'", *id);
        }

        DynamoLexical & DynamoGenerator:: findLexical( const string &id )
        {
            DynamoLexical *l = lexicals.search(id);
            if(!l)
            {
                throw exception("DynamoGenerator: no registered lexical '@%s'", *id);
            }
            return *l;
        }



        void DynamoGenerator:: lexicalDrop( Lexer &target, const DynamoArgs &args )
        {
            const size_t n = args.size();
            for(size_t i=1;i<=n;++i)
            {
                const string rx = args[i];
                (*target).drop(rx,rx);
            }
        }

        void DynamoGenerator:: lexicalEndl( Lexer &target, const DynamoArgs &args )
        {
            const size_t n = args.size();
            for(size_t i=1;i<=n;++i)
            {
                const string rx = args[i];
                (*target).endl(rx,rx);
            }
        }

    }

}

#include "y/lang/lexical/plugin/comment.hpp"

namespace upsylon
{

    namespace Lang
    {

        void DynamoGenerator:: lexicalComm( Lexer &target, const DynamoArgs &args )
        {
            switch( args.size() )
            {
                case 1: {
                    // end of line comment
                    const string &arg = args[1];
                    const string  comment_name = "comm@" + arg;
                    target.hook<Lexical::EndOfLineComment>(*target, comment_name, *arg);
                } break;

                case 2:
                {
                    // multiple line comment
                    const string &arg1 = args[1];
                    const string &arg2 = args[2];
                    const string  comment_name = "comm@" + arg1 + arg2;
                    target.hook<Lexical::MultiLinesComment>(*target,comment_name,*arg1,*arg2);
                }   break;

                default:
                    throw exception("DynamoGenerator.lexicalComm: invalid #args=%u", unsigned(args.size()));
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



