

#include "y/jive/language/parser.hpp"
#include "y/jive/lexical/plugin/jstring.hpp"
#include "y/jive/common-regexp.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Jive;

namespace
{
    
    class MyParser : public Language::Parser
    {
    public:
        
        MyParser() : Language::Parser("JSON")
        {
            
            Alternate   &JSON    = alt(name);
            Alternate   &VALUE   = alt("value");
            const Axiom &STRING  = plugin<Lexical::jString>("string");
            const Axiom &NUMBER  = terminal("number",RegExpFor::Double);
            VALUE << STRING << NUMBER << terminal("false") << terminal("true");
            const Axiom &COMMA   = division(',');
            
            {
                Alternate   &ARRAY     = alt("array");
                JSON << ARRAY;
                VALUE << ARRAY;
                {
                    const Axiom &LBRACK      = division('[');
                    const Axiom &RBRACK      = division(']');
                    // empty array
                    ARRAY << ( agg("empty_array") << LBRACK << RBRACK );
                    
                    // heavy array
                    Aggregate &HEAVY_ARRAY = agg("heavy_array");
                    ARRAY << HEAVY_ARRAY;
                    HEAVY_ARRAY << LBRACK << VALUE << zeroOrMore( cat(Axioms(COMMA,VALUE))) << RBRACK;
                }
            }
            
            {
                Alternate &OBJECT = alt("object");
                JSON  << OBJECT;
                VALUE << OBJECT;
                {
                    const Axiom &LBRACE     = division('{');
                    const Axiom &RBRACE     = division('}');
                    // empty object
                    OBJECT << ( agg("empty_object") << LBRACE << RBRACE );
                    
                    // heavy object
                    Aggregate &HEAVY_OBJECT = agg("heavy_object");
                    OBJECT << HEAVY_OBJECT;
                    HEAVY_OBJECT << LBRACE;
                    const Axiom &PAIR = agg("pair") << STRING << division(':') << VALUE;
                    HEAVY_OBJECT << PAIR;
                    HEAVY_OBJECT << zeroOrMore( cat(Axioms(COMMA,PAIR)));
                    HEAVY_OBJECT << RBRACE;
                }
            }
            
            
            drop("[:blank:]", "[:blank:]" );
            drop_endl("[:endl:]", "[:endl:]");
            
            validate();
        }
        
        virtual ~MyParser() throw()
        {
        }
        
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(MyParser);
    };
    
    
}
#include "y/fs/local/fs.hpp"

Y_UTEST(lang_parser)
{
    Language::Axiom::Verbose = true;
    vfs &fs = local_fs::instance();
    fs.try_remove_file("tree.dot");
    fs.try_remove_file("tree.png");
    
    MyParser parser;
    parser.graphViz("parser.dot");
    
    if(argc>1)
    {
        //parser.reset();
        Source source( Module::OpenFile(argv[1]));
        xTree  tree( parser.parse(source) );
        if( tree.is_valid() )
        {
            tree->graphViz("tree.dot");
            tree->save_to("tree.bin");
            xTree load( xNode::Load( Module::OpenFile("tree.bin"),parser) );
            Y_CHECK( load->to_binary() == tree->to_binary() );
        }
    }
    
}
Y_UTEST_DONE()

