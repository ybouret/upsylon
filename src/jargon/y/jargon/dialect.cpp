
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
        
        Dialect:: Dialect() : Parser( "Jargon::Dialect" )
        {
            
            if(!dict.insert("NAME",RegularExpression::Identifier))
                throw exception("[%s] couldn't initialize dictionary!",**title);
            
            
            //__________________________________________________________________
            //
            // setup grammar
            //__________________________________________________________________
            Aggregate &G   = agg("dialect");
            
            
            //__________________________________________________________________
            //
            // terminals declarations
            //__________________________________________________________________
            Axiom     &END   = mark(';');
            Axiom     &COLON = mark(':');
            Axiom     &ID    = term("ID","{NAME}");
            Axiom     &RX    = plug(Lexical::jString::Type,"RX");
            Axiom     &RS    = plug(Lexical::rString::Type,"RS");
            Axiom     &STR   = choice(RX,RS);
            Axiom     &INT   = term("INT","[-+]?[:digit:]+");
            Axiom     &ZOM_STR = zom(STR);
            
            //__________________________________________________________________
            //
            // top level: declare module name
            //__________________________________________________________________
            {
                G << cat(term("module", "[.]{NAME}"),END);
            }
            
            //__________________________________________________________________
            //
            // aliases/operators...
            //__________________________________________________________________
            Axiom & AKA = ( agg("aka") << ID << COLON << STR << END);
            
            
            //__________________________________________________________________
            //
            // plugins
            //__________________________________________________________________
            Axiom & PLG = ( agg("plg") << term("pid","@{NAME}") << COLON << STR << ZOM_STR << END);
            
            //__________________________________________________________________
            //
            // pre-processor...
            //__________________________________________________________________
            Axiom & CTL = ( agg("ctl") <<  term("cid","#{NAME}") << zom(choice(STR,INT)) );
            
            //__________________________________________________________________
            //
            // lexical
            //__________________________________________________________________
            Axiom & LEX = ( agg("lex") <<  term("lid","%{NAME}") << ZOM_STR );
            
            //__________________________________________________________________
            //
            // define grammar
            //__________________________________________________________________
            G << zom(  ( alt() << AKA << PLG << CTL << LEX) );

            
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

