
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

            //__________________________________________________________________
            //
            //
            // types and definitions
            // Incoming: Species/Lineage to Equilibrium/Primary
            // Outgoing: from Equilibrium/Primary to Species/Lineag
            // Foward: from reactants to product
            // Reverse: from products to reactant
            //
            //__________________________________________________________________
#define Y_CHEM_STREAM_DECL(NAME) \
class NAME;\
typedef ref_dnode<const NAME> NAME##Link;\
typedef ref_dlist<const NAME> NAME##Links;

            Y_CHEM_STREAM_DECL(ForwardIncoming);
            Y_CHEM_STREAM_DECL(ForwardOutgoing);
            Y_CHEM_STREAM_DECL(ReverseIncoming);
            Y_CHEM_STREAM_DECL(ReverseOutgoing);



            //__________________________________________________________________
            //
            //
            //! a vertex for the graph
            //
            //__________________________________________________________________
            template <typename CLASS>
            class Vertex  : public Object, public authority<const CLASS>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef core::list_of_cpp<Vertex> List;

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup
                inline explicit Vertex(const CLASS &args) throw() :
                Object(), authority<const CLASS>(args), next(0), prev(0)
                {
                }

                //! cleanup
                inline virtual ~Vertex() throw() {}

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! recall name
                inline const string & name() const throw()
                {
                    return (***this).name;
                }

                Vertex *next; //!< for list
                Vertex *prev; //!< for list

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Vertex);
            };

            //__________________________________________________________________
            //
            //
            // aliases for vertices
            //
            //__________________________________________________________________
            typedef Vertex<Lineage>     LineageVertex;   //!< alias
            typedef Vertex<Primary>     PrimaryVertex;   //!< alias
            typedef LineageVertex::List LineageVertices; //!< alias
            typedef PrimaryVertex::List PrimaryVertices; //!< alias


            //__________________________________________________________________
            //
            //
            //! base class for an edge
            //
            //__________________________________________________________________
            class Edge : public Object
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! course
                enum Course
                {
                    Forward, //!< reac->prod
                    Reverse  //!< prod->reac
                };
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~Edge() throw(); //!< cleanup

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Course course; //!< current course
                const unit_t weight; //!< posivite coefficient

            protected:
                //! setup
                explicit Edge(const Course, const unit_t) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Edge);
            };


            //__________________________________________________________________
            //
            //
            //! templated edge
            //
            //__________________________________________________________________
            template <
            Edge::Course COURSE,
            typename     SOURCE,
            typename     TARGET
            >
            class Arrow : public Edge
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


#define Y_CHEMICAL_ARROW(NAME,COURSE,SOURCE,TARGET)                                     \
class NAME : public Arrow<COURSE,SOURCE,TARGET>, public dnode<NAME>                     \
/**/  {                                                                                 \
/**/   public:                                                                          \
/**/    typedef core::list_of_cpp<NAME> List;                                           \
/**/    inline explicit NAME(const SOURCE &s, const unit_t w, const TARGET &t) throw(): \
/**/    Arrow<COURSE,SOURCE,TARGET>(s,w,t) {}                                           \
/**/    inline virtual ~NAME() throw() {}                                               \
/**/   private:                                                                         \
/**/    Y_DISABLE_COPY_AND_ASSIGN(NAME);                                                \
/**/  };\
/**/  typedef NAME::List NAME##Edges


            Y_CHEMICAL_ARROW(ForwardIncoming,Edge::Forward,LineageVertex,PrimaryVertex);
            Y_CHEMICAL_ARROW(ForwardOutgoing,Edge::Forward,PrimaryVertex,LineageVertex);
            Y_CHEMICAL_ARROW(ReverseIncoming,Edge::Reverse,LineageVertex,PrimaryVertex);
            Y_CHEMICAL_ARROW(ReverseOutgoing,Edge::Reverse,PrimaryVertex,LineageVertex);



            class Graph : public Object
            {
            public:

                explicit Graph(const Lineage::Array &lineage,
                               const Primary::Array &primary);

                virtual ~Graph() throw();

                void graphViz(const string &fileName) const;


                const LineageVertices      lineageVertices;
                const PrimaryVertices      primaryVertices;
                const ForwardIncomingEdges forwardIncomingEdges;
                const ForwardOutgoingEdges forwardOutgoingEdges;
                const ReverseIncomingEdges reverseIncomingEdges;
                const ReverseOutgoingEdges reverseOutgoingEdges;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Graph);
            };


        }

    }

}

#endif
