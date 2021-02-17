
#include "y/concurrent/loop/simt.hpp"

namespace upsylon
{

    namespace concurrent
    {

        simt:: simt() :
        topo( new topology() ),
        crew( topo->size()   ),
        verbose( nucleus::thread::verbosity(Y_VERBOSE_SIMT) )
        {
        }

        simt:: ~simt() throw()
        {
        }

        void simt:: setup()
        {
            try
            {
                const size_t count = topo->size();
                for(size_t rank=0;rank<count;++rank)
                {
                    crew.build<nucleus::thread::call,void*,size_t,size_t>(stub,this,count,rank);
                }
            }
            catch(...)
            {
                throw;
            }
        }

        void simt:: stub(void *args) throw()
        {
            assert(args);
            simt  *self = static_cast<simt *>(args);

        }



        
    }

}

