
#include "y/jargon/dialect.hpp"

#include "y/jargon/lexical/plugin/end-of-line-comment.hpp"
#include "y/jargon/lexical/plugin/multi-lines-comment.hpp"
#include "y/jargon/lexical/plugin/jstring.hpp"
#include "y/jargon/lexical/plugin/rstring.hpp"
#include "y/jargon/lexical/plugin/error.hpp"

#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {

        Dialect:: ~Dialect() throw()
        {
        }
        
        Dialect:: Dialect() : Parser( "Dialect" )
        {
            
            if(!dict.insert("ID",RegularExpression::Identifier))
                throw exception("[%s] couldn't initialize dictionary!",**title);
            
            Aggregate &dialect = agg("dialect");
            Axiom     &stop    = mark(';');
            Alternate &item    = alt("item");
            dialect   << cat(term("module", "[.]{ID}"),stop) << zom(item);
            Axiom     &id      = term("id","{ID}");
            Axiom     &rx      = plug(Lexical::jString::Type,"rx");
            Axiom     &rs      = plug(Lexical::rString::Type,"rs");
            Axiom     &str     = ( alt("str") << rx << rs);
           
            Axiom     &zom_str = zom(str);
            Axiom     &sep     = mark(':');
            {
                Axiom     &plg     = ( agg("plg") << term("pid","@{ID}") << sep << id << zom_str << stop);
                item << plg;
            }
            
            
            {
#if 0
                Axiom     &_int  = term("int","[-+]?[:digit:]+");
                Axiom     &_hex  = term("hex","0x[:xdigit:]+");
                Axiom     &ints  = choice(_int,_hex);
#endif
                Axiom     &ctl   = ( agg("ctl") <<  term("cid","#{ID}") << zom(choice(str,id)) );
                item << ctl;
            }
            
            Axiom &atom = act("atom") << choice(id,rs,rx) << opt(term('^'));
            {
                Axiom & aka = ( agg("aka") << id << sep << atom << stop );
                item << aka;
            }
            
            
            {
                Axiom & lex = ( agg("lex") <<  term("lid","%{ID}") << zom_str );
                item << lex;
            }
            
            
            
            
            //__________________________________________________________________
            //
            // lexical only
            //__________________________________________________________________
            grab(Lexical::EndOfLineComment::Type,  "EOLComment","//");
            grab(Lexical::MultiLinesComment::Type, "MLComments","/\\*","\\*/");
            drop("blank", "[:blank:]");
            endl("endl",  "[:endl:]");
            grab(Lexical::Error::Type,"Dialect Syntax Error");
            
            dict.release_all();
            compile();
        }
        

        XNode * Dialect:: compileFile(const string &fileName)
        {
            return checkIncludes( expressBlocks(parseFile(fileName)),fileName);
        }

        XNode * Dialect:: compileFile(const char   *fileName)
        {
            const string _(fileName); return compileFile(_);
        }
        
        XNode * Dialect:: compileFlat(Module *module)
        {
            assert(module);
            auto_ptr<XNode>  root = parse(module);
            for(const XNode *sub  = root->children.head;sub;sub=sub->next)
            {
                if(sub->name()=="ctl")
                {
                    const string   cid = readCID(*sub);
                    const Context &ctx = *module;
                    throw exception("%s::compileFlat: no allowed control '%s' in '%s'",**title,*cid,**(ctx.tag));
                }
            }
            return root.yield();
        }


    }
    
}

