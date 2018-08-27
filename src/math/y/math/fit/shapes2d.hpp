//! \file
#ifndef Y_FIT_SHAPES2D_INCLUDED
#define Y_FIT_SHAPES2D_INCLUDED 1

#include "y/type/point2d.hpp"
#include "y/sequence/arrays.hpp"
#include "y/sequence/vector.hpp"
#include "y/container/matrix.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            typedef arrays<double,memory::global> Arrays; //!< for internal arrays

            //! base class to perform computation
            class Shape2D : public Arrays
            {
            public:
                //! destructor
                virtual ~Shape2D() throw();


            protected:
                explicit Shape2D(const size_t num_arrays); //!< memory for num_arrays

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Shape2D);
            };
            
        }
      

    }
}

#endif

