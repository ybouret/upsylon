

#include "y/jargon/lexer.hpp"
#include "y/jargon/lexeme.hpp"

#include "y/jargon/lexical/plugin/end-of-line-comment.hpp"
#include "y/jargon/lexical/plugin/multi-lines-comment.hpp"
#include "y/jargon/lexical/plugin/jstring.hpp"
#include "y/jargon/lexical/plugin/rstring.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jargon;

namespace {
    
    
    class myLex : public Lexer
    {
    public:
        myLex() : Lexer( "myLex" )
        {
            emit("id",   "[:alpha:]+");
            emit("int",  "[:digit:]+");
            endl("endl", "[:endl:]");
            drop("blank","[:blank:]");
            call("com1", "#", this, & myLex::enterCom1  );
            jump("com2", "//", this, & myLex::enterCom2 );
            call("com3", "/\\*", this, & myLex::enterCom3 );
           
            Lexical::Plugin &luaEOL = load(type2type<Lexical::EndOfLineComment>(), "luaEOL", "--");
            luaEOL.hook(*this);
            
            Lexical::Plugin &texEOL = load(type2type<Lexical::TeXComment>(),"texEOL");
            texEOL.hook(*this);
            
            Lexical::Plugin &xmlCom = load(type2type<Lexical::MultiLinesComment>(),"xmlCom","<!--","-->");
            xmlCom.hook(*this);
            
            Lexical::Plugin & ccom = load(type2type<Lexical::C_Comments>(),"C_Comments");
            if(false)
            {
                ccom.hook(*this);
            }
            (void)load(type2type<Lexical::XML_Comments>(),"XML_Comments");

            Lexical::Plugin &jstring = load(type2type<Lexical::jString>(),"jstring");
            jstring.hook(*this);
            
            Lexical::Plugin &rstring = load(type2type<Lexical::rString>(),"rstring");
            rstring.hook(*this);
            
            emit("punct","[:punct:]");
            
            
            {
                Scanner &com1 = decl("com1");
                com1.back("[:endl:]",this,& myLex::leaveCom1);
                com1.drop("any", ".");
            }
            
            {
                Scanner &com2 = decl("com2");
                com2.jump(*label,"[:endl:]",this, &myLex::leaveCom2);
                com2.drop("any",".");
            }
            
            {
                Scanner &com3 = decl("com3");
                com3.back("\\*/", this, & myLex::leaveCom3);
                com3.endl("endl", "[:endl:]");
                com3.drop("any",".");
            }
            
           
            Tags::Display();
            dict.release_all();
        }
        
        virtual ~myLex() throw()
        {
        }
        
        void enterCom1( const Token &token )
        {
            std::cerr << "<Com1 @" << token << ">" << std::endl;
        }
        
        void leaveCom1(const Token &token )
        {
            std::cerr << "</Com1 @" << token << ">" << std::endl;
            newLine(token);
        }
        
        void enterCom2( const Token &token )
        {
            std::cerr << "<Com2 @" << token << ">" << std::endl;
        }
        
        void leaveCom2(const Token &token )
        {
            std::cerr << "</Com2 @" << token << ">" << std::endl;
            newLine(token);
        }
        
        void enterCom3( const Token &token )
        {
            std::cerr << "<Com3 @" << token << ">" << std::endl;
        }
        
        void leaveCom3(const Token &token )
        {
            std::cerr << "</Com3 @" << token << ">" << std::endl;
        }
        
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(myLex);
    };
}

Y_UTEST(jargon_lexer)
{
    Lexical::Scanner::Verbose = true;
    myLex L;
    if(argc>1)
    {
        Lexemes   lexemes;
        Cache     cache;
        Source    source(cache,Module::OpenFile(cache, argv[1]));
        Lexeme   *lx = 0;
        while( NULL != (lx = L.get(source) ) )
        {
            lexemes.push_back(lx);
        }
        
        while( lexemes.size )
        {
            lx = lexemes.head;
            std::cerr << "<" << lx->label << "> '" << *lx << "' @" << lx->tag << ":" << lx->line << ":" << lx->column << std::endl;
            delete lexemes.pop_front();
        }
    }
    
}
Y_UTEST_DONE()

