//! \file
#ifndef Y_INK_ENGINE_INCLUDED
#define Y_INK_ENGINE_INCLUDED 1

#include "y/concurrent/scheme/for-each.hpp"
#include "y/ink/area.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace ink
    {
        //! shared for_each object
        typedef arc_ptr<concurrent::for_each> dispatcher;


        //! parallel engine for a given area
        /**
         build an engine for a given area, with a reusable dispatcher
         */
        class engine : public area
        {
        public:
            //! constructor, build tiles with shared from full
            explicit engine(const dispatcher &shared,
                            const area       &full);
            //! desctructor
            virtual ~engine() throw();


            //! FUNC(args,zone,sync), generic parallel processing
            template <typename FUNC, typename T>
            void run( FUNC &func, T &args )
            {
                parameters<FUNC,T> params = { this, &func, &args };
                agent->run( call<FUNC,T>, &params);
            }

        private:
            dispatcher  agent;
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

        public:
            const areas tiles; //!< computed tiles
        };

    }
}


#endif
