
#include "y/jargon/parser.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

#include "y/jargon/lexical/plugin/end-of-line-comment.hpp"
#include "y/jargon/lexical/plugin/strings.hpp"

using namespace  upsylon;
using namespace  Jargon;


namespace {

    class JSON_Parser : public Parser
    {
    public:
        explicit JSON_Parser() : Parser( "JSON" )
        {
            Axiom &LBRACK  = term('[');
            Axiom &RBRACK  = term(']');
            Axiom &COMA    = term(',');
            Axiom &NUMBER  = term("number","[:digit:]+");

            Alternate &VALUE = alt();
            VALUE << term("true") << term("false") << term("null") << NUMBER;
            
            Axiom &empty_array = ( agg("empty_array") << LBRACK << RBRACK );
            Axiom &heavy_array = ( agg("heavy_array") << LBRACK << VALUE << zom( cat(COMA,VALUE) ) << RBRACK);
            Axiom &array       = choice(empty_array,heavy_array);
            
            VALUE << array;
            
            // lexical
            load(type2type<Lexical::CppComment>(),"com").hook(*this);
            endl("endl",  "[:endl:]");
            drop("blanks","[:blank:]");
            
            
            setGround(zom(array));
            graphViz("json.dot");
            displayAxioms();
            validate();
        }
        
        virtual ~JSON_Parser() throw()
        {
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(JSON_Parser);
    };

}

Y_UTEST(parser)
{
    Y_UTEST_SIZEOF(Grammar);
    Y_UTEST_SIZEOF(Parser);
    Axiom::Verbose   = true;
    Lexical::Scanner::Verbose = true;
    JSON_Parser json;
    
    if( argc>1 )
    {
        const string    fileName = argv[1];
        Source          source(json.tcache,Module::OpenFile(json.tcache,fileName));
        auto_ptr<XNode> tree = json.parse(source);
    }
    
    
}
Y_UTEST_DONE()
