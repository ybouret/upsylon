
#include "y/lang/dynamo/compiler.hpp"
#include "y/exception.hpp"



#if 0
namespace upsylon
{
    namespace Lang
    {
        void DynamoCompiler:: decl(XNode &node)
        {
            const string id = node.rule.name;
            std::cerr << "decl: " << id << std::endl;
            const int h = top_h(id);
            std::cerr << "h=" << h << std::endl;
            switch(h)
            {
                case 0: assert("dynamo"==id); {
                    declModule(node);
                }  break;

                case 1: assert("rule"==id); {
                    declRule(node);
                } break;

                default:
                    break;
            }
        }

        string * DynamoCompiler:: getModuleName( const XNode *node ) const
        {
            assert(node);
            static const char fn[] = "DynamoCompiler::getModuleName";
            const  string    &id   = node->rule.name;
            if("module" != id)  throw exception("%s(<%s>!=<module>)",    fn,*id);
            if(!node->terminal) throw exception("%s(internal node <%s>)",fn,*id);
            const string s = node->lexeme().to_string(1,0);
            return new string(s);
        }

        void DynamoCompiler:: declModule(XNode &node)
        {
            static const char fn[] = "DynamoCompiler::declModule";
            assert("dynamo"==node.rule.name);
            if(!node.internal) throw exception("%s(terminal <dynamo>)", fn );
            XNode::List &self = node.children();
            if(self.size<=0)   throw exception("%s(empty node <dynamo>)", fn);
            XNode       *sub  = self.head;       assert(sub);
            const Tag    mid  = getModuleName(sub);
            std::cerr << "ENTER: moduleID=" << mid << std::endl;
            modules.push_back(mid);
            for(sub=sub->next;sub;sub=sub->next)
            {
                decl(*sub);
            }
            modules.pop_back();
            std::cerr << "LEAVE: moduleID=" << mid << std::endl;
        }

        void DynamoCompiler:: declRule(XNode &node)
        {
            static const char fn[] = "DynamoCompiler::declRule";
            assert(node.rule.name=="rule");
            if(!node.internal) throw exception("%s(terminal <rule>)", fn );
            XNode::List &self = node.children();
            if(self.size<=0)   throw exception("%s(empty node <rule>",fn);
            
        }

    }
}

#endif


namespace upsylon
{
    namespace Lang
    {
        DynamoNode:: ~DynamoNode() throw()
        {
            assert(impl);
            switch(type)
            {
                case DynamoInternal:
                    delete static_cast<DynamoList *>(impl);
                    break;

                case DynamoTerminal:
                    delete static_cast<string*>(impl);
                    break;
            }
            impl=0;
        }

        DynamoNode:: DynamoNode( const string &id, const Lexeme &lx, const size_t nskip, const size_t ntrim) :
        type( DynamoTerminal ),
        name(id),
        impl( NULL )
        {
            const string s = lx.to_string(nskip,ntrim);
            impl = new string(s);
        }

        const string & DynamoNode:: content() const throw()
        {
            assert(DynamoTerminal==type);
            assert(NULL!=impl);
            return *static_cast<const string *>(impl);
        }

        DynamoList   &  DynamoNode:: children() throw()
        {
            assert(DynamoInternal==type);
            assert(NULL!=impl);
            return *static_cast<DynamoList *>(impl);
        }


        DynamoNode:: DynamoNode( const string &id ) :
        type( DynamoInternal ),
        name(id),
        impl( new DynamoList() )
        {

        }

    }
}


namespace upsylon
{
    namespace Lang
    {

        void DynamoCompiler:: compile( const XNode &node )
        {
            created=0;
            items.clear();
            walk(node);
            std::cerr << "#items  =" << items.size << std::endl;
            std::cerr << "#created=" << created    << std::endl;
        }

        void DynamoCompiler:: onTerminal(const string &id, const Lexeme &lx)
        {
            Syntax::Analyzer::onTerminal(id,lx);
            size_t nskip = 0;
            size_t ntrim = 0;
            items.push_back( new DynamoNode(id,lx,nskip,ntrim) );
            ++created;
        }


        void DynamoCompiler:: onInternal(const string &id, const size_t sz, const string *data)
        {
            Syntax::Analyzer::onInternal(id,sz,data);
            if(sz>items.size) throw exception("DynamoCompiler: corrupted internal stack!!!");
            DynamoNode *node = new DynamoNode(id);
            DynamoList &self = node->children();

            for(size_t i=sz;i>0;--i)
            {
                self.push_front( items.pop_back() );
            }
            items.push_back(node);

            ++created;
        }
    }

}
