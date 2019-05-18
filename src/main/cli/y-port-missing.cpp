#include "y/program.hpp"
#include "y/string/temporary-name.hpp"
#include "y/fs/local.hpp"
#include "y/ios/icstream.hpp"
#include "y/sequence/vector.hpp"
#include "y/lang/pattern/matching.hpp"
#include "y/memory/pooled.hpp"
#include "y/string/tokenizer.hpp"
#include "y/sort/unique.hpp"

#if defined(Y_BSD)
#include <unistd.h>
#endif
using namespace upsylon;


static bool isSepV1( const char C ) { return C == ' ' || C == ','; }
static bool isSepV2( const char C ) { return C == ' '; }

Y_PROGRAM_START()
{
#if defined(Y_BSD)
    // create cmd
    if(argc<=1) throw exception("usage : %s port_name [variants]",program);
    const string                  tmp = temporary_name(8);
    vector<string,memory::pooled> deps;
    vfs         &fs  = local_fs::instance();

    // extract all deps
    try
    {
        {

            // forge command
            string       cmd = "port deps";
            for(int iarg=1;iarg<argc;++iarg)
            {
                cmd << ' ' << argv[iarg];
            }

            // forge temporary name
            fs.try_remove_file(tmp);
            cmd << " &> " << tmp;

            // execute cmd
            {
                ios::ocstream::overwrite(tmp);
                const int ret = system(*cmd);
                if(0!=ret)
                {
                    throw exception("invalid return code from '%s'", *cmd);
                }
            }

            // parse result
            {
                Lang::Matching isDep = "[Dd]ependencies:";
                string line;
                ios::icstream fp(tmp);
                while( fp.gets(line) )
                {
                    if( isDep.partly(line) )
                    {
                        const char *org = *line;
                        const char *sep = strchr(org, ':');
                        if(!sep)
                        {
                            throw exception("unexpected failure in looking for ':'");
                        }
                        line.skip(sep-org+2);
                        tokenizer<char> tkn(line);
                        while( tkn.next(isSepV1) )
                        {
                            const string dep = tkn.to_string();
                            deps.push_back(dep);
                        }
                    }
                }
            }
            fs.try_remove_file(tmp);
        }
        unique(deps);

        if(deps.size())
        {
            vector<string,memory::pooled> installed(deps.size(),as_capacity);

            // query
            {
                string cmd = "port installed";
                for(size_t i=1;i<=deps.size();++i)
                {
                    cmd << ' ' << deps[i];
                }
                cmd << " &> " << tmp;
                ios::ocstream::overwrite(tmp);
                {
                    const int ret = system( *cmd );
                    if( ret != 0 ) throw exception("couldn't query dependencies!!!");
                }
            }

            // parse installed
            {
                ios::icstream fp(tmp);
                string line;
                if(!fp.gets(line)) throw exception("no header line!!!");

                while( fp.gets(line) )
                {
                    tokenizer<char> tkn(line);
                    if( !tkn.next(isSepV2) )
                    {
                        throw exception("missing port name!");
                    }
                    const string id = tkn.to_string();
                    installed.push_back(id);
                }

            }
            fs.try_remove_file(tmp);
            unique(installed);

            // cross databases
            size_t count = 0;
            for(size_t i=1;i<=deps.size();++i)
            {
                const string &dep   = deps[i];
                bool          found = false;

                for(size_t j=1;j<=installed.size();++j)
                {
                    if(dep==installed[j])
                    {
                        found = true;
                        break;
                    }
                }
                if(!found)
                {
                    if(count++>0) std::cout << ' ';
                    std::cout << dep;
                }
            }
            if(count>0) std::cout << std::endl;
        }
    }
    catch(...)
    {
        fs.try_remove_file(tmp);
        throw;
    }
#else
    throw exception("%s not valid on %s", program, platform() );
#endif


}
Y_PROGRAM_END()
