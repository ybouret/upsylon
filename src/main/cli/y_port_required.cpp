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
    typedef sorted_vector<string,depcomp,memory::pooled> dependencies_type;
    typedef ordered_unique<dependencies_type>            dependencies;

    static inline
    void load( strings &lines, const string &cmd )
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

    static inline bool isInstalled(const string &dep)
    {
        Lang::Matching match = dep;
        strings        result;
        {
            const string cmd = "port installed " + dep;
            load(result,cmd);
            for(size_t i=1;i<=result.size();++i)
            {
                if(match.partly(result[i]))
                {
                    std::cerr << "installed " << dep << std::endl;
                    return true;
                }
            }
        }
        std::cerr << "missing   " << dep << std::endl;
        return false;
    }

    static inline bool WS(const int C) throw() { return C==' ' || C=='\t'; }


    static inline
    bool populate( dependencies &deps, const string &info )
    {
        tokenizer<char> tkn(info);
        bool            ans = false;
        while( tkn.next_with(',') )
        {
            string dep( tkn.token(), tkn.units() );
            dep.clean(WS);
            if(isInstalled(dep)) continue;
            if(deps.insert(dep))
            {
                ans = true;
            }
        }

        return ans;
    }

    static inline
    bool queryDeps( dependencies &deps, const strings &lines )
    {
        bool ans = false;
        Lang::Matching match("[D|d]ependencies");
        deps.free();
        for(size_t i=1;i<=lines.size();++i)
        {
            strings info(2,as_capacity);
            tokenizer<char>::split_with(info, lines[i], ':' );
            if(info.size()!=2) continue;
            if( !match.partly(info[1])) continue;
            if(populate(deps,info[2]))
            {
                ans = true;
            }
        }
        std::cerr << "|_" << deps << std::endl;
        return ans;
    }

    
}

Y_PROGRAM_START()
{
    if( argc < 2 )
    {
        return 0;
    }

    string portName = argv[1];
    for(int i=2;i<argc;++i)
    {
        portName << ' ' << argv[i];
    }

    const string cmd = "port deps " + portName;
    strings lines;
    load(lines,cmd);
    dependencies deps;
    queryDeps(deps,lines);

}
Y_PROGRAM_END()

