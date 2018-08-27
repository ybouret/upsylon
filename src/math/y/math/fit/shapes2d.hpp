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
        typedef arrays<double,memory::global> shape2d_arrays; //!< for internal arrays

        //! base class to perform computation
        class shape2d : public shape2d_arrays
        {
        public:
            //! shapoe
            virtual ~shape2d() throw();


        protected:
            explicit shape2d(const size_t num_arrays); //!< memory for num_arrays
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(shape2d);
        };

      

    }
}

#endif

