
#include "y/program.hpp"
#include "y/string.hpp"
#include "y/sequence/list.hpp"
#include "y/ios/tools/posix-command.hpp"
#include "y/core/list.hpp"
#include "y/core/node.hpp"
#include "y/ptr/auto.hpp"
#include "y/ordered/sorted-vector.hpp"
#include "y/string/tokenizer.hpp"
#include "y/type/aliasing.hpp"

using namespace upsylon;

namespace {

    typedef list<string>               strings;
    class                              Port;
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
                    temp.merge_back(child->children);
                    delete child;
                }
                else
                {
                    temp.push_back( child );
                }
            }

            children.swap_with(temp);
        }


        void clean() throw()
        {
            const size_t nextLevel = level+1;
            for(Port *child=children.head;child;child=child->next)
            {
                aliasing::_(child->level) = nextLevel;
                child->clean();
            }
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
                            installed.insert(id);
                        }
                    }
                }
            }
        }
        required.exclude(installed);
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
    installed.insert( "xattr" );

    // build tree
    for(strings::iterator it=lines.begin();it!=lines.end();++it)
    {
        assert(curr);
        string      &content = *it;
        const size_t level   = Parse(content,required);
        switch( KindFor(curr,level) )
        {
            case Child:    curr->children.push_back( new Port(curr,level,content) ); curr = curr->children.tail;    break;
            case Parent:   assert(curr->parent); curr=curr->parent; /* FALLTHRU */
            case Sibling:  assert(curr->parent); curr->children.push_back( new Port(curr->parent,level,content ) );  break;
        }
    }

    //root->display();

    // query all installed from required
    QueryInstalled(installed,required);
    root->remove_already(installed);
    if(required.size())
    {
        //root->clean();
        root->display();
        std::cout << required  << std::endl;
    }

}
Y_PROGRAM_END()



