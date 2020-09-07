#include "y/program.hpp"
#include "y/core/list.hpp"
#include "y/core/inode.hpp"
#include "y/ios/tools/posix-command.hpp"
#include "y/ptr/auto.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/tokenizer.hpp"
#include "y/associative/suffix/tree.hpp"
#include "y/memory/allocator/pooled.hpp"

using namespace upsylon;

namespace
{
    typedef memory::pooled           Allocator;
    typedef vector<string,Allocator> Strings;
    typedef suffix_tree<string>      Deps;


    class Node : public object, public inode<Node>
    {
    public:
        typedef core::list_of_cpp<Node> List;
        const Node  *root;
        const string portName;
        List         children;

        inline explicit Node(const Node   *node,
                             const string &name,
                             const string &vars ) :
        root(node),
        portName(name),
        children()
        {
            Deps           deps;
            {
                string         cmd = "port deps " + portName + vars;
                Strings        lines;
                ios::posix_command::query(lines,cmd);
                for(size_t i=lines.size();i>0;--i)
                {
                    processLine(lines[i],deps);
                }
            }

            deps.sort_with(string::compare);
            checkInstalled(deps);

            {
                Strings already;
                for(Deps::iterator i=deps.begin();i!=deps.end();++i)
                {
                    const string &dep = *i;
                    if(root&&root->find(dep))
                    {
                        already.push_back(dep);
                    }
                }
                std::cerr << "already " << already << std::endl;
                for(size_t i=already.size();i>0;--i)
                {
                    deps.no(already[i]);
                }
            }




        }


        bool find(const string &id) const throw()
        {
            for(const Node *node=children.head;node;node=node->next)
            {
                if(node->find(id)) return true;
            }
            return id==portName;
        }


        inline virtual ~Node() throw()
        {

        }


        inline void processLine(const string &line, Deps &deps)
        {
            static const char   dep[] = "Dependencies:";
            static const size_t len   = sizeof(dep)/sizeof(dep[0])-1;
            const char         *org = *line;
            size_t              rem = line.size();
            {
                bool        found = false;

                while(rem>len)
                {
                    if(0==memcmp(org,dep,len))
                    {
                        found = true;
                        break;
                    }
                    ++org;
                    --rem;
                }
                if(!found) return;
                org+=len;
                string  data = org;
                Strings subs;
                tokenizer<char>::split_with(subs,data,',');
                for(size_t i=subs.size();i>0;--i)
                {
                    string &sub = subs[i];
                    sub.clean_with(' ');
                    (void) deps.insert_by(sub,sub);
                }
            }
        }

        inline void checkInstalled( Deps &deps )
        {
            if(deps.entries())
            {
                string cmd = "port installed";
                for(Deps::iterator i=deps.begin();i!=deps.end();++i)
                {
                    cmd << ' ' << *i;
                }
                std::cerr << "executing " << cmd << std::endl;
                Strings lines;
                ios::posix_command::query(lines,cmd);
                for(size_t i=lines.size();i>1;--i)
                {
                    tokenizer<char> tkn(lines[i]);
                    if( tkn.next_with(' ') )
                    {
                        const string guess(tkn.token(),tkn.units());
                        //std::cerr << "found <" << guess << ">" << std::endl;
                        (void) deps.remove_by(guess);
                    }
                }
                std::cerr << "#remaining: " << deps.entries() << std::endl;
            }
        }



    private:
        Y_DISABLE_COPY_AND_ASSIGN(Node);
    };

}

Y_PROGRAM_START()
{


    if(argc>1)
    {

        const string portName = argv[1];
        string       portVars;
        for(int i=2;i<argc;++i)
        {
            portVars << ' ' << argv[i];
        }
        const string portFull =  portName + portVars;
        std::cerr << "looking for " << portFull << std::endl;
        auto_ptr<Node> root = new Node(NULL,portName,portVars);

    }

}
Y_PROGRAM_END()

