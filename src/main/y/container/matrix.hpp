//! \file
#ifndef Y_MATRIX_INCLUDED
#define Y_MATRIX_INCLUDED 1

#include "y/sequence/array.hpp"

namespace upsylon
{

    class matrix_data : public object
    {
    public:
        const size_t rows;
        const size_t cols;
        const size_t items;           //!< rows*cols
        const bool   is_square;       //!< rows==cols
        const size_t largest;         //!< max_of(cols,rows)
        const size_t total_items;     //!< items+2*largest
        const size_t data_offset;     //!< 0
        const size_t data_length;     //!< total_items*item_size
        const size_t rows_offset;     //!< |data_offset+indx_offsest|
        const size_t rows_length;     //!< rows * sizeof( lightweigt_array<...> )
        const size_t indx_offset;     //!< |rows_offset+rows_length|
        const size_t indx_length;     //!< largest * sizeof(size_t)
        const size_t allocated;       //!< |indx_offset+indx_length|

        lightweight_array<size_t> r_indx; //!< size() = rows
        lightweight_array<size_t> c_indx; //!< size() = cols

        virtual ~matrix_data() throw();
        explicit matrix_data(const size_t nr, const size_t nc, const size_t item_size);

    protected:
        void  *workspace;

    private:
        Y_DISABLE_ASSIGN(matrix_data);
        void __kill() throw();
    };

}

#endif
