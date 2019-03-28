#include "y/lang/dynamo/manager.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(manager)
{
    DynamoManager &mgr = DynamoManager::instance();
    
    if(argc>1)
    {
        const string fileName = argv[1];
        
        // make an XNode
        auto_ptr<XNode> xnode = mgr.loader().load( Module::OpenFile(fileName) );
        
        const size_t xnodeOutBytes = xnode->outputBytes();
        size_t       xnodeCount    = 0;
        xnode->save("xnode.bin", &xnodeCount);
        const string xnodeCode = xnode->toBinary();
       
        std::cerr << "xnodeOutBytes="<< xnodeOutBytes << std::endl;
        Y_CHECK(xnodeOutBytes==xnodeCount);
        Y_CHECK(xnodeOutBytes==ios::local_file::length_of("xnode.bin"));
        Y_CHECK(xnodeOutBytes==xnodeCode.size());
       
        // reload from file
        {
            auto_ptr<XNode> reload = XNode::Load( Module::OpenFile("xnode.bin"), mgr.loader() );
            const string reloaded_file = reload->toBinary();
            Y_CHECK(xnodeCode==reloaded_file);
        }
        
        // reload from data
        {
            auto_ptr<XNode> reload = XNode::Load( Module::OpenData("xnode",xnodeCode), mgr.loader() );
            const string reloaded_data = reload->toBinary();
            Y_CHECK(xnodeCode==reloaded_data);
        }
    }
}
Y_UTEST_DONE()

