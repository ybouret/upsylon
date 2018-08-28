//! \file
#ifndef Y_INK_ENGINE_INCLUDED
#define Y_INK_ENGINE_INCLUDED 1

#include "y/concurrent/scheme/simd.hpp"
#include "y/ink/tiles.hpp"
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


            //! FUNC(zone,sync), generic parallel processing
            template <typename FUNC>
            void run( FUNC &func )
            {
                parameters<FUNC> params = { this, &func };
                agent->run( call<FUNC>, &params);
            }

        private:
            dispatcher  agent;
            template <typename FUNC>
            struct parameters
            {
                engine *self_p;
                FUNC   *func_p;
            };

            template <typename FUNC>
            static inline void call( void *addr, parallel &ctx, lockable &sync )
            {
                assert(addr);
                parameters<FUNC> *p = static_cast<parameters<FUNC>*>(addr);
                assert(p->self_p);
                assert(p->func_p);
                assert(ctx.size==p->self_p->zones.size());

                engine     &self = *(p->self_p);
                FUNC       &func = *(p->func_p);
                const tile &zone = self.zones[ctx.indx];
                func(zone,sync);
            }

        public:
            const tiles zones; //!< computed tiles
        };

    }
}


#endif
