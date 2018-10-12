//! \file
#ifndef Y_INK_MASK_INCLUDED
#define Y_INK_MASK_INCLUDED 1

#include "y/ink/vertex.hpp"
#include "y/hashing/sfh.hpp"

namespace upsylon
{
    namespace Ink
    {
        //! pointer to vertex
        class MetaVertex
        {
        public:
            const Vertex *vtx;
            //! setup
            inline  MetaVertex(const Vertex *v) throw() : vtx(v) { assert(vtx); }
            //! cleanup
            inline ~MetaVertex() throw() {}
            //! key is coordinate
            const coord & key() const throw() { assert(vtx); return vtx->position; }
            //! copy
            inline MetaVertex(const MetaVertex &other) throw() : vtx(other.vtx) { assert(vtx); }

            template <typename HashFunction>
            class Hasher
            {
            public:
                inline  Hasher() throw() : hfn() {}
                inline ~Hasher() throw() {}
                HashFunction hfn;
                inline size_t operator()( const coord &key ) throw()
                {
                    size_t h = 0;
                    hfn.set();
                    hfn.run(&key,sizeof(key));
                    hfn.get(&h,sizeof(h));
                    return h;
                }
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Hasher);
            };

        private:
            Y_DISABLE_ASSIGN(MetaVertex);
        };
    }
}

#endif

