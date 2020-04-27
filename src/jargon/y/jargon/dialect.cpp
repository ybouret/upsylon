
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
            
            //------------------------------------------------------------------
            //
            // prepare the items to collect
            //
            //------------------------------------------------------------------
            Alternate &item    = alt("item");

            //------------------------------------------------------------------
            // terminals
            //------------------------------------------------------------------
            Axiom     &stop    = mark(';');
            Axiom     &id      = term("id","{ID}");
            Axiom     &rx      = plug(Lexical::jString::Type,"rx");
            Axiom     &rs      = plug(Lexical::rString::Type,"rs");
            Axiom     &sep     = mark(':');
            
            //------------------------------------------------------------------
            //
            // plugins
            //
            //------------------------------------------------------------------
            {
                Axiom   &plg_init = zom( alt("plg.init") << rx << rs );
                Axiom   &plg      = ( agg("plg") << term("plg.name","@{ID}") << sep << id << plg_init << stop);
                item << plg;
            }
            
            //------------------------------------------------------------------
            //
            // include
            //
            //------------------------------------------------------------------
            {
                Axiom &inc_name = term("#include");
                Axiom &inc_args = ( alt("inc.args") << rs << rx );
                Axiom &inc      = ( agg("inc") << inc_name << inc_args );
                item << inc;
            }
            
            {
                
            }
            
            //------------------------------------------------------------------
            //
            // lexical helpers
            //
            //------------------------------------------------------------------
            {
                Axiom   &lex_args = zom( alt("lex.args") << rx << rs );
                Axiom   &lex      = ( agg("lex") <<  term("lex.name","%{ID}") << lex_args );
                item << lex;
            }
            
            //------------------------------------------------------------------
            //
            // axiom
            //
            //------------------------------------------------------------------
            {
                Aggregate &axiom = agg("axiom");
                
                axiom << id << sep;
                {
                    Aggregate &group = act("group");
                    {
                        Alternate &content = alt("content");
                        content << id << rs << rx;
                        group << oom(content);
                    }
                    
                    axiom << group << zom( cat(op("or",'|'),group) );
                    
                }
                axiom << stop;
                item << axiom;
            }
            
            
            //__________________________________________________________________
            //
            // lexical only
            //__________________________________________________________________
            grab(Lexical::EndOfLineComment::Type,  "Comment","//");
            grab(Lexical::MultiLinesComment::Type, "Comments","/\\*","\\*/");
            drop("blank", "[:blank:]");
            endl("endl",  "[:endl:]");
            grab(Lexical::Error::Type,"Dialect Syntax Error");
            
            //__________________________________________________________________
            //
            // and final Axion which is the root
            //__________________________________________________________________
            setRoot(agg("dialect") << cat(term("module", "[.]{ID}"),stop) << zom(item) );
            
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
                if(sub->name()=="inc")
                {
                    const Context &context = *module;
                    throw exception("%s::compileFlat: no allowed inclusion in '%s'",**title,**(context.tag));
                }
            }
            return root.yield();
        }


    }
    
}

