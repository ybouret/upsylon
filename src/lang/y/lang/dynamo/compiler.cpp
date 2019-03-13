#include "y/lang/dynamo/compiler.hpp"
#include "y/exception.hpp"
#include "y/string/convert.hpp"

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

        DynamoNode:: DynamoNode(const string &id,
                                const Lexeme &lx,
                                const size_t  nskip,
                                const size_t  ntrim) :
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

        const DynamoList   &  DynamoNode:: children() const throw()
        {
            assert(DynamoInternal==type);
            assert(NULL!=impl);
            return *static_cast<const DynamoList *>(impl);
        }


        DynamoNode:: DynamoNode( const string &id ) :
        type( DynamoInternal ),
        name(id),
        impl( new DynamoList() )
        {

        }

        std::ostream & DynamoNode:: display( std::ostream &os, int level ) const
        {
            switch(type)
            {
                case DynamoTerminal: {
                    const string s = string_convert::to_visible( content() );
                    Indent(os,level) << "[term] <" << name << ">";
                    if(s.size()>0)
                    {
                        os << "='" << s << "'";
                    }
                    os << std::endl;
                } break;

                case DynamoInternal: {
                    const DynamoList &self = children();
                    Indent(os,level) << "[call] <" << name << ">/" << self.size << std::endl;
                    ++level;
                    for(const DynamoNode *node = self.head; node; node=node->next )
                    {
                        node->display(os,level);
                    }
                    --level;
                } break;
            }
            return os;
        }

        std::ostream & DynamoNode:: Indent(std::ostream &os, int level)
        {
            const string s = vformat("x%02x",level);
            os << '|' << s << '|';
            for(;level>0;--level)
            {
                os << '_' << '_';
            }
            return os;
        }
    }
}



namespace upsylon
{
    namespace Lang
    {

        static const char *lxkw[] =
        {
            "module", //0
            "lid",    //1
            "cid",    //2
            "rx",     //3
            "rs"      //4
        };
        
        DynamoCompiler:: DynamoCompiler() :
        Syntax::Analyzer(),
        created(0),
        items(),
        lxh( Y_MPERF_FOR(lxkw) )
        {
        }

        DynamoCompiler::~DynamoCompiler() throw()
        {
        }

    }

}


namespace upsylon
{
    namespace Lang
    {

        DynamoNode *DynamoCompiler:: compile( const XNode &node )
        {
            (size_t&)created=0;
            items.clear();
            walk(node);
            std::cerr << "#items  =" << items.size << std::endl;
            std::cerr << "#created=" << created    << std::endl;

            if(1!=items.size)
            {
                throw exception("DynamoCompiler: corrupted Syntax Node <%s>", *(node.rule.name));
            }
            return items.pop_front();
        }

        void DynamoCompiler:: onTerminal(const string &id, const Lexeme &lx)
        {
            //Syntax::Analyzer::onTerminal(id,lx);
            size_t nskip = 0;
            size_t ntrim = 0;
            switch( lxh(id) )
            {
                case 0: assert("module"==id); nskip=1; break;
                case 1: assert("lid"==id);    nskip=1; break;
                case 2: assert("cid"==id);    nskip=1; break;
                case 3: assert("rx"==id);     nskip=1; ntrim=1; break;
                case 4: assert("rs"==id);     nskip=1; ntrim=1; break;

                default: break; // take the full lexeme
            }
            
            items.push_back( new DynamoNode(id,lx,nskip,ntrim) );
            ++(size_t&)created;

            indent(std::cerr) << "[push] <" << id << ">";
            const string &content = items.tail->content();
            if(content.size()>0)
            {
                const string s = string_convert::to_visible(content);
                std::cerr << "='" << s << "'";
            }
            std::cerr << std::endl;

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

            ++(size_t&)created;
        }
    }

}

