
#include "y/concurrent/loop/runic.hpp"
#include "y/momentary/value.hpp"

namespace upsylon
{
    
    namespace concurrent
    {
        
        runic:: ~runic() throw()
        {
        }
        
        
#define Y_RUNIC_CTOR()             \
ref_ptr<looper>(l),                \
memory::shacks( (**this).size(), construct_filled ), \
runnable(),                        \
kProc(0),                          \
kArgs(0)
        
        runic:: runic(const arc_ptr<looper> &l) : Y_RUNIC_CTOR() {}
        
        runic:: runic(looper *l) : Y_RUNIC_CTOR() {}
        
        
        void runic:: run(const context &ctx, lockable &sync) throw()
        {
            assert(0!=kProc);
            assert(size()==ctx.size);
            memory::shack &cache = *(*this)[ctx.rank];
            kProc(ctx,cache,kArgs,sync);
        }
        
        
        void runic:: operator()(proc user_proc,void *user_args) throw()
        {
            assert(0==kProc);
            assert(0==kArgs);
            
            const momentary_value<proc>   temp_proc(kProc,user_proc);
            const momentary_value<void *> temp_args(kArgs,user_args);
            (**this).for_each(*this);
        }

        
        
    }
    
}

