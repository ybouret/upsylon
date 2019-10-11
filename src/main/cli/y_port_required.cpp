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

