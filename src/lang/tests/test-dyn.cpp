#include "y/lang/dynamo/loader.hpp"
#include "y/lang/dynamo/generator.hpp"
#include "y/fs/local/file.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Lang;

static inline Module * doOpen( const string &fn )
{
    return (fn=="STDIN") ? Module::OpenSTDIN() : Module::OpenFile(fn);
}


Y_UTEST(dyn)
{
    DynamoLoader    dynamo;
    DynamoCompiler  dynCmp;
    DynamoGenerator dynGen;

    dynamo.graphViz("dynamo.dot");

    DynamoSymbols symbols;

    if(argc>1)
    {
        const string           fn = argv[1];
        auto_ptr<Syntax::Node> g  = dynamo.load( doOpen(fn) );

        if(true)
        {
            //g->graphViz( "dynamo_tree.dot" );
            const size_t bytes = g->save_to("dynamo.bin");
            std::cerr << "Written=" << bytes << std::endl;
            Y_CHECK(ios::local_file::length_of("dynamo.bin")==bytes);
            
            {
                std::cerr << "Reloading" << std::endl;
                auto_ptr<Syntax::Node> g2 = Syntax::Node::Load( Module::OpenFile("dynamo.bin"),dynamo );
                //g2->graphViz("dynamo_tree2.dot");
            }
        }

        auto_ptr<DynamoNode> il = dynCmp.compile(*g);
        std::cerr << "IntermediateLang=" << std::endl;
        std::cerr << il;

        const size_t out_bytes = il->serialize_length();
        il->save_to("il.bin");
        

        {
            auto_ptr<DynamoNode> il2 = DynamoNode::Load( Module::OpenFile("il.bin") );
            hashing::sha1 H;
            digest md0 = il->md(H);
            digest md2 = il2->md(H);
            std::cerr << "md0=" << md0 << std::endl;
            std::cerr << "md2=" << md2 << std::endl;
            if(md0!=md2)
            {
                throw exception("DynamoNode I/O failure");
            }

        }

        il->graphViz("il0.dot");
        auto_ptr<Syntax::Parser> P = dynGen.build( *il, &symbols );
        
        std::cerr << "symbols.terminals=" << symbols.terminals << std::endl;
        std::cerr << "symbols.internals=" << symbols.internals << std::endl;

        //DynamoHash31 H;

        for(DynamoSymbolIterator i= symbols.terminals.begin(); i != symbols.terminals.end(); ++i)
        {
            const DynamoInfo &di = *i;
            std::cerr << "term '" << di.rule.name << std::endl; //"' -> " << H(di.rule.name) << std::endl;
        }

        for(DynamoSymbolIterator i= symbols.internals.begin(); i != symbols.internals.end(); ++i)
        {
            const DynamoInfo &di = *i;
            std::cerr << "rule '" << di.rule.name << std::endl; //"' -> " << H(di.rule.name) << std::endl;
        }



        if(argc>2)
        {
            const string    fn      = argv[2];
            auto_ptr<XNode> tree    = P->run((fn=="STDIN") ? Module::OpenSTDIN() : Module::OpenFile(fn));
            const string    dotfile = *(P->name) + "_tree.dot";
            tree->graphViz(dotfile);

            Syntax::Analyzer A;
            A.walk(*tree);
        }

        std::cerr << "OutputIL=" << out_bytes << std::endl;
        {
            Y_CHECK(ios::local_file::length_of("il.bin")==out_bytes);
        }

    }
}
Y_UTEST_DONE()

