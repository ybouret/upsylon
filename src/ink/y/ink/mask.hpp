//! \file
#ifndef Y_INK_MASK_INCLUDED
#define Y_INK_MASK_INCLUDED 1

#include "y/ink/vertex.hpp"
#include "y/hashing/fnv.hpp"
#include "y/associative/set.hpp"

namespace upsylon
{
    namespace Ink
    {
        //! pointer to vertex
        class MetaVertex
        {
        public:
            Vertex *vtx; //!< pointer to handled vertex

            //! setup
            inline  MetaVertex( Vertex *v) throw() : vtx(v) { assert(vtx); }
            //! cleanup
            inline ~MetaVertex() throw() {}
            //! key is coordinate
            const coord & key() const throw() { assert(vtx); return vtx->position; }
            //! copy
            inline MetaVertex(const MetaVertex &other) throw() : vtx(other.vtx) { assert(vtx); }

            //! dedicated key hasher
            template <typename HashFunction>
            class Hasher
            {
            public:
                inline  Hasher() throw() : hfn() {} //!< setup
                inline ~Hasher() throw() {}         //!< destructor
                HashFunction hfn;                   //!< hashing function

                //! hash integral coordinate
                inline size_t operator()( const coord &q ) throw()
                {
                    size_t h = 0;
                    hfn.set();
                    hfn.run(&q,sizeof(q));
                    hfn.get(&h,sizeof(h));
                    return h;
                }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Hasher);
            };

        private:
            Y_DISABLE_ASSIGN(MetaVertex);
        };

        //! a Mask is a set of unique vertices
        class Mask : public container
        {
        public:
            typedef MetaVertex::Hasher<hashing::fnv> MetaHasher; //!< hasher for MetaVertex
            typedef set<coord,MetaVertex,MetaHasher> MetaDict;   //!< dedicated set
            
            explicit Mask();                  //!< setup
            virtual ~Mask() throw();          //!< desctructor
            Mask( const Mask &other);         //!< hard copy
            Mask & operator=( const Mask & ); //!< assign

            virtual size_t size() const throw();      //!< dict.size(), list.size
            virtual size_t capacity() const throw();  //!< dict.capacity(), list.size+pool.size
            virtual void   free() throw();            //!< dict.free(), pool.store_all(list)
            virtual void   release() throw();         //!< container: release all memory
            virtual void   reserve( const size_t n);  //!< container:reserve

            const Vertex  *head() const throw(); //!< list.head
            bool append( const coord );          //!< append a coordinate, false if already registerded
            bool remove( const coord ) throw();  //!< try remove

            void  match_capacities() const; //!< adjust pool size
            bool  try_match_capacities() const throw(); //!< try to adjust pool size

            //! append wrapper
            inline bool append( const unit_t x, const unit_t y) { return append( coord(x,y) ); }

            //! test if has a coordinate
            inline bool has( const coord q ) const throw() { return 0 != dict.search(q); }

            Mask & __or(  const Mask &other ); //!< logical OR
            Mask & __and( const Mask &other ); //!< logical AND
            Mask & __xor( const Mask &other ); //!< logical XOR
            
        private:
            Vertex::List         list;
            MetaDict             dict;
            mutable Vertex::Pool pool;
        };
    }
}

#endif

