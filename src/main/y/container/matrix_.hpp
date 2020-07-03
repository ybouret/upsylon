//! \file

#ifndef Y_MATRIX__INCLUDED
#define Y_MATRIX__INCLUDED 1

#include "y/ptr/counted.hpp"
#include "y/sequence/array.hpp"
#include "y/memory/marker.hpp"

namespace upsylon {

    //__________________________________________________________________________
    //
    //
    //! data for matrix layout, whith enough space for algebra
    //
    //__________________________________________________________________________
    class matrix_ : public counted_object
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef lightweight_array<size_t> indices_type; //!< alias
        typedef memory::marker            marker;       //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~matrix_() throw(); //!< destructor

        //______________________________________________________________________
        //
        // Methods
        //______________________________________________________________________

        //! item = (r-1)*cols+(c-1) in [0:items-1]
        void get_item(const size_t item, size_t &r, size_t &c) const throw();

        //! test dimensions
        bool same_size_than(const matrix_ &) const throw();

        //______________________________________________________________________
        //
        // Members
        //______________________________________________________________________
        const size_t rows;            //!< number of rows
        const size_t cols;            //!< number of columns
        const size_t items;           //!< rows*cols
        const bool   is_square;       //!< rows==cols
        const size_t largest;         //!< max_of(cols,rows)
        const size_t total_items;     //!< items+2*largest
        const marker _data;           //!< 0,total_items*item_size
        const marker _rows;           //!< _data+rows*sizeof(lightweight_array<...>)
        const marker _indx;           //!< _indx+ largest*sizeof(size_t)
        const size_t allocated;       //!< |indx_offset+indx_length|
        indices_type r_indices;       //!< size() = rows
        indices_type c_indices;       //!< size() = cols

    protected:

        explicit matrix_(const size_t nr,
                         const size_t nc,
                         const size_t item_size);        //!< constructor
        void     hook() throw();                         //!< reset arrays
        void     exchange( matrix_ &other ) throw();     //!< full exchange
        void *   workspace;                              //!< where all memory is
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(matrix_);
        void __free() throw();
    };

}

#endif

