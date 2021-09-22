
//! \file

#ifndef Y_CHEMICAL_STREAM_INCLUDED
#define Y_CHEMICAL_STREAM_INCLUDED 1

#include "y/chemical/sys/lineage.hpp"
#include "y/core/rlinked.hpp"

namespace upsylon
{
    namespace Chemical
    {

        namespace Stream
        {
            
            template <typename CLASS>
            class Vertex  : public Object, public authority<const CLASS>
            {
            public:
                typedef core::list_of_cpp<Vertex> List;

                Vertex *prev;
                Vertex *next;

                inline explicit Vertex(const CLASS &args) throw() :
                Object(), authority<const CLASS>(args), next(0), prev(0)
                {
                }

                inline virtual ~Vertex() throw() {}

                inline const string & name() const throw()
                {
                    return (***this).name;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Vertex);
            };

            typedef Vertex<Lineage>     LineageVertex;
            typedef Vertex<Primary>     PrimaryVertex;
            typedef LineageVertex::List LineageVertices;
            typedef PrimaryVertex::List PrimaryVertices;


            class Edge : public Object
            {
            public:
                enum Course
                {
                    Forward,
                    Reverse
                };

                virtual ~Edge() throw();

                const Course course;
                const unit_t weight;

            protected:
                explicit Edge(const Course, const unit_t) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Edge);
            };

            template <
            Edge::Course COURSE,
            typename     SOURCE,
            typename     TARGET
            >
            class Arrow : public Edge, public dnode< Arrow<COURSE,SOURCE,TARGET> >
            {
            public:
                inline virtual ~Arrow() throw() {}

                inline explicit Arrow(const SOURCE &s,
                                      const unit_t  w,
                                      const TARGET &t) throw() :
                Edge(COURSE,w),
                source(s),
                target(t)
                {
                }

                const SOURCE &source; //!< a vertex
                const TARGET &target; //!< a vertex

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arrow);
            };


            class Graph : public Object
            {
            public:

                explicit Graph(const Lineage::Array &lineage,
                               const Primary::Array &primary);

                virtual ~Graph() throw();


                const LineageVertices lineageVertices;
                const PrimaryVertices primaryVertices;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Graph);
            };


        }

    }

}

#endif
