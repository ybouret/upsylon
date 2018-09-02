//! \file
#ifndef Y_INK_PARTICLES_INCLUDED
#define Y_INK_PARTICLES_INCLUDED 1

#include "y/ink/vertex.hpp"
#include "y/comparison.hpp"
#include "y/sort/merge.hpp"

namespace upsylon
{
    namespace Ink
    {
        template <typename T>
        class Particle : public Object, public Vertex::List
        {
        public:
            Y_DECL_ARGS(T,type); //!< alias

            const_type tag;  //!< identifier
            Particle  *next; //!< for list
            Particle  *prev; //!< for list

            //! constructor with tag
            inline explicit Particle( param_type id ) :
            Object(),
            Vertex::List(),
            tag(id),
            next(0),
            prev(0) {}

            //! desctrutor
            inline virtual ~Particle() throw() {}
            //! copy
            inline explicit Particle( const Particle &other ) :
            Object(),
            Vertex::List( other ),
            tag( other.tag ),
            next(0),
            prev(0)
            {}

            //! comparison for merge sort
            static inline int CompareBySize( const Particle *lhs, const Particle *rhs, void *) throw()
            {
                assert(lhs); assert(rhs);
                return comparison::decreasing(lhs->size,rhs->size);
            }

            typedef core::list_of_cpp<Particle> ListType;

            class List : public ListType
            {
            public:
                inline explicit List() throw() : ListType() {}
                inline virtual ~List() throw() {}
                inline List(const List &other) : ListType(other) {}

                inline void sort()
                {
                    merging<Particle>::sort( *this, Particle::CompareBySize, NULL);
                }

            private:
                Y_DISABLE_ASSIGN(List);
            };

        private:
            Y_DISABLE_ASSIGN(Particle);
        };



    }
}

#endif

