
#include "y/jargon/parser.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

#include "y/jargon/lexical/plugin/end-of-line-comment.hpp"
#include "y/jargon/lexical/plugin/jstring.hpp"

using namespace  upsylon;
using namespace  Jargon;


namespace {

    class JSON_Parser : public Parser
    {
    public:
        explicit JSON_Parser() : Parser( "JSON" )
        {
            Axiom &LBRACK  = mark('[');
            Axiom &RBRACK  = mark(']');
            Axiom &COMA    = mark(',');
            Axiom &NUMBER  = term("number","[:digit:]+");

            Alternate &VALUE = alt();
            VALUE << term("true") << term("false") << term("null") << NUMBER;
            
            Axiom &empty_array = ( agg("empty_array") << LBRACK << RBRACK );
            Axiom &heavy_array = ( agg("heavy_array") << LBRACK << VALUE << zom( cat(COMA,VALUE) ) << RBRACK);
            Axiom &array       = choice(empty_array,heavy_array);
            
            VALUE << array;
            VALUE << plug( type2type<Lexical::jString>(), "string" );
            
            // lexical
            load(type2type<Lexical::CppComment>(),"com").hook(*this);
            endl("endl",  "[:endl:]");
            drop("blanks","[:blank:]");
            discard("error", ".", this, &JSON_Parser::syntaxError);
            
            setGround(zom(array));
            graphViz("json.dot");
            displayAxioms();
            validate();
        }
        
        virtual ~JSON_Parser() throw()
        {
        }
        
        void syntaxError( const Token &bad ) const
        {
            assert(bad.size>=1);
            const Char &C = *(bad.head);
            exception excp("%s:%d:%d: %s syntax error '",
                            **C.tag,C.line,C.column,**title);
            bad.cat(excp);
            excp.cat("'");
            throw excp;
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(JSON_Parser);
    };

}

#include "y/ios/ocstream.hpp"
Y_UTEST(parser)
{
    Y_UTEST_SIZEOF(Grammar);
    Y_UTEST_SIZEOF(Parser);
    Axiom::Verbose   = true;
    JSON_Parser json;
    
    {
        ios::ocstream fp( ios::cstderr );
        json.display(fp);
    }
    
    if( argc>1 )
    {
        const string    fileName = argv[1];
        Source          source(Module::OpenFile(json.tcache,fileName));
        auto_ptr<XNode> tree = json.parse(source);
        
        tree->graphViz("json_tree.dot");
        
        //json.clear( tree.yield() );
        
        std::cerr << "done" << std::endl;
    }
    
    
}
Y_UTEST_DONE()
