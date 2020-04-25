
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
            Axiom     &op      = term("op",'^');
            Axiom     &sep     = mark(':');
            {
                Axiom   &plg_init = zom( alt("plg.init") << rx << rs );
                Axiom   &plg      = ( agg("plg") << term("plg.name","@{ID}") << sep << id << plg_init << stop);
                item << plg;
            }
            
            
            {
                Alternate &ctl_args = alt("ctl.args");
                ctl_args << rs << rx << term("int","[-+]?[:digit:]+") << term("hex","0x[:xdigit:]+");
                
                Axiom     &ctl   = ( agg("ctl") <<  term("ctl.name","#{ID}") << zom(ctl_args) );
                item << ctl;
            }
            
            Axiom &atom = act("atom") << choice(id,rs,rx) << opt(op);
            {
                Axiom & aka = ( agg("aka") << id << sep << atom << stop );
                item << aka;
            }
            
            
            {
                Axiom   &lex_args = zom( alt("lex.args") << rx << rs );
                Axiom & lex = ( agg("lex") <<  term("lex.name","%{ID}") << lex_args );
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
                    const string   ctlName = readCtlName(*sub);
                    const Context &context = *module;
                    throw exception("%s::compileFlat: no allowed control '%s' in '%s'",**title,*ctlName,**(context.tag));
                }
            }
            return root.yield();
        }


    }
    
}

