//! \file

#ifndef Y_CHEMICAL_FLUX_INCLUDED
#define Y_CHEMICAL_FLUX_INCLUDED 1

#include "y/chemical/sys/lineage.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/core/rlinked.hpp"

namespace upsylon
{
    namespace Chemical
    {

        namespace Flux
        {
            //__________________________________________________________________
            //
            //
            // aliases and definitions
            //
            //__________________________________________________________________
            typedef ios::vizible Viz;     //!< alias
            class                Vertex;  //!< forward declaration
            
            //__________________________________________________________________
            //
            //! alias for list of vertices
            //__________________________________________________________________
            typedef core::list_of_cpp<Vertex> Vertices;

            //__________________________________________________________________
            //
            //! Course for an Edge
            //__________________________________________________________________
            enum Course
            {
                Forward, //!< reac->eq or eq->prod
                Reverse  //!< prod->eq or eq->reac
            };

            //__________________________________________________________________
            //
            //! human readible course
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
                //!
            protected:
                explicit Oriented(const Course)     throw(); //!< setup
                explicit Oriented(const Oriented &) throw(); //!< copy

            private:
                Y_DISABLE_ASSIGN(Oriented);
            };

            //__________________________________________________________________
            //
            //! Family for an Edge
            //__________________________________________________________________
            enum Family
            {
                LineageToPrimary, //!< species -> equilibrium
                PrimaryToLineage  //!< equilibrium -> species
            };


            //__________________________________________________________________
            //
            //
            //! a variant edge in the graph
            //
            //__________________________________________________________________
            class Edge : public Oriented, public dnode<Edge>
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                virtual ~Edge() throw();

                //! setup
                explicit Edge(const Course  way,
                              const Family  how,
                              const Vertex &ini,
                              const Vertex &end,
                              const unit_t  cof) throw();


                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                void viz(ios::ostream &) const; //!< encode as graphViz

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Family  family; //!< family
                const Vertex &source; //!< source
                const Vertex &target; //!< target
                const unit_t  weight; //!< positive stoichiometric coefficient

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Edge);
            };

            //__________________________________________________________________
            //
            // multiple
            //__________________________________________________________________
            typedef core::list_of_cpp<Edge> Edges;  //!< alias for list of Edges
            typedef ref_dnode<const Edge>   Link;  //!< link to an Edge
            typedef ref_dlist<const Edge>   Links; //!< References
            //!
            //__________________________________________________________________
            //
            //
            //! Dual Edges for one course
            //
            //__________________________________________________________________
            class DualEdges
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                DualEdges() throw();
                ~DualEdges() throw();

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Edges lineageToPrimary; //!< same course, LineagetoPrimary family
                const Edges primaryToLineage; //!< same course, PrimaryToLineage family

            private:
                Y_DISABLE_COPY_AND_ASSIGN(DualEdges);
            };

            //__________________________________________________________________
            //
            //! identifier for Vertex
            //__________________________________________________________________
            enum Genus
            {
                IsLineage, //!< for lineage/species
                IsPrimary, //!< for primary/equilibrium
            };


            //__________________________________________________________________
            //
            //
            //! a variant vertex in the graph
            //
            //__________________________________________________________________
            class Vertex : public Object, public dnode<Vertex>
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit     Vertex(const Lineage &) throw(); //!< define as lineage
                explicit     Vertex(const Primary &) throw(); //!< define as primart
                virtual     ~Vertex() throw();                //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                const char   * name()      const throw(); //!< genus depending name
                ios::ostream & viz(ios::ostream &) const; //!< encode as graphviz

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Genus genus; //!< identifier
                union {
                    const Lineage *const lineage; //!< lightweight lineage
                    const Primary *const primary; //!< lightweight primary
                };

                const Links forward; //!< where I go to
                const Links reverse; //!< where I come from

                //! display information
                template <typename OSTREAM> inline
                OSTREAM & display(OSTREAM &os, const size_t indent) const
                {
                    Library::Indent(os,indent) << name();
                    DisplayLinks(os,reverse,"<");
                    DisplayLinks(os,forward,">");
                    os << '\n';
                    return os;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Vertex);

                template <typename OSTREAM> static inline
                void DisplayLinks(OSTREAM &os, const Links &links, const char *symb)
                {
                    if(links.size)
                    {
                        os << ' ' << symb << " #" << vformat("%u ",unsigned(links.size));
                        os << '{';
                        for(const Link *link=links.head;link;link=link->next)
                        {
                            os << ' ' << (**link).target.name();
                        }
                        os << ' ' << '}';
                    }
                }

            };

            typedef ref_dnode<const Lineage> Member; //!< ...
            typedef ref_dlist<const Lineage> Members; //!< ...

            //__________________________________________________________________
            //
            //
            //! Path
            //
            //__________________________________________________________________
            class Path : public Oriented, public dnode<Path>
            {
            public:
                typedef core::list_of_cpp<Path> List;       //!< alias
                static  const char              CLID[];     //!< "Stream::Path"
                static  const size_t            BaseIndent; //!< for verbosity
                
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit Path(const Edge &edge, List &temp); //!< setup from initial edge
                explicit Path(const Path &);                 //!< copy
                virtual ~Path() throw();                     //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                bool           owns(const Lineage *lineage) const throw();    //!< test ownership
                std::ostream & indent(std::ostream &) const;                  //!< verbosity helper
                void           reshape();                                     //!< re-order members by increasing species index
                static bool    AreAnalog(const Path &, const Path &) throw(); //!< check same species, AFTER reshape of both paths
                void           viz(ios::ostream &, const unsigned c) const;   //!< color rendering

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const bool    isValid; //!< initially false
                const Members members; //!< met along the way
                const Links   roadmap; //!< from start to finish

            private:
                Y_DISABLE_ASSIGN(Path);
                void conn(const Vertex &vhub, List &temp); // first level recursion
                void grow(const Edge   &edge, List &temp); // second level recursion

                void store(const Lineage &);
                void store(const Edge    &);
            };



            //__________________________________________________________________
            //
            //
            //! Graph of all vertices and edges (forward AND reverse)
            //
            //__________________________________________________________________
            class Graph
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit Graph(const Lineage::Array &,
                               const Primary::Array &); //!< create all vertices and edges
                virtual ~Graph() throw();               //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                void graphViz(const string &fileName) const; //!< save and render

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Vertices   lvtx;    //!< lineage vertices
                const Vertices   pvtx;    //!< primary vertices
                const DualEdges  forward; //!< all forward connectivity
                const DualEdges  reverse; //!< all reverse connectivity
                const Path::List paths;   //!< all paths

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Graph);
                bool checkConnectivity()      const throw(); //!< check
                void buildPaths(); //!< building all paths
                void zapAnalogs() throw();
                void expandPath(const Edge &edge);
            };


        }

    }
}

#endif
