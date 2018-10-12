//! file
#ifndef Y_INK_VERTEX_INCLUDED
#define Y_INK_VERTEX_INCLUDED 1

#include "y/ink/types.hpp"
#include "y/core/pool.hpp"
#include "y/core/list.hpp"

namespace upsylon
{
    namespace Ink
    {
        //! dynamic vertex
        class Vertex : public Object
        {
        public:
            const coord position; //!< x,y
            Vertex *next;   //!< for list/pool
            Vertex *prev;   //!< for list/pool
            //! initialize
            inline Vertex( const coord C ) throw() : Object(), position(C), next(0), prev(0) {}
            //! copy
            inline Vertex(const Vertex &other) throw() : Object(), position(other.position), next(0), prev(0) {}


            typedef core::list_of_cpp<Vertex> List;     //!< list of vertices
            typedef core::pool_of_cpp<Vertex> PoolType; //!< pool of vertices

            //! a pool to reuse vertices
            class Pool : public PoolType
            {
            public:
                inline Pool() throw() : PoolType() {} //!< constructor
                inline virtual ~Pool() throw() {}     //!< destructor

                //! query/create and construct a vertex for pos
                Vertex * operator()(const coord pos)
                {
                    if(size>0)
                    {
                        Vertex *vtx = query();
                        (coord &)(vtx->position) = pos;
                        return vtx;
                    }
                    else
                    {
                        return new Vertex(pos);
                    }
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Pool);
            };

        private:
            Y_DISABLE_ASSIGN(Vertex);
        };

    }

}

#endif

