
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

