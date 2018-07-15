//! \file
#ifndef Y_MATRIX_INCLUDED
#define Y_MATRIX_INCLUDED 1

#include "y/object.hpp"

namespace upsylon
{

    class matrix_dims : public object
    {
    public:
        const size_t rows;
        const size_t cols;
        const size_t items;           //!< rows*cols
        const bool   is_square;       //!< rows==cols
        
        virtual ~matrix_dims() throw();

    protected:
        explicit matrix_dims(const size_t nr, const size_t nc, const size_t item_size);

    private:
        Y_DISABLE_ASSIGN(matrix_dims);
    };

}

#endif
