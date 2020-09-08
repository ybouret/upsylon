
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/jive/lexical/editor/program.hpp"
#include "y/ios/ocstream.hpp"
#include "y/information/translator/base64/encoder.hpp"

using namespace upsylon;
using namespace Jive;

namespace {
    
    class Ed : public Lexical::Editor::Program
    {
    public:
        Ed()
        {
            //on("[:lower:]",this, & Ed::OnLower);
            on("[:word:]+",this, & Ed::OnWord );
        }
        
        virtual ~Ed() throw()
        {
        }
        
        //void OnLower(ios::ostream &fp, const Token &t) { fp << '<' << t << '>'; }
        
        void OnWord(ios::ostream &fp, const Token &t)
        {
            for(const Char *ch = t.tail;ch;ch=ch->prev)
            {
                fp << *ch;
            }
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Ed);
    };
    
    class Ed64 : public Lexical::Editor::Program
    {
    public:
        Information::Base64Encoder enc;
        
        Ed64() : enc()
        {
            on("[:word:]+",this, & Ed64::OnWord );
        }
        
        virtual ~Ed64() throw()
        {
        }
        
        
        void OnWord(ios::ostream &fp, const Token &t)
        {
            enc.reset();
            for(const Char *ch = t.head;ch;ch=ch->next)
            {
                enc.write(ch->code);
            }
            enc.flush();
            char C=0;
            while(enc.query(C)) fp << C;
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Ed64);
    };
}

Y_UTEST(editor)
{
    
    Ed prog;
    if(argc>1)
    {
        Source        source( Module::OpenFile(argv[1]) );
        ios::ocstream fp( ios::cstdout );
        prog.run(fp,source);
    }
    
}
Y_UTEST_DONE()

Y_UTEST(editor64)
{
    Ed64 prog;
    if(argc>1)
    {
        Source        source( Module::OpenFile(argv[1]) );
        ios::ocstream fp( ios::cstdout );
        prog.run(fp,source);
    }
}
Y_UTEST_DONE()

namespace {
 
    class Snake : public Lexical::Editor::Program
    {
    public:
        
        Snake()
        {
            on("_[:lower:]",this, & Snake::OnLower );
            on("_[:upper:]",this, & Snake::OnUpper);

        }
        
        virtual ~Snake() throw()
        {
        }
        
        
        void OnLower(ios::ostream &fp, const Token &t)
        {
            fp << toupper(t.tail->code);
        }
        
        void OnUpper(ios::ostream &fp, const Token &t)
        {
            fp << t.tail->code;
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Snake);
    };
    
}

Y_UTEST(ed_snake)
{
    Snake ed;
    const string src = "hello_world_You";
    const string tgt = ed.run(src);
    std::cerr << "src=" << src << " => " << tgt << std::endl;
}
Y_UTEST_DONE()

