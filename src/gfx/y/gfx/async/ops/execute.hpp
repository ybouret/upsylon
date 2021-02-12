
//! \file

#ifndef Y_GFX_ASYNC_EXECUTE_INCLUDED
#define Y_GFX_ASYNC_EXECUTE_INCLUDED 1

#include "y/gfx/async/broker.hpp"


namespace upsylon
{
    namespace GFX
    {
        template <typename T> class Pixmap;

        namespace Async
        {

            struct Execute
            {
            public:
                //! pixel = func(pixel)
                template <typename TARGET, typename SOURCE, typename FUNC>  static inline
                void Function(TARGET &target, const SOURCE &source, FUNC &func, Broker &broker)
                {
                    UnaryFunctionContext<TARGET,SOURCE,FUNC> op = { target, source, func };
                    broker(op.run,&op);
                }

                

                //! pixel = proc(pixel,x,y)
                template <typename TARGET, typename SOURCE, typename PROC>  static inline
                void Procedure(TARGET &target, const SOURCE &source, PROC &proc, Broker &broker)
                {
                    UnaryProcedureContext<TARGET,SOURCE,PROC> op = { target, source, proc };
                    broker(op.run,&op);
                }

            private:
                template <typename TARGET, typename SOURCE, typename FUNC>
                struct UnaryFunctionContext
                {
                    TARGET       &target;
                    const SOURCE &source;
                    FUNC         &func;

                    static inline void run( Worker &w, lockable &, void *args )
                    {
                        assert(args);
                        UnaryFunctionContext &self  = *static_cast<UnaryFunctionContext *>(args);
                        TARGET               &target = self.target;
                        const SOURCE         &source = self.source;
                        const Tile           &tile   = w.tile;
                        FUNC                 &func   = self.func;
                        for(size_t t=tile.size();t>0;--t)
                        {
                            const HScan                &s   = tile[t];
                            Point                       p   = s.begin;
                            const typename SOURCE::Row &src = source[p.y];
                            typename TARGET::Row       &tgt = target[p.y];
                            for(unit_t i=s.width;i>0;--i,++p.x)
                            {
                                tgt[p.x] = func(src[p.x]);
                            }
                        }
                    }
                };

                template <typename TARGET, typename SOURCE, typename PROC>
                struct UnaryProcedureContext
                {
                    TARGET       &target;
                    const SOURCE &source;
                    PROC         &proc;

                    static inline void run( Worker &w, lockable &, void *args )
                    {
                        assert(args);
                        UnaryProcedureContext &self   = *static_cast<UnaryProcedureContext *>(args);
                        TARGET                &target = self.target;
                        const SOURCE          &source = self.source;
                        const Tile            &tile   = w.tile;
                        PROC                  &proc   = self.proc;
                        for(size_t t=tile.size();t>0;--t)
                        {
                            const HScan                &s   = tile[t];
                            Point                       p   = s.begin;
                            typename TARGET::Row       &tgt = target[p.y];
                            for(unit_t i=s.width;i>0;--i,++p.x)
                            {
                                tgt[p.x] = proc(source,p.x,p.y);
                            }
                        }
                    }
                };


            };
        }

    }

}

#endif
