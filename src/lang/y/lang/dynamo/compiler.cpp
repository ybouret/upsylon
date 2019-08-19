#include "y/lang/dynamo/compiler.hpp"
#include "y/exception.hpp"
#include "y/string/convert.hpp"
#include "y/string/io.hpp"
#include "y/ios/tools/graphviz.hpp"


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
            "rs",     //4
            "eid"     //5
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
            items.release();
            walk(node);
            if(1!=items.size)
            {
                throw exception("DynamoCompiler: corrupted Syntax Node <%s>", *(node.rule.name));
            }
            return items.pop_front();
        }

        void DynamoCompiler:: onTerminal(const string &id, const Lexeme &lx)
        {
            size_t nskip = 0;
            size_t ntrim = 0;
            switch( lxh(id) )
            {
                case 0: assert("module"==id); nskip=1; break;
                case 1: assert("lid"   ==id); nskip=1; break;
                case 2: assert("cid"   ==id); nskip=1; break;
                case 3: assert("rx"    ==id); nskip=1; ntrim=1; break;
                case 4: assert("rs"    ==id); nskip=1; ntrim=1; break;
                case 5: assert("eid"   ==id); nskip=1; break;

                default: break; // take the full lexeme
            }
            
            items.push_back( new DynamoNode(id,lx,nskip,ntrim) );
            ++(size_t&)created;

#if 0
            if(verbose)
            {
                indent(std::cerr) << "[push] <" << id << ">";
                const string &content = items.tail->content();
                if(content.size()>0)
                {
                    const string s = string_convert::to_visible(content);
                    std::cerr << "='" << s << "'";
                }
                std::cerr << std::endl;
            }
#endif
            
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
            if(self.tail->name == '^' )
            {
                delete self.pop_back();
            }
            
            items.push_back(node);

            ++(size_t&)created;
        }
    }

}

