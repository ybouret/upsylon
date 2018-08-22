#ifndef Y_INK_ENGINE_INCLUDED
#define Y_INK_ENGINE_INCLUDED 1

#include "y/concurrent/scheme/for-each.hpp"
#include "y/ink/area.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace ink
    {
        typedef arc_ptr<concurrent::for_each> workers;


        //! parallel engine for a given area
        class engine : public area
        {
        public:
            explicit engine(const workers &shared,
                            const area    &full);
            virtual ~engine() throw();

            //! FUNC(args,zone,sync)
            template <typename FUNC, typename T>
            void run( FUNC &func, T &args )
            {
                parameters<FUNC,T> params = { this, &func, &args };
                agents->run( call<FUNC,T>, &params);
            }

        private:
            workers    agents;

        public:
            const areas tiles;
            template <typename FUNC,typename T>
            struct parameters
            {
                engine *self_p;
                FUNC   *func_p;
                T      *args_p;
            };

            template <typename FUNC,typename T>
            static inline void call( void *addr, parallel &ctx, lockable &sync )
            {
                assert(addr);
                parameters<FUNC,T> *p = static_cast< parameters<FUNC,T> *>(addr);
                assert(p->self_p);
                assert(p->func_p);
                assert(p->args_p);
                assert(ctx.size==p->self_p->tiles.size());

                engine     &self = *(p->self_p);
                FUNC       &func = *(p->func_p);
                T          &args = *(p->args_p);
                const area &zone = self.tiles[ctx.indx];
                func(args,zone,sync);
            }
        };

    }
}


#endif
