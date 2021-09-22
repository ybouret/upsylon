
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

            //! references nodes and list
#define Y_CHEM_STREAM_DECL(NAME) \
class NAME;\
typedef ref_dnode<const NAME> NAME##Link;\
typedef ref_dlist<const NAME> NAME##Links;

            Y_CHEM_STREAM_DECL(ForwardIncoming); //!< Forward Incoming Link(s)
            Y_CHEM_STREAM_DECL(ForwardOutgoing); //!< Forward Outgoing Link(s)
            Y_CHEM_STREAM_DECL(ReverseIncoming); //!< Reverse Incoming Link(s)
            Y_CHEM_STREAM_DECL(ReverseOutgoing); //!< Reverse Outgoing Link(s)



            //__________________________________________________________________
            //
            //
            //! a vertex for the graph
            //
            //__________________________________________________________________
            template <
            typename CLASS,
            typename FORWARD,
            typename REVERSE>
            class Vertex  : public Object, public authority<const CLASS>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef core::list_of_cpp<Vertex> List; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup
                inline explicit Vertex(const CLASS &args) throw() :
                Object(), authority<const CLASS>(args), next(0), prev(0),
                forward(), reverse()
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

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                Vertex       *next;    //!< for list
                Vertex       *prev;    //!< for list
                const FORWARD forward; //!< forward links
                const REVERSE reverse; //!< reverse links

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Vertex);
            };

            //__________________________________________________________________
            //
            //
            // aliases for vertices
            //
            //__________________________________________________________________
            typedef Vertex<Lineage,ForwardIncomingLinks,ReverseIncomingLinks>  LineageVertex;   //!< alias
            typedef Vertex<Primary,ForwardOutgoingLinks,ReverseOutgoingLinks>  PrimaryVertex;   //!< alias
            typedef LineageVertex::List LineageVertices; //!< alias
            typedef PrimaryVertex::List PrimaryVertices; //!< alias

            //__________________________________________________________________
            //
            //! Course for an Oirented object
            //__________________________________________________________________
            enum Course
            {
                Forward, //!< reac->eq or eq->prod
                Reverse  //!< prod->eq or eq->reac
            };

            //__________________________________________________________________
            //
            //! human readable course
            //__________________________________________________________________
            const char *CourseText(const Course) throw();

            //__________________________________________________________________
            //
            //
            //! base class for oriented objects
            //
            //__________________________________________________________________
            class Oriented : public Object
            {
            public:
                virtual      ~Oriented() throw();          //!< cleanup
                const char   *courseText() const throw();  //!< info
                const Course  course;                      //!< the value

            protected:
                explicit Oriented(const Course)     throw(); //!< setup
                explicit Oriented(const Oriented &) throw(); //!< copy

            private:
                Y_DISABLE_ASSIGN(Oriented);
            };


            //__________________________________________________________________
            //
            //
            //! base class for an edge
            //
            //__________________________________________________________________
            class Edge : public Oriented
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~Edge() throw(); //!< cleanup

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
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
            Course       COURSE,
            typename     SOURCE,
            typename     TARGET
            >
            class Arrow : public Edge
            {
            public:
                //______________________________________________________________
                //
                // definitions
                //______________________________________________________________
                static const Course CourseType = COURSE; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                //! cleanup
                inline virtual ~Arrow() throw() {}

                //! setup
                inline explicit Arrow(const SOURCE &s,
                                      const unit_t  w,
                                      const TARGET &t) throw() :
                Edge(COURSE,w),
                source(s),
                target(t)
                {
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const SOURCE &source; //!< a vertex
                const TARGET &target; //!< a vertex

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arrow);
            };

            //__________________________________________________________________
            //
            //
            //! implement the specialized arrows
            //
            //__________________________________________________________________
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


            Y_CHEMICAL_ARROW(ForwardIncoming,Forward,LineageVertex,PrimaryVertex); //!< Forward Incoming Edge(s)
            Y_CHEMICAL_ARROW(ForwardOutgoing,Forward,PrimaryVertex,LineageVertex); //!< Forward Outgoing Edge(s)
            Y_CHEMICAL_ARROW(ReverseIncoming,Reverse,LineageVertex,PrimaryVertex); //!< Reverse Incoming Edge(s)
            Y_CHEMICAL_ARROW(ReverseOutgoing,Reverse,PrimaryVertex,LineageVertex); //!< Reverse Outgoing Edge(s)


            //__________________________________________________________________
            //
            //
            // aliases for references to species/lineage
            //
            //__________________________________________________________________
            typedef ref_dnode<const Lineage> Member;  //!< alias
            typedef ref_dlist<const Lineage> Members; //!< alias


            //__________________________________________________________________
            //
            //
            //! Path base class : list of visited species
            //
            //__________________________________________________________________
            class Path : public Oriented
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~Path() throw();

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                void           reshape();                                     //!< re-order visited
                std::ostream & indent(std::ostream&)                   const; //!< helper to indent
                bool           owns(const Lineage &)           const throw(); //!< test ownership
                static bool    areAnalog(const Path &, const Path &) throw(); //!< both paths must be reshaped

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Members visited; //!< keep track of visited
                const bool    isValid; //!< initially false
                
            protected:
                explicit Path(const Course) throw(); //!< setup
                explicit Path(const Path &);         //!< full copy
                void     visit(const Lineage &l);    //!< append reference
                void     validate() throw();         //!< isValid = true
                //!
            private:
                Y_DISABLE_ASSIGN(Path);
            };

            //__________________________________________________________________
            //
            //
            //! Parametric Route
            //
            //__________________________________________________________________
            template <
            Course   COURSE,
            typename INCOMING,
            typename OUTGOING
            >
            class Route : public Path, public dnode< Route<COURSE,INCOMING,OUTGOING> >
            {
            public:
                //______________________________________________________________
                //
                // types and definition
                //______________________________________________________________
                static const Course CourseType = COURSE; //!< alias
                typedef Route<COURSE,INCOMING,OUTGOING>   RouteType;    //!< alias
                typedef dnode<RouteType>                  Node;         //!< alias
                typedef core::list_of<Node>               List;         //!< alias
                typedef typename INCOMING::node_type      IncomingLink; //!< retrieve link
                typedef typename OUTGOING::node_type      OutgoingLink; //!< retrieve link
                typedef typename IncomingLink::const_type IncomingEdge; //!< retrieve const Edge
                typedef typename OutgoingLink::const_type OutgoingEdge; //!< retrieve const Edge

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                inline virtual ~Route() throw() {}


                //! initialize and probe
                inline explicit Route(IncomingEdge &entry,
                                      List         &extra) :
                Path(COURSE),
                Node(),
                incoming(),
                outgoing()
                {
                    //__________________________________________________________
                    //
                    // initialize first edge and first visited
                    //__________________________________________________________
                    aliasing::_(incoming).append(entry);
                    visit(*entry.source);
                    if(Verbosity) {
                        indent(std::cerr) << ***(visited.tail) << " starts " << courseText() << " to " << entry.target.name() << std::endl;
                    }
                }


                //! full copy
                inline explicit Route(const Route &route) :
                Path(route),
                Node(),
                incoming(route.incoming),
                outgoing(route.outgoing)
                {
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const INCOMING incoming; //!< links to incoming edges
                const OUTGOING outgoing; //!< links to outgoing edges

            private:
                Y_DISABLE_ASSIGN(Route);
            };

            typedef Route<Forward,ForwardIncomingLinks,ForwardOutgoingLinks> ForwardRoute;  //!< alias
            typedef ForwardRoute::List                                       ForwardRoutes; //!< alias
            typedef Route<Reverse,ReverseIncomingLinks,ReverseOutgoingLinks> ReverseRoute;  //!< alias
            typedef ReverseRoute::List                                       ReverseRoutes; //!< alias

            //__________________________________________________________________
            //
            //
            //! Full bidirectional Graph
            //
            //__________________________________________________________________
            class Graph : public Object
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! build all
                explicit Graph(const Lineage::Array &lineage,
                               const Primary::Array &primary);

                //! cleanup
                virtual ~Graph() throw();

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                void graphViz(const string &fileName) const; //!< encode/render

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const LineageVertices      lineageVertices;      //!< all species
                const PrimaryVertices      primaryVertices;      //!< all equilibria
                const ForwardIncomingEdges forwardIncomingEdges; //!< forward incoming
                const ForwardOutgoingEdges forwardOutgoingEdges; //!< forward outgoing
                const ReverseIncomingEdges reverseIncomingEdges; //!< reverse incoming
                const ReverseOutgoingEdges reverseOutgoingEdges; //!< reverse outgoing
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Graph);
                void buildRoutes();
            };


        }

    }

}

#endif
