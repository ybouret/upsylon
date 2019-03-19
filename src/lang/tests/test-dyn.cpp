#include "y/lang/dynamo/loader.hpp"
#include "y/lang/dynamo/generator.hpp"

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


    if(argc>1)
    {
        const string           fn = argv[1];
        auto_ptr<Syntax::Node> g  = dynamo.load( doOpen(fn) );

        if(false)
        {
            g->graphViz( "dynamo_tree.dot" );
            g->save("dynamo.bin");

            {
                std::cerr << "Reloading" << std::endl;
                auto_ptr<Syntax::Node> g2 = Syntax::Node::Load( Module::OpenFile("dynamo.bin"),dynamo );
                g2->graphViz("dynamo_tree2.dot");
            }
        }

        auto_ptr<DynamoNode> il = dynCmp.compile(*g);
        std::cerr << "IntermediateLang=" << std::endl;
        std::cerr << il;

        {
            ios::ocstream fp("il.bin");
            il->save(fp);
        }

        il->graphViz("il0.dot");
        auto_ptr<Syntax::Parser> P = dynGen.build( *il );
        //il->graphViz("il1.dot");


        if(false)
        {
            MatchString cut = "lxr|plg|cmd|aka";
            XNode::RemoveFrom(*g,cut);
            g->graphViz( "dynamo_cut.dot" );
        }

        if(argc>2)
        {
            const string    fn = argv[2];
            auto_ptr<XNode> tree = P->run((fn=="STDIN") ? Module::OpenSTDIN() : Module::OpenFile(fn));
            const string    dotfile = *(P->name) + "_tree.dot";
            tree->graphViz(dotfile);
        }


    }
}
Y_UTEST_DONE()

