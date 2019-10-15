
#include "y/program.hpp"
#include "y/string.hpp"
#include "y/sequence/list.hpp"
#include "y/ios/tools/posix-command.hpp"
#include "y/core/list.hpp"
#include "y/core/node.hpp"
#include "y/ptr/auto.hpp"
#include "y/ordered/sorted-vector.hpp"
#include "y/string/tokenizer.hpp"

using namespace upsylon;

namespace {

    typedef list<string>    strings;
    class Port;
    typedef core::inode<Port>          PortNode;
    typedef core::list_of_cpp<Port>    Ports;
    typedef sorted_vector<string>      Collection;
    typedef ordered_single<Collection> Required;
    typedef ordered_single<Collection> Installed;

    class Port : public object, public PortNode
    {
    public:
        Port          *parent;
        const size_t   level;
        const string   name;
        Ports          children;

        inline explicit Port(Port          *p,
                             const size_t   l,
                             const string  &n) :
        object(), PortNode(), parent(p), level(l), name(n), children()
        {
        }

        inline virtual ~Port() throw()
        {
        }

        inline void display() const
        {
            indent(); std::cout << "|_" << name << std::endl;
            for(const Port *p=children.head;p;p=p->next)
            {
                p->display();
            }
        }

        inline void indent() const
        {
            for(size_t i=level;i>0;--i)
            {
                std::cout << ' ';
            }
        }

        void remove_already( const Installed &installed ) throw()
        {
            for(Port *child=children.head;child;child=child->next)
            {
                child->remove_already(installed);
            }

            Ports temp;
            while( children.size )
            {
                Port *child = children.pop_front();
                if( installed.search(child->name) )
                {
                    delete child;
                }
                else
                {
                    temp.push_back( child );
                }
            }

            children.swap_with(temp);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Port);
    };

    static inline size_t LevelOf( const string &line ) throw()
    {
        size_t   len = line.length();
        size_t   ans = 0;
        while(ans<len&&isblank(line[ans]))
        {
            ++ans;
        }
        return ans;
    }

    static inline size_t Parse( string &line, Required &required ) throw()
    {
        const size_t level = LevelOf(line);
        line.skip(level);
        std::cerr << "[" << line << "]@" << level << std::endl;
        required.insert(line);
        return level;
    }

    static inline bool IsNoPort(const string &line) throw()
    {
        return LevelOf(line)<=0;
    }


    enum Kind
    {
        Child,
        Parent,
        Sibling
    };

    static inline Kind KindFor( const Port *curr, const size_t level )
    {
        assert(curr);
        if(level>curr->level)
        {
            return Child;
        }
        else if(level<curr->level)
        {
            return Parent;
        }
        else return Sibling;
    }

    static inline void QueryInstalled(Installed      &installed,
                                      Required       &required )
    {
        if(required.size())
        {
            strings lines;
            {
                string cmd = "port installed";
                for(Required::iterator i=required.begin();i!=required.end();++i)
                {
                    cmd << ' ' << *i;
                }
                ios::posix_command::query(lines,cmd);
                if(lines.size())
                {
                    lines.pop_front();
                    for(strings::iterator i=lines.begin();i!=lines.end();++i)
                    {
                        tokenizer<char> tkn( *i );
                        if( tkn.next_with(' ') )
                        {
                            const string id = tkn.to_string();
                            //std::cerr << "-> " << id << std::endl;
                            required.no(id);
                            installed.insert(id);
                        }
                    }
                }
            }

        }
    }




}

Y_PROGRAM_START()
{
    if( argc < 2 )
    {
        std::cerr << "usage: " << program << " [port [variants]]" << std::endl;
        return 0;
    }

    // assemble portName + variants
    const string portName = argv[1];
    string fullPortName = portName;
    for(int i=2;i<argc;++i)
    {
        fullPortName << ' ' << argv[i];
    }

    // query full rdeps
    strings lines;
    {
        const string cmd = "port rdeps " + fullPortName;
        ios::posix_command::query(lines,cmd);
    }

    // remove bad lines
    lines.remove_if(IsNoPort);

    // build root
    Port          *curr = new Port(0,0,portName);
    auto_ptr<Port> root = curr;
    Required       required(  lines.size()+1, as_capacity );
    Installed      installed( lines.size()+1, as_capacity );
    required.insert(portName);

    // build tree
    for(strings::iterator it=lines.begin();it!=lines.end();++it)
    {
        assert(curr);
        string      &content = *it;
        const size_t level   = Parse(content,required);
        switch( KindFor(curr,level) )
        {
            case Child:    curr->children.push_back( new Port(curr,level,content) ); curr = curr->children.tail;          break;
            case Parent:   assert(curr->parent); curr=curr->parent;
            case Sibling:  assert(curr->parent); curr->children.push_back( new Port(curr->parent,level,content ) );  break;
        }
    }

    root->display();
    //std::cerr << "required=" << required << std::endl;

    // query all installed from required
    QueryInstalled(installed,required);
    //std::cerr << "installed: " << installed << std::endl;
    //std::cerr << "required : " << required  << std::endl;
    root->remove_already(installed);
    std::cerr << "Tree: " << std::endl;
    root->display();
    std::cerr << "required : " << required  << std::endl;


}
Y_PROGRAM_END()



#if 0

#include "y/program.hpp"
#include "y/string.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/pooled.hpp"
#include "y/fs/local/fs.hpp"
#include "y/ios/icstream.hpp"
#include "y/string/tokenizer.hpp"
#include "y/lang/pattern/matching.hpp"
#include "y/ordered/sorted-vector.hpp"

#include <cstdlib>

using namespace upsylon;

namespace {

