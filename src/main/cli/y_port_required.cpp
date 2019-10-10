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

    typedef vector<string,memory::pooled>                strings;
    typedef increasing_comparator<string>                depcomp;
    typedef sorted_vector<string,depcomp,memory::pooled> collection_type;
    typedef ordered_single<collection_type>              collection;

    static inline
    void load(strings      &lines,
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

    static inline bool WS(const int C) throw() { return C==' ' || C=='\t'; }

    static inline void ExtractRequired(collection   &required,
                                        const string &info )
    {
        tokenizer<char> tkn(info);
        while( tkn.next_with(',') )
        {
            string tmp( tkn.token(), tkn.units() );
            tmp.clean(WS);
            required.insert(tmp);
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

    static inline void QueryRequired(collection   &required,
                                     const string &portName)
    {
        required.free();
        strings lines;
        // get port deps output
        {
            const string cmd = "port deps " + portName;
            load(lines,cmd);
        }

        // parse each line
        Lang::Matching match("[D|d]ependencies");
        for(size_t i=1;i<=lines.size();++i)
        {
            strings info(2,as_capacity); tokenizer<char>::split_with(info, lines[i], ':' );
            if(info.size()!=2)          continue; //! not the good format
            if( !match.partly(info[1])) continue; //! not a dependency line
            ExtractRequired(required,info[2]);
        }
    }

    static inline void RemoveFrom(collection &target, const collection &source)
    {
        for( collection::const_iterator i=source.begin(); i != source.end(); ++i)
        {
            target.no(*i);
        }

    }



    // installed are already know installed ports
    // required  are already known

    static inline void CheckRequired(const string &portName,
                                     collection   &installed,
                                     collection   &missing,
                                     const int     level = 0 )
    {
        indent(std::cerr,level) << "<"  << portName << ">" << std::endl;

        collection    required;
        QueryRequired(required,portName);

        RemoveFrom(required,installed);
        RemoveFrom(required,missing);

        if( required.size()>0)
        {
            strings lines;
            // query installed among required
            {
                string cmd = "port installed ";
                for(collection::iterator i=required.begin(); i != required.end(); ++i)
                {
                    cmd << ' ' << *i;
                }
                load(lines,cmd);
            }

            for(size_t i=2;i<=lines.size();++i)
            {
                tokenizer<char> tkn( lines[i] );
                if( !tkn.next_with(' ') ) continue;
                const string id = tkn.to_string();
                if( required.search(id) )
                {
                    // this is an installed port
                    installed.insert(id);
                }
            }
            RemoveFrom(required,installed);
            if(required.size()>0)
            {
                const int nextLevel = level+1;
                for(collection::iterator i=required.begin(); i != required.end(); ++i)
                {
                    missing.insert(*i);
                    CheckRequired(*i,installed,missing,nextLevel);
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

    string portName = argv[1];
    for(int i=2;i<argc;++i)
    {
        portName << ' ' << argv[i];
    }

    collection installed;
    collection missing;
    CheckRequired(portName,installed,missing);
    if(missing.size())
    {
        std::cerr << missing << std::endl;
    }

}
Y_PROGRAM_END()

