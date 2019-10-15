
#include "y/ios/tools/posix-command.hpp"
#include "y/string/temporary-name.hpp"
#include "y/fs/local/fs.hpp"
#include "y/type/aliasing.hpp"
#include "y/exception.hpp"
#include "y/ios/icstream.hpp"

#include <cstdlib>

namespace upsylon {

    namespace ios {

        void posix_command:: query(sequence<string> &lines,
                                   const string     &cmd)
        {
            // setup
            static vfs &fs = local_fs::instance();
            lines.free();

            // get a temporary file name
            const string fn;
            {
                bool lnk = false;
                do {
                    aliasing::_(fn) = temporary_name::create();
                } while(vfs::entry::no_ent != fs.query_attribute(fn,lnk));
            }

            try
            {
                // execute
                const string xcmd = cmd + " &> " + fn;
                if( 0 != system( *xcmd ) )
                {
                    throw exception("couldn't execute [%s]", *xcmd);
                }

                {
                    ios::icstream fp( fn );
                    string line;
                    while( fp.gets(line) )
                    {
                        lines.push_back(line);
                    }
                }

                // success cleanup
                fs.try_remove_file(fn);



            }
            catch(...)
            {
                // failure cleanup
                fs.try_remove_file(fn);
                throw;
            }



        }
    }
}
