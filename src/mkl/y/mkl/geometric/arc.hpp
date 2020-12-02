
//! \file
#ifndef Y_GEOMETRIC_ARC_INCLUDED
#define Y_GEOMETRIC_ARC_INCLUDED 1

#include "y/mkl/geometric/node.hpp"
#include "y/associative/hash/set.hpp"
#include "y/sequence/vector.hpp"
#include <typeinfo>

namespace upsylon
{

    namespace mkl
    {

        namespace Geometric
        {

            //__________________________________________________________________
            //
            //! common stuff for an arc
            //__________________________________________________________________
            class Arc_ : public Object
            {
            public:
                virtual ~Arc_() throw(); //!< cleanup

            protected:
                explicit Arc_() throw(); //!< setup
                
                //! throw exception, shouldn't happen
                void   nodeInsertFailure(const std::type_info &tid) const;



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arc_);
            };

            //__________________________________________________________________
            //
            //! base class for an arc
            //__________________________________________________________________
            template <typename T, template <class> class VTX>
            class Arc : public Arc_
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                Y_DECL_ARGS(T,type);                                     //!< aliases
                typedef Point<T,VTX>                     PointType;      //!< alias
                typedef typename PointType::Pointer      SharedPoint;    //!< alias
                typedef typename PointType::vertex       vertex;         //!< alias
                typedef Node<T,VTX>                      NodeType;       //!< alias
                typedef typename NodeType::Pointer       SharedNode;     //!< alias
                typedef hash_set<NodeKey,SharedNode>     Nodes;          //!< alias
                typedef            vector<vertex>        Coefficients; //!< alias

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                //! cleanup
                inline virtual ~Arc() throw() {}

                //! insert new point, update segments

                inline void insert(const SharedPoint &point)
                {
                    const SharedNode node = new NodeType(point);
                    if(! aliasing::_(nodes).insert(node) ) this->nodeInsertFailure( typeid(vertex) );
                    aliasing::_(compiled) = false;
                }

                //! tau in [1:tauMax]
                virtual type tauMax() const throw() = 0;


                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Nodes        nodes; //!< table/list of nodes
                const Coefficients A;
                const Coefficients B;
                
            protected:
                //! setup empty
                explicit Arc() : nodes(),  A(), B(), compiled(false)
                {
                }
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arc);
            public:
                const bool compiled; //!< flag
            };

        }

    }

}

#endif
