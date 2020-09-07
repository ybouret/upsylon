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
    typedef memory::pooled           Allocator; //!< local memory
    typedef vector<string,Allocator> Strings;   //!< temporary strings
    typedef suffix_tree<string>      Deps;      //!< database of deps


    class Node : public object, public inode<Node>
    {
    public:
        typedef core::list_of_cpp<Node> List;
        const string portName;
        List         children;
        
        //----------------------------------------------------------------------
        //
        //! create a new node with a root
        //
        //----------------------------------------------------------------------
        inline explicit Node(Deps         &deps,
                             const string &name,
                             const string &vars ) :
        portName(name),
        children()
        {
            //------------------------------------------------------------------
            // find local dependencies
            //------------------------------------------------------------------
            Deps localDeps;
            {
                const string cmd = "port deps " + portName + vars;
                parseDeps(localDeps,cmd);
            }
            
            //------------------------------------------------------------------
            // remove already gathered
            //------------------------------------------------------------------
            alreadyInstalled(localDeps,deps);
            
            //------------------------------------------------------------------
            // check currently installed
            //------------------------------------------------------------------
            checkInstalled(localDeps);
            
            //------------------------------------------------------------------
            // merge
            //------------------------------------------------------------------
            for(Deps::iterator i=localDeps.begin();i!=localDeps.end();++i)
            {
                const string &dep = *i;
                std::cerr << "|_" << dep << std::endl;
                if(!deps.insert_by(dep,dep))
                {
                    throw exception("unexpected multiple '%s'", *dep);
                }
            }
            
            //------------------------------------------------------------------
            // process
            //------------------------------------------------------------------
            for(Deps::iterator i=localDeps.begin();i!=localDeps.end();++i)
            {
                const string &dep = *i;
                children.push_back( new Node(deps,dep,vars) );
            }
            
        }

        void alreadyInstalled(Deps &localDeps, const Deps &deps)
        {
            Strings already;
            for(Deps::iterator i=localDeps.begin();i!=localDeps.end();++i)
            {
                const string &dep = *i;
                if(deps.has(dep)) already.push_back(dep);
            }
            while( already.size() )
            {
                localDeps.no( already.back() );
            }
        }
      

        inline virtual ~Node() throw()
        {

        }

        inline void parseDeps(Deps &deps, const string &cmd)
        {
            Strings output;
            ios::posix_command::query(output,cmd);
            for(size_t i=output.size();i>0;--i)
            {
                addDeps(deps,output[i]);
            }
            deps.sort_with(string::compare);
        }
        
        inline void addDeps(Deps &deps, const string &line)
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
                Strings lines;
                ios::posix_command::query(lines,cmd);
                for(size_t i=lines.size();i>1;--i)
                {
                    tokenizer<char> tkn(lines[i]);
                    if( tkn.next_with(' ') )
                    {
                        const string guess(tkn.token(),tkn.units());
                        deps.no(guess);
                    }
                }
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
        Deps           deps;
        {
            auto_ptr<Node> root = new Node(deps,portName,portVars);
        }
        
    }

}
Y_PROGRAM_END()

