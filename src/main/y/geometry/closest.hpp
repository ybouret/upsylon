//! \file

#ifndef Y_GEOM_CLOSEST_INCLUDED
#define Y_GEOM_CLOSEST_INCLUDED 1

#include "y/core/list.hpp"
#include "y/core/node.hpp"
#include "y/concurrent/scheme/for-each.hpp"
#include <cstring>

namespace upsylon {

    namespace geometry {

        //! NODE is like node_of<POINT>, may be different
        template <typename NODE>
        struct closest
        {
            typedef typename core::list_of<NODE>      list_type;  //!< alias
            typedef typename NODE::type               point_type; //!< alias
            typedef typename point_type::mutable_type type;       //!< alias
            typedef typename point_type::const_type   const_type; //!< alias

            //! store local info
            struct info
            {
                const NODE *l; //!< from left list
                const NODE *r; //!< from right list
                type        d; //!< distance (squared)

                //! update if smaller d factor
                template <typename PROC>
                inline void update( const NODE *L, const NODE *R, PROC &D ) throw()
                {
                    const_type tmp = D( **L, **R );
                    if(tmp<d)
                    {
                        d = tmp;
                        l = L;
                        r = R;
                    }
                }
            };

            //! full parallel run
            template <typename PROC>
            static inline void find(info                 &opt,
                                    const list_type      &lhs,
                                    const list_type      &rhs,
                                    concurrent::for_each &loop,
                                    PROC                 &proc)
            {
                memset(&opt,0,sizeof(info));
                if(lhs.size>0&&rhs.size>0)
                {
                    loop.engine().ready<info>(memory::storage::shared);

                    struct Task
                    {
                        const list_type *L;
                        const list_type *R;
                        PROC            *H;

                        static inline
                        void Run( void *args, parallel &ctx, lockable & ) throw()
                        {
                            Task            &task = *static_cast<Task *>(args);
                            const list_type &L    = *task.L;
                            const list_type &R    = *task.R;
                            PROC            &H    = *task.H;
                            info            &I    = (*ctx).get<info>();

                            I.l = NULL;
                            I.r = NULL;
                            I.d = 0;

                            assert(L.size>=R.size);
                            size_t offset = 0;
                            size_t length = L.size;
                            ctx.split(length, offset);

                            if(length>0)
                            {
                                I.l = L.fetch(offset);
                                I.r = R.head;
                                I.d = H( **I.l, **I.r);
                                for( const NODE *l = L.fetch(offset); length>0; --length, l=l->next )
                                {
                                    for(const NODE *r = R.head; r; r=r->next )
                                    {
                                        I.update(l,r,H);
                                    }
                                }
                            }

                        }

                    };

                    Task       task = { &lhs, &rhs, &proc };
                    const bool swp  = (lhs.size<rhs.size);
                    if(swp)    cswap( task.L, task.R );

                    loop.run( Task::Run, &task);
                    const size_t          n = loop.number();
                    concurrent::executor &E = loop.engine();
                    size_t       i = 0;
                    for(i=0;i<n;++i)
                    {
                        info &I = (*E[i]).get<info>();
                        if(I.l)
                        {
                            assert(I.r!=0);
                            std::cerr << "\t@context#" << i << " : " << I.d << std::endl;
                            memcpy(&opt,&I,sizeof(info));
                            break;
                        }
                    }

                    for(++i;i<n;++i)
                    {
                        info &I = (*E[i]).get<info>();
                        if(I.l)
                        {
                            assert(I.r!=0);
                            std::cerr << "@context#" << i << " : " << I.d << std::endl;

                            if(I.d<opt.d)
                            {
                                memcpy(&opt,&I,sizeof(info));
                            }
                        }
                    }

                    std::cerr << "winner: " << opt.d << std::endl;
                    if(swp) cswap(opt.l,opt.r);

                }
                
            }
        };
    }
    
}


#endif

