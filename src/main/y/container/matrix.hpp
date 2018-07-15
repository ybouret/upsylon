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


    template <typename T>
    class matrix : public matrix_data
    {
    public:
        Y_DECL_ARGS(T,type);
        typedef lightweight_array<T> row;

        row r_scalars;   //!< size() = rows
        row c_scalars;   //!< size() = cols
        row r_auxiliary; //!< size() = rows
        row c_auxiliary; //!< size() = cols

        inline matrix(const size_t nr=0, const size_t nc=0) :
        matrix_data(nr,nc,sizeof(T)), row_ptr(0)
        {
            setup();
            init0();
        }

        //! row access
        inline array<T>  & operator[](const size_t r) throw()       { assert(r>0); assert(r<=rows); return row_ptr[r]; }
        //! row access
        inline const array<T>  & operator[](const size_t r) const throw() { assert(r>0); assert(r<=rows); return row_ptr[r]; }

        //! destructor
        inline virtual ~matrix() throw()
        {
            __clear(total_items);
        }

        inline friend std::ostream & operator<<( std::ostream &os, const matrix &m )
        {
            os << '[';
            for(size_t r=1;r<=m.rows;++r)
            {
                const array<T> &R = m[r];
                for(size_t c=1;c<=m.cols;++c)
                {
                    os << R[c]; if(c<m.cols) os << ' ';
                }
                if(r<m.rows) os << ';';
            }
            os << ']';
            return os;
        }
        

    private:
        Y_DISABLE_COPY_AND_ASSIGN(matrix);
        row *row_ptr; //! [1..rows]

        //! just preparing memory
        void setup() throw()
        {
            //__________________________________________________________________
            //
            // prepare auxiliary arrays
            //__________________________________________________________________
            {
            T *extra = static_cast<T*>(workspace)+items;
            new (&r_scalars) row(extra,rows);
            new (&c_scalars) row(extra,cols);
            extra += largest;
            new (&r_auxiliary) row(extra,rows);
            new (&c_auxiliary) row(extra,cols);
            }
            //__________________________________________________________________
            //
            // prepare rows
            //__________________________________________________________________
            row_ptr = memory::io::cast<row>(workspace,rows_offset);
            {
                T *p = memory::io::cast<T>(workspace,data_offset);
                for(size_t r=0;r<rows;++r,p+=cols)
                {
                    new (row_ptr+r) row(p,cols);
                }
            }
            --row_ptr;
        }

        inline void __clear(size_t count) throw()
        {
            mutable_type *entry = memory::io::cast<mutable_type>(workspace,data_offset);
            while(count-->0)
            {
                destruct(entry+count);
            }
        }

        //! default initialisation
        inline void init0()
        {
            size_t        count = 0;
            mutable_type *entry = memory::io::cast<mutable_type>(workspace,data_offset);
            try
            {
                while(count<total_items)
                {
                    new (entry+count) T();
                    ++count;
                }
            }
            catch(...)
            {
                __clear(count);
                throw;
            }
        }
    };

}

#endif
