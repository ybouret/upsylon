
#include "y/lang/dynamo/compiler.hpp"

namespace upsylon
{
    namespace Lang
    {
        DynamoList:: DynamoList(const string *s) throw() :
        core::list_of_cpp<DynamoNode>(),
        data(s)
        {
            if(data)
            {
                assert(data->refcount()>0);
                ( (string *)data )->withhold();
            }
        }
        
        DynamoList:: ~DynamoList() throw()
        {
            if(data)
            {
                string *s = (string *)data;
                if(s->liberate())
                {
                    delete s;
                }
                data = 0;
            }
        }
        
        DynamoList:: DynamoList( const DynamoList &other ) :
        core::list_of_cpp<DynamoNode>(other),
        data(other.data)
        {
            if(data)
            {
                assert(data->refcount()>0);
                ( (string *)data )->withhold();
            }
        }
    }
}

namespace upsylon
{
    namespace Lang
    {
        DynamoNode:: DynamoNode(const string    &id,
                                const Lexeme    &lx,
                                const size_t     nskip,
                                const size_t     ntrim) :
        type(DynamoTerminal),
        name(id),
        impl(NULL)
        {
            const string s = lx.to_string(nskip,ntrim);
            impl = new string(s);
        }
        
        DynamoNode:: DynamoNode(const string &id, const string *s):
        type(DynamoInternal),
        name(id),
        impl( new DynamoList(s) )
        {
        }
        

        
        DynamoNode:: ~DynamoNode() throw()
        {
            assert(impl);
            
            switch (type)
            {
                case DynamoTerminal:
                    delete static_cast<string *>(impl);
                    break;
                    
                case DynamoInternal:
                    delete static_cast<DynamoList*>(impl);
                    break;
            }
        }
        
        DynamoNode:: DynamoNode( const DynamoNode &other ) :
        type(other.type),
        name(other.name),
        impl(NULL)
        {
            switch (type)
            {
                case DynamoTerminal:
                    impl = new string( *static_cast<const string *>(other.impl) );
                    break;
                    
                case DynamoInternal:
                    impl = new DynamoList( other.children() );
                    break;
            }
        }
        
        
        DynamoList & DynamoNode:: children() throw()
        {
            assert(type==DynamoInternal);
            assert(impl);
            return *static_cast<DynamoList *>(impl);
        }
        
        
        const DynamoList & DynamoNode:: children() const throw()
        {
            assert(type==DynamoInternal);
            assert(impl);
            return *static_cast<const DynamoList *>(impl);
        }
    }
}

#include "y/exception.hpp"
namespace upsylon
{
    namespace Lang
    {
        
        DynamoCompiler:: DynamoCompiler() :
        DynamoLoader(),
        Syntax::Analyzer(),
        items(NULL)
        {
        }
        
        DynamoCompiler:: ~DynamoCompiler() throw()
        {
        }
        
        
        DynamoNode * DynamoCompiler:: IL( const XNode &top )
        {
            items.clear();
            created = 0;
            walk(top);
            std::cerr << "#items  =" << items.size << std::endl;
            std::cerr << "#created=" << created    << std::endl;
            return NULL;
        }
        
        void DynamoCompiler:: onTerminal( const string &id, const Lexeme &lx )
        {
            std::cerr << "dynamo.TERMINAL@"; Analyzer::onTerminal(id,lx);
            ++created;
            items.push_back( new DynamoNode(id,lx) );
            
        }
        
        void DynamoCompiler:: onInternal( const string &id, const size_t  sz, const string *data)
        {
            std::cerr << "dynamo.INTERNAL@"; Analyzer::onInternal(id,sz,data);
            if(sz>items.size) throw exception("{%s} missing items",**name);
            auto_ptr<DynamoNode> node = new DynamoNode(id,data);
            DynamoList          &self = node->children();
            ++created;
            for(size_t i=sz;i>0;--i)
            {
                self.push_front( items.pop_back() );
            }
            items.push_back( node.yield() );
        }

        
    }
}
