
//! \file
#ifndef Y_CHEMICAL_FLUX_INCLUDED
#define Y_CHEMICAL_FLUX_INCLUDED 1

#include "y/chemical/sys/strain.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/core/rlinked.hpp"
#include "y/associative/be-keys.hpp"
#include "y/associative/suffix/set.hpp"

namespace upsylon
{
    namespace Chemical
    {
        //! alias
        typedef ios::vizible Vizible;

        //______________________________________________________________________
        //
        //
        //! Matter Flux API
        //
        //______________________________________________________________________
        struct Flux
        {
            class Vertex; //!< forward declaration

            //__________________________________________________________________
            //
            //
            //! an Edge connect two vertices
            //
            //__________________________________________________________________
            class Edge : public Object
            {
            public:
                //______________________________________________________________
                //
                // types and definition
                //______________________________________________________________
                typedef ref_dnode<const Edge>   Node;     //!< for vertex edges
                typedef ref_list<const Edge>    List;     //!< for vertex edges
                typedef be_keys<2>              Key;      //!< UUID from 2 vertices
                typedef intr_ptr<Key,Edge>      Pointer;  //!< alias for Set
                typedef suffix_set<Key,Pointer> Set;      //!< database of edges
                typedef Set::data_node          Iter;     //!< iterator on database

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                virtual ~Edge() throw();

                //! setup
                explicit Edge(const Vertex &ini, const Vertex &end, const unit_t  cof) throw() ;

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                const Key & key()      const throw(); //!< UUID
                void        viz(ios::ostream&) const; //!< standalone graphViz code


                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Vertex &source; //!< source Vertex
                const Vertex &target; //!< target Vertex
                const unit_t  weight; //!< stoichiometry (>0 here!)


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Edge);
                const Key     vkey;
            };


            //__________________________________________________________________
            //
            //
            //! a vertex hold a Strain/Species or a Primary/Equilibrium
            //
            //__________________________________________________________________
            class Vertex : public Object
            {
            public:
                //______________________________________________________________
                //
                // types and definition
                //______________________________________________________________
                typedef arc_ptr<const Vertex>     Pointer; //!< alias
                typedef vector<Pointer,Allocator> Array;   //!< alias

                //! genus of this vertux
                enum Genus
                {
                    IsStrain, //!< species
                    IsPrimary //!< equilibrium
                };



                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~Vertex() throw();                 //!< cleanup
                explicit Vertex(const Strain  &) throw();  //!< setup as Strain
                explicit Vertex(const Primary &) throw();  //!< setup as Primary

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                void          viz(ios::ostream &) const; //!< standalone graphViz code
                const char   *name()      const throw(); //!< genus depending
                void          display()           const; //!< info

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Genus        genus;    //!< identifier
                union {
                    const Strain  *strain;   //!< in case of strain
                    const Primary *primary;  //!< in case of primary
                };
                const Edge::List   outgoing; //!< edges from this
                const Edge::List   incoming; //!< edges to   this

                bool hasOutgoing(const Edge &) const throw(); //!< test to debug

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Vertex);
                static void Display(const Edge::List &, const bool);
            };


            typedef ref_dnode<const Strain>  sNode; //!< alias
            typedef ref_list<const Strain>   sList; //!< alias

            //__________________________________________________________________
            //
            //
            //! path in the graph
            //
            //__________________________________________________________________
            class Path : public Object, public dnode<Path>
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                Path(const Edge &) throw(); //!< setup with first edge
                Path(const Path &);         //!< full duplication
                virtual ~Path()    throw(); //!< cleanup
                
                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Edge::List edges; //!< travelled edges
                const sList      slist; //!< met species


            private:
                Y_DISABLE_ASSIGN(Path);
                void setup(const Edge &edge);
                void push(const Edge   &);
                void push(const Strain *);
            };

            //__________________________________________________________________
            //
            //
            //! Build a graph from all strains and primary
            //
            //__________________________________________________________________
            class Graph
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup
                explicit Graph(const Strain::Array  &strain,
                               const Primary::Array &primary);

                //! cleanup
                virtual ~Graph() throw();

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                void graphViz(const string &) const; //!< save/render
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Vertex::Array svtx;   //!< all unique strains/species
                const Vertex::Array pvtx;   //!< all unique primaries/equilibria
                const Edge::Set     edges;  //!< all unique connecting edges

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Graph);
                void        connect(const Strain::Array &);
                const Edge &query(const Vertex *ini, const Vertex *end, const unit_t cof);
                static void Save(ios::ostream &fp, const Vertex::Array &arr);
                void        join(ios::ostream &fp) const;





            };

            


        };


    }

}

#endif
