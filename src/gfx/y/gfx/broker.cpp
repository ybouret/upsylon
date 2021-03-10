
#include "y/gfx/broker.hpp"
#include "y/momentary/value.hpp"

namespace upsylon
{
    namespace graphic
    {

        broker:: ~broker() throw()
        {
            call = 0;
        }

        broker:: broker(const engine &l,
                        const area   &a) :
        func(0),
        args(0),
        loop(l),
        tess(a,*loop),
        call(0)
        {
            setup();
        }

        void broker:: setup() throw()
        {
            if(tess.size()<loop->size())
            {
                std::cerr << "DOWNSIZED" << std::endl;
                call = & broker::run_downsized;
            }
            else
            {
                std::cerr << "OPTIMIZED" << std::endl;
                assert(tess.size()==loop->size());
                call = & broker::run_optimized;
            }
        }

        void broker:: run_downsized(const concurrent::context &ctx, lockable &sync) throw()
        {
            assert(tess.size()<ctx.size);
            assert(func);
            assert(args);
            if(ctx.rank<tess.size())
            {
                func(tess[ctx.rank],args,sync);
            }
        }

        void broker:: run_optimized(const concurrent::context &ctx, lockable &sync) throw()
        {
            assert(ctx.indx<=tess.size());
            assert(ctx.size==tess.size());
            assert(func);
            assert(args);
            func(tess[ctx.rank],args,sync);
        }

        void broker:: run( const concurrent::context &ctx, lockable &sync) throw()
        {
            assert(call);
            ((*this).*(call))(ctx,sync);
        }

        void broker:: operator()(proc user_func, void *user_args) throw()
        {
            assert(!func);
            assert(!args);
            assert(user_func);
            assert(user_args);
            const momentary_value<proc>   keep_func(func,user_func);
            const momentary_value<void *> keep_args(args,user_args);
            assert(func);
            assert(args);
            loop->for_each(*this);
        }

    }

}
