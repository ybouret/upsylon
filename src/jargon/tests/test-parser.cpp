
#include "y/jargon/parser.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

#include "y/jargon/lexical/plugin/jstring.hpp"
#include "y/jargon/lexical/plugin/error.hpp"

#include "y/jargon/evaluator.hpp"

using namespace  upsylon;
using namespace  Jargon;


namespace {

    class JSON_Parser : public Parser
    {
    public:
        explicit JSON_Parser() : Parser( "JSON" )
        {
            
            Alternate &json = alt();
            
            // value
            Alternate &VALUE  = alt();
            Axiom     &STRING = plug( type2type<Lexical::jString>(), "string" );
            {
                VALUE << term("number",RegularExpression::Real);
                VALUE << term("null");
                VALUE << term("true");
                VALUE << term("false");
                VALUE << STRING;
            }
            
            // common terminal
            Axiom &COMA    = mark(',');
            
            // arrays
            {
                Axiom &LBRACK  = mark('[');
                Axiom &RBRACK  = mark(']');
                Axiom &empty_array = ( agg("empty_array") << LBRACK << RBRACK );
                Axiom &heavy_array = ( agg("heavy_array") << LBRACK << VALUE << zom( cat(COMA,VALUE) ) << RBRACK);
                Axiom &array       = choice(empty_array,heavy_array);
                
                VALUE << array;
                json  << array;
            }
            
            // objects
            {
                Axiom  &LBRACE      = mark('{');
                Axiom  &RBRACE      = mark('}');
                Axiom  &PAIR        = ( agg("pair") << STRING << mark(':') << VALUE);
                Axiom &empty_object = ( agg("empty_object") << LBRACE << RBRACE );
                Axiom &heavy_object = ( agg("heavy_object") << LBRACE << PAIR << zom(cat(COMA,PAIR)) << RBRACE );
                Axiom &the_object   = choice(empty_object,heavy_object);
                VALUE << the_object;
                json  << the_object;
            }
            
            // lexical
            endl("endl",  "[:endl:]");
            drop("blanks","[:blank:]");
            
            
            load( type2type<Lexical::Error>(), "JSON Error" ).hook(*this);
           
            Tags::Display();
            graphViz("json.dot");
            compile();
        }
        
        virtual ~JSON_Parser() throw()
        {
        }
        
        
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(JSON_Parser);
    };

}

#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/serialized.hpp"
using namespace ios;

Y_UTEST(parser)
{
   

    Axiom::Verbose   = true;
    JSON_Parser json;
    
    
    if( argc>1 )
    {
        const string    fileName = argv[1];
        auto_ptr<XNode> tree = json.parse(Module::OpenFile(fileName));
        
        tree->graphViz("json_tree.dot");
        
        Evaluator_ E(json.title);
        E.browse(*tree);
        
        tree->save_to("tree.bin");
        
        {
            auto_ptr<XNode> tree2 = json.loadTreeFromFile("tree.bin");
            tree2->save_to( "tree2.bin" );
            Y_CHECK(serialized::are_same_binary(*tree,*tree2));
        }
        
        
        
    }
    
    Y_UTEST_SIZEOF(Source);
    Y_UTEST_SIZEOF(Grammar);
    Y_UTEST_SIZEOF(Parser);
    Y_UTEST_SIZEOF(Axiom::DB);
    
}
Y_UTEST_DONE()
