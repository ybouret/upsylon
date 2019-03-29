#include "y/lang/dynamo/manager.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(manager)
{
    std::cerr << "sizeof(DynamoLoader)     = " << sizeof(DynamoLoader)      << std::endl;
    std::cerr << "sizeof(DynamoCompiler)   = " << sizeof(DynamoCompiler)    << std::endl;
    std::cerr << "sizeof(DynamoGenerator)  = "  << sizeof(DynamoGenerator)  << std::endl;
    std::cerr << "sizeof(DynamoManager)    = "  << sizeof(DynamoManager)    << std::endl;
    
    
    DynamoManager &mgr = DynamoManager::instance();
    //mgr.loader().setVerbose(false);
    
    if(argc>1)
    {
        const string fileName = argv[1];
        
        // make an XNode
        auto_ptr<XNode> xnode     = mgr.loader().load( Module::OpenFile(fileName) );
        const string    xnodeCode = xnode->toBinary();
        {
            const size_t xnodeOutBytes = xnode->outputBytes();
            size_t       xnodeCount    = 0;
            xnode->save("xnode.bin", &xnodeCount);
            
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
        
        // make a DynamoNode
        auto_ptr<DynamoNode> dnode     = mgr.compiler().compile(*xnode);
        const string         dnodeCode = dnode->toBinary();
        
        {
            const size_t dnodeOutBytes = dnode->outputBytes();
            size_t       dnodeCount    = 0;
            dnode->save("dnode.bin", &dnodeCount);
            
            std::cerr << "dnodeOutBytes="<< dnodeOutBytes << std::endl;
            Y_CHECK(dnodeOutBytes==dnodeCount);
            Y_CHECK(dnodeOutBytes==ios::local_file::length_of("dnode.bin"));
            Y_CHECK(dnodeOutBytes==dnodeCode.size());
            
            // reload from file
            {
                auto_ptr<DynamoNode> reload = DynamoNode:: Load( Module::OpenFile("dnode.bin") );
                const string reloaded_file = reload->toBinary();
                Y_CHECK(dnodeCode==reloaded_file);
            }
            
            // reload fron data
            {
                auto_ptr<DynamoNode> reload = DynamoNode:: Load( Module::OpenData("done",dnodeCode) );
                const string reloaded_data = reload->toBinary();
                Y_CHECK(dnodeCode==reloaded_data);
            }
        }
        
        // create a resource file
        {
            ios::ocstream fp("xrc.bin");
            for( size_t i = 1 + alea.leq(10); i>0 ; --i)
            {
                fp << alea.range('A', 'Z');
            }
            fp << '\n';
        }
        
        {
            ios::rc::writer rc("xrc.bin");
            rc.append_data("xnode",xnodeCode);
            rc.append_data("dnode",dnodeCode);
            rc.finalize();
        }
        
        {
            ios::rc::loader rc("xrc.bin");
            
            {
                auto_ptr<XNode> reload = XNode::Load( Module::OpenResource(rc,"xnode"),mgr.loader());
                const string reloaded_rc = reload->toBinary();
                Y_CHECK(xnodeCode==reloaded_rc);
            }
            
            {
                auto_ptr<DynamoNode> reload = DynamoNode::Load( Module::OpenResource(rc,"dnode") );
                const string reloaded_rc = reload->toBinary();
                Y_CHECK(dnodeCode==reloaded_rc);
            }
        }
        
        
    }
}
Y_UTEST_DONE()

