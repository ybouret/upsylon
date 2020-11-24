#include "y/json/value.hpp"
#include "y/json/parser.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/serialized.hpp"

using namespace upsylon;

Y_UTEST(value)
{
    {
        JSON::Value  v0;
        JSON::Value  v1 = JSON::NullType;
        JSON::Value  v2 = JSON::TrueType;
        JSON::Value  v3 = JSON::FalseType;
        JSON::Value  v4 = 4.0;
        JSON::Value  v5 = "Hello";
        const string world = "World";
        JSON::Value  v6 = world;

        {
            v0 = JSON::NullType;
            v0 = JSON::TrueType;
            v0 = JSON::FalseType;
            v0 = 1;
            v0 = "Hello";
            v0 = world;
        }
    }

    JSON::Parser jsp;
    if(argc>1)
    {
        Jive::XNode::Pointer xnode( jsp.parseFile(argv[1]));
        xnode->graphViz("json-tree.dot");
    }

#if 0
    JSON::Compiler json;
    json.graphViz("json.dot");
    if(argc>1)
    {
        auto_ptr<Jargon::XNode> tree = json.parseFile(argv[1]);
        tree->save_to("json_tree.bin");
        {
            auto_ptr<Jargon::XNode> tree2 = json.loadTreeFromFile("json_tree.bin");
            Y_CHECK(ios::serialized::are_same_binary(*tree, *tree2));
        }
        tree->graphViz("json_tree.dot");
        json.browse(*tree);
        ios::ocstream fp( ios::cstderr );
        json.value.display(fp,0);
        fp << '\n';
    }
#endif

}
Y_UTEST_DONE()

