#include "y/lang/dynamo/manager.hpp"
#include "y/fs/local/file.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(manager)
{
    Y_UTEST_SIZEOF(DynamoParser);
    Y_UTEST_SIZEOF(DynamoCompiler);
    Y_UTEST_SIZEOF(DynamoGenerator);
    Y_UTEST_SIZEOF(DynamoManager);


    
    
    DynamoManager &mgr = DynamoManager::instance();
    //mgr.loader().setVerbose(false);
    
    if(argc>1)
    {
        const string fileName = argv[1];
        
        // make an XNode
        auto_ptr<XNode> xnode     = mgr.loader().load( Module::OpenFile(fileName) );
        const string    xnodeCode = xnode->to_binary();
        {
            const size_t xnodeOutBytes = xnode->serialize_length();
            const size_t xnodeCount    = xnode->save_to("xnode.bin");
            
            std::cerr << "xnodeOutBytes="<< xnodeOutBytes << std::endl;
            Y_CHECK(xnodeOutBytes==xnodeCount);
            Y_CHECK(xnodeOutBytes==ios::local_file::length_of("xnode.bin"));
            Y_CHECK(xnodeOutBytes==xnodeCode.size());
            
            // reload from file
            {
                auto_ptr<XNode> reload = XNode::Load( Module::OpenFile("xnode.bin"), mgr.loader() );
                const string reloaded_file = reload->to_binary();
                Y_CHECK(xnodeCode==reloaded_file);
            }
            
            // reload from data
            {
                auto_ptr<XNode> reload = XNode::Load( Module::OpenData("xnode",xnodeCode), mgr.loader() );
                const string reloaded_data = reload->to_binary();
                Y_CHECK(xnodeCode==reloaded_data);
            }
            
            
        }
        
        // make a DynamoNode
        auto_ptr<DynamoNode> dnode     = mgr.compiler().compile(*xnode);
        const string         dnodeCode = dnode->to_binary();
        
        {
            const size_t dnodeOutBytes = dnode->serialize_length();
            const size_t dnodeCount    = dnode->save_to("dnode.bin");
            
            std::cerr << "dnodeOutBytes="<< dnodeOutBytes << std::endl;
            Y_CHECK(dnodeOutBytes==dnodeCount);
            Y_CHECK(dnodeOutBytes==ios::local_file::length_of("dnode.bin"));
            Y_CHECK(dnodeOutBytes==dnodeCode.size());
            
            // reload from file
            {
                auto_ptr<DynamoNode> reload = DynamoNode:: Load( Module::OpenFile("dnode.bin") );
                const string reloaded_file = reload->to_binary();
                Y_CHECK(dnodeCode==reloaded_file);
            }
            
            // reload fron data
            {
                auto_ptr<DynamoNode> reload = DynamoNode:: Load( Module::OpenData("done",dnodeCode) );
                const string reloaded_data = reload->to_binary();
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

#if 0
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
#endif
        
    }
}
Y_UTEST_DONE()