    typedef memory::pooled                               Allocator;
    typedef vector<string,Allocator>                     Strings;
    typedef increasing_comparator<string>                Comparator;
    typedef sorted_vector<string,Comparator,Allocator>   Collection;
    typedef ordered_single<Collection>                   Present;
    typedef ordered_unique<Collection>                   Missing;


    //==========================================================================
    //
    // Execute a command and return lines of output
    //
    //==========================================================================
    static inline
    void load(Strings      &lines,
              const string &cmd )
    {
        vfs         &fs  = local_fs::instance();
        lines.free();
        const string tmp = fs.get_temporary_name();
        try
        {
            const string run = cmd + " &> " + tmp;
            if( 0 != system( *run ) )
            {
                throw exception("unable to execute <%s>", *run );
            }

            {
                ios::icstream fp(tmp);
                string line;
                while( fp.gets(line) )
                {
                    lines << line;
                }
            }

            fs.try_remove_file(tmp);
        }
        catch(...)
        {
            fs.try_remove_file(tmp);
            throw;
        }
    }

    //==========================================================================
    //
    // - convert a line of separated port names into a missing database
    // - return true if something is missing
    //
    //==========================================================================
    static inline bool WS(const int C) throw() { return C==' ' || C=='\t'; }

    static inline bool ExtractRequired(Missing      &required,
                                       const string &info )
    {
        bool ans = false;
        {
            tokenizer<char> tkn(info);
            while( tkn.next_with(',') )
            {
                string tmp( tkn.token(), tkn.units() );
                tmp.clean(WS);
                if( required.insert(tmp) )
                {
                    ans = true;
                }
            }
        }
        return ans;
    }


    //==========================================================================
    //
    //
    // - query all the depedencies of portName, and fill the required database
    // - return true if something is missing
    //
    //==========================================================================
    static inline bool QueryRequired(Missing      &required,
                                     const string &portName)
    {
        bool ans = false;
        required.free();
        {
            Strings lines;
            //------------------------------------------------------------------
            //
            // get port deps output
            //
            //------------------------------------------------------------------
            {
                const string cmd = "port deps " + portName;
                load(lines,cmd);
            }

            //------------------------------------------------------------------
            //
            // parse each line
            //
            //------------------------------------------------------------------
            Lang::Matching match("[D|d]ependencies");
            for(size_t i=1;i<=lines.size();++i)
            {
                Strings info(2,as_capacity);
                tokenizer<char>::split_with(info, lines[i], ':' );
                if(info.size()!=2)          continue; //! not the good format
                if( !match.partly(info[1])) continue; //! not a dependency line
                if( ExtractRequired(required,info[2]) )
                {
                    ans = true;
                }
            }
        }
        return ans;
    }



    //==========================================================================
    //
    // query presence
    //
    //==========================================================================
    static inline void QueryPresent(Present &present, const string &ports)
    {
        Strings lines;
        {
            const string cmd = "port installed " + ports;
            load(lines,cmd);
        }

        for(size_t i=2;i<=lines.size();++i)
        {
            tokenizer<char> tkn( lines[i] );
            if( !tkn.next_with(' ') ) continue;
            const string id = tkn.to_string();
            present.insert(id);
        }

    }

    static std::ostream & indent( std::ostream &os, int level )
    {

        if(level)
        {
            while(level-- > 0)
            {
                os << ' ';
            }
            os << "|_";
        }
        return os;
    }
    //==========================================================================
    //
    // installed are already know installed ports
    // required  are already known
    //
    //==========================================================================
    static inline void CheckRequired(const string &portName,
                                     Present      &present,
                                     Missing      &missing,
                                     const int     level = 0 )
    {
        indent(std::cerr,level) << "<"  << portName << ">" << std::endl;

        //----------------------------------------------------------------------
        //
        // query all required dependecies
        //
        //----------------------------------------------------------------------
        Missing    required;
        if(QueryRequired(required,portName))
        {
            assert(required.size()>0);

            required.exclude(present);
            required.exclude(missing);

            if(required.size()>0)
            {
                //--------------------------------------------------------------
                // collect ports
                //--------------------------------------------------------------
                {
                    string ports;
                    bool   first = true;
                    for(Collection::iterator i=required.begin(); i != required.end(); ++i)
                    {
                        if(first)
                        {
                            first = false;
                        }
                        else
                        {
                            ports << ' ';
                        }
                        ports << *i;
                    }
                    QueryPresent(present,ports);
                }


                required.exclude(present);

                //--------------------------------------------------------------
                // ok, these are the new required ports
                //--------------------------------------------------------------
                if(required.size()>0)
                {
                    const int nextLevel = level+1;
                    for(Collection::iterator i=required.begin(); i != required.end(); ++i)
                    {
                        const string &sub = *i;
                        if( missing.insert(sub) )
                        {
                             CheckRequired(sub,present,missing,nextLevel);
                        }
                    }
                }
            }

        }



    }





    
}

Y_PROGRAM_START()
{
    if( argc < 2 )
    {
        std::cerr << "usage: " << program << " [port [variants]]" << std::endl;
        return 0;
    }

    const string portName = argv[1];
    string fullPortName = portName;
    for(int i=2;i<argc;++i)
    {
        fullPortName << ' ' << argv[i];
    }

    Present present;
    QueryPresent(present,portName);
    if( present.search(portName) )
    {
        std::cerr << "<" << portName << ">*" << std::endl;
    }
    else
    {
        Missing missing;
        CheckRequired(fullPortName,present,missing);
        if(missing.size())
        {
            std::cerr << missing << std::endl;
        }
    }
}
Y_PROGRAM_END()

#endif

