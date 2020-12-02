

//! \file
#ifndef Y_GEOMETRIC_ARC_PERIODIC_INCLUDED
#define Y_GEOMETRIC_ARC_PERIODIC_INCLUDED 1

#include "y/mkl/geometric/arc.hpp"

namespace upsylon
{

    namespace mkl
    {

        namespace Geometric
        {

            //__________________________________________________________________
            //
            //! periodic arc
            //__________________________________________________________________
            template <typename T,template <class> class VTX>
            class PeriodicArc : public Arc<T,VTX>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                Y_DECL_ARGS(T,type);                                   //!< aliases
                typedef Arc<T,VTX> ArcType;                            //!< alias
                typedef typename   ArcType::SharedPoint  SharedPoint;  //!< alias
                typedef typename   ArcType::NodeType     NodeType;     //!< alias
                typedef typename   ArcType::SharedNode   SharedNode;   //!< alias
                typedef typename   ArcType::vertex       vertex;       //!< alias
                typedef typename   ArcType::Nodes        Nodes;        //!< alias
                
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                inline explicit PeriodicArc() : ArcType() {}
                inline virtual ~PeriodicArc() throw() {}

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                inline virtual type tauMax() const throw()
                {
                    return type(this->nodes.size()+1);
                }

                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(PeriodicArc);


            };

        }

    }

}

#endif
