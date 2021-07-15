
#include "y/concurrent/loop/env.hpp"
#include "y/concurrent/loop/solo.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/string/env.hpp"
#include "y/string/convert.hpp"

namespace upsylon
{

    namespace concurrent
    {

        const char looper_from_env::var[] = "Y_LOOPER";

        looper *looper_from_env:: get ()
        {
            static const char *seq[] = {
                "mono",
                "seq",
                "solo",
                "0"
            };

            string content;
            if(environment::get(content,var))
            {
                content.clean_with(" \t");
                if(content.size()>0)
                {
                    // check named value
                    string_convert::make_lower(content);
                    for(size_t i=0;i<sizeof(seq)/sizeof(seq[0]);++i)
                    {
                        if(seq[i] == content) return new solo();
                    }

                    // check #procs
                    const char head = content[0];
                    if(head>='0'&&head<='9')
                    {
                        const size_t nproc = string_convert::to<size_t>(content,var);
                        return new simt(0,nproc,1);
                    }
                }
            }

            return new simt(); //! using Y_THREADING

        }
    }

}
