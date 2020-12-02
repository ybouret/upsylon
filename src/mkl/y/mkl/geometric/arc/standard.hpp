
//! \file
#ifndef Y_GEOMETRIC_ARC_STANDARD_INCLUDED
#define Y_GEOMETRIC_ARC_STANDARD_INCLUDED 1

#include "y/mkl/geometric/arc.hpp"

namespace upsylon
{

    namespace mkl
    {

        namespace Geometric
        {
            //__________________________________________________________________
            //
            //! standard arc
            //__________________________________________________________________
            template <typename T,template <class> class VTX>
            class StandardArc : public Arc<T,VTX>
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
                inline explicit StandardArc() : ArcType() {} //!< setup
                inline virtual ~StandardArc() throw() {}     //!< cleanup


                
                inline virtual type tauMax() const throw()
                {
                    return type(this->nodes.size());
                }


                
               

            private:
                Y_DISABLE_COPY_AND_ASSIGN(StandardArc);
                

            };

        }

    }

}

#endif
