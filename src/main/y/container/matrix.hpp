//! \file
#ifndef Y_MATRIX_INCLUDED
#define Y_MATRIX_INCLUDED 1

#include "y/sequence/array.hpp"
#include "y/ptr/counted.hpp"
#include "y/type/bswap.hpp"
#include "y/type/self-destruct.hpp"
#include <cstdlib>

namespace upsylon
{

    //! special for transposition
    struct matrix_transpose_t {};
    //! helper for transposition
    extern const matrix_transpose_t matrix_transpose;

    //! data for matrix layout, whith enough space for alebra
    class matrix_data : public counted_object
    {
    public:
        const size_t rows;            //!< number of rows
        const size_t cols;            //!< number of columns
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

        lightweight_array<size_t> r_indices; //!< size() = rows
        lightweight_array<size_t> c_indices; //!< size() = cols

        //! destructor
        virtual ~matrix_data() throw();
        
        //! item = (r-1)*cols+(c-1) in [0:items-1]
        inline void get_item( const size_t item, size_t &r, size_t &c) const
        {
            assert(item<items);
            const ldiv_t d = ldiv(long(item),long(cols));
            r=d.quot+1; assert(r>0); assert(r<=rows);
            c=d.rem +1; assert(c>0); assert(c<=cols);
            assert((r-1)*cols+(c-1)==item);
        }

        //! test dimensions
        inline bool same_size_than( const matrix_data &other ) throw()
        {
            return (rows==other.rows) && (cols==other.cols);
        }
        
    protected:
        void  *workspace; //!< where all memory is
        //!constructor
        explicit matrix_data(const size_t nr, const size_t nc, const size_t item_size);

        void   hook() throw(); //!< reset arrays
        void   exchange( matrix_data &other ) throw(); //!< full exchange

    private:
        Y_DISABLE_COPY_AND_ASSIGN(matrix_data);
        void __kill() throw();
    };


    //! constructor helper
#define Y_MATRIX_CTOR(NR,NC) object(), matrix_data(NR,NC,sizeof(T)), row_ptr(0), r_aux1(), c_aux1(), r_aux2(), c_aux2()

    //! versatile matrix
    template <typename T>
    class matrix : public matrix_data
    {
    public:
        Y_DECL_ARGS(T,type);                 //!< alias
        typedef lightweight_array<type> row; //!< internal row type
        

        //! default constructor
        inline matrix(const size_t nr, const size_t nc) : Y_MATRIX_CTOR(nr,nc)
        {
            initialize();
        }

        //! zero matrix needs no memory
        inline matrix() throw() : Y_MATRIX_CTOR(0,0)
        {

        }

        //! release memory
        inline void release() throw()
        {
            matrix empty;
            swap_with(empty);
        }


        //! temporary array of all items
        inline lightweight_array<type> as_array() throw()
        {
            if(items>0)
            {
                matrix &self = *this;
                return lightweight_array<type>(&self[1][1],items);
            }
            else
            {
                return row(0,0);
            }
        }
        
        //! row access, use array for contiguous access
        inline array<type>  & operator[](const size_t r) throw()             { assert(r>0); assert(r<=rows); return row_ptr[r]; }

        //! row access, use array for contiguous access
        inline const array<type>  & operator[](const size_t r) const throw() { assert(r>0); assert(r<=rows); return row_ptr[r]; }

        //! destructor
        inline virtual ~matrix() throw()
        {
            __clear(total_items);
        }

        //! display Octave/Julia style
        inline friend std::ostream & operator<<( std::ostream &os, const matrix &m )
        {
            os << '[';
            for(size_t r=1;r<=m.rows;++r)
            {
                const accessible<T> &R = m[r];
                for(size_t c=1;c<=m.cols;++c)
                {
                    os << R[c]; if(c<m.cols) os << ' ';
                }
                if(r<m.rows) os << ';';
            }
            os << ']';
            return os;
        }

        //! copy
        inline matrix(const matrix &other) : Y_MATRIX_CTOR(other.rows,other.cols)
        {
            initialize();
            assert(items==other.items);
            mutable_type *target = memory::io::cast<mutable_type>(workspace,data_offset);
            const_type   *source = memory::io::cast<mutable_type>(other.workspace,other.data_offset);
            size_t        count  = 0;
            try
            {
                while(count<items)
                {
                    target[count] = source[count];
                    ++count;
                }
            }
            catch(...)
            {
                __clear(total_items);
                throw;
            }
        }

        //! copy transpose
        inline matrix(const matrix &other, const matrix_transpose_t & ) : Y_MATRIX_CTOR(other.cols,other.rows)
        {
            initialize();
            try
            {
                matrix &self = *this;
                for(size_t r=rows;r>0;--r)
                {
                    for(size_t c=cols;c>0;--c)
                    {
                        self[r][c] = other[c][r];
                    }
                }
            }
            catch(...)
            {
                __clear(total_items);
                throw;
            }
        }


        //! swap and link everything
        inline void swap_with( matrix &other ) throw()
        {
            exchange(other);
            setup();
            other.setup();
        }

        //! manual assignment
        template <typename U>
        inline void assign( const matrix<U> &other )
        {
            assert( same_size_than(other) );
            for(size_t i=rows;i>0;--i)
            {
                addressable<type>      &target = (*this)[i];
                const accessible<U>    &source = other[i];
                for(size_t j=cols;j>0;--j)
                {
                    target[j] = static_cast<type>(source[j]);
                }
            }
        }

        //! manual assignment of transpose matrix
        template <typename U>
        inline void assign_transpose( const matrix<U> &other )
        {
            assert( rows==other.cols );
            assert( cols==other.rows );
            for(size_t i=rows;i>0;--i)
            {
                addressable<type> &target = (*this)[i];
                for(size_t j=cols;j>0;--j)
                {
                    target[j] = static_cast<type>(other[j][i]);
                }
            }
        }


        
        //! assignment
        inline matrix & operator=(const matrix &other)
        {
            if( this != &other )
            {
                if( same_size_than(other) )
                {
                    assign(other);
                }
                else
                {
                    matrix tmp(other);
                    swap_with(tmp);
                }
            }
            return *this;
        }

        //! change dimension, no further initialization
        inline matrix & make(const size_t nr, const size_t nc)
        {
            if( (nr!=rows) || (nc!=cols) )
            {
                matrix tmp(nr,nc);
                swap_with(tmp);
            }
            assert(rows==nr);
            assert(cols==nc);
            return *this;
        }

        //! load value
        inline void ld( param_type value )
        {
            mutable_type *p = memory::io::cast<T>(workspace,data_offset);
            for(size_t i=0;i<items;++i)
            {
                p[i] = value;
            }
        }

        //! take the opposite when possible value
        inline void neg() throw()
        {
            mutable_type *p = memory::io::cast<T>(workspace,data_offset);
            for(size_t i=0;i<items;++i)
            {
                p[i] = -p[i];
            }
        }

        //! load values
        inline void diag(param_type diag_value,
                         param_type extra_value)
        {
            matrix &self = *this;
            for(size_t r=rows;r>0;--r)
            {
                array<type> &R = self[r];
                for(size_t c=cols;c>0;--c)
                {
                    R[c] = (r==c) ? diag_value : extra_value;
                }
            }
        }
        
        //! load values
        inline void diag( const array<T> &d )
        {
            assert(d.size()>=rows);
            assert(is_square);
            matrix     &self = *this;
            const_type  zero(0);
            for(size_t r=rows;r>0;)
            {
                array<type> &R = self[r];
                for(size_t c=cols;c>r;--c)
                {
                    R[c] = zero;
                }
                R[r] = d[r];
                --r;
                for(size_t c=r;c>0;--c)
                {
                    R[c] = zero;
                }
            }
        }
        
        //! load indentity
        inline void Id()
        {
            assert(is_square);
            matrix      &self = *this;
            const_type   zero(0);
            const_type   one(1);
            for(size_t r=rows;r>0;)
            {
                addressable<type> &R = self[r];
                for(size_t c=cols;c>r;--c)
                {
                    R[c] = zero;
                }
                R[r] = one;
                --r;
                for(size_t c=r;c>0;--c)
                {
                    R[c] = zero;
                }
            }
        }
        
        //! direct access of [0:items-1]
        inline type * operator*() throw()
        {
            assert(items>0);
            matrix<T>   &self = *this;
            return &self[1][1];
        }
        
        //! direct access of [0:items-1]
        inline const_type * operator*() const throw()
        {
            assert(items>0);
            const matrix<T>   &self = *this;
            return &self[1][1];
        }
        
        //! store the minor_{I,J} of M
        template <typename U>
        inline void minor_of(const matrix<U>       &M,
                             const size_t           I,
                             const size_t           J) throw()
        {
            assert(M.rows>1);
            assert(M.cols>1);
            assert(this->rows==M.rows-1);
            assert(this->cols==M.cols-1);

            matrix<T>   &self = *this;
            const size_t nr = M.rows;
            const size_t nc = M.cols;
            for(size_t i=1,ir=1;i<=nr;++i)
            {
                if(i!=I)
                {
                    for(size_t j=1,jr=1;j<=nc;++j)
                    {
                        if(j!=J)
                        {
                            self[ir][jr] = T(M[i][j]);
                            ++jr;
                        }
                    }
                    ++ir;
                }
            }
        }

        //! swap rows content
        inline void swap_rows(const size_t r1,const size_t r2) throw()
        {
            matrix<T>       &self = *this;
            addressable<T>  &R1   = self[r1];
            addressable<T>  &R2   = self[r2];
            for(size_t i=cols;i>0;--i)
            {
                bswap(R1[i],R2[i]);
            }
        }

        //! swap cols content
        inline void swap_cols(const size_t c1, const size_t c2) throw()
        {
            matrix<T> &self = *this;
            for(size_t i=rows;i>0;--i)
            {
                bswap(self[i][c1],self[i][c2]);
            }
        }

        //! swap both columns and rows
        inline void swap_both(size_t i,size_t j) throw()
        {
            swap_rows(i,j);
            swap_cols(i,j);
        }

        //! mod2 for one row


    private:
        row *row_ptr; //! [1..rows]

        inline void initialize()
        {
            setup();
            init0();
        }

        //! just preparing memory layout
        void setup() throw()
        {
            //__________________________________________________________________
            //
            // prepare auxiliary arrays
            //__________________________________________________________________
            {
                T *extra = static_cast<T*>(workspace)+items;
                new (&r_aux1) row(extra,rows);
                new (&c_aux1) row(extra,cols);
                extra += largest;
                new (&r_aux2) row(extra,rows);
                new (&c_aux2) row(extra,cols);
            }
            //__________________________________________________________________
            //
            // prepare rows
            //__________________________________________________________________
            row_ptr = memory::io::cast<row>(workspace,rows_offset);
            {
                mutable_type *p = memory::io::cast<mutable_type>(workspace,data_offset);
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
            while(count-- > 0)
            {
                self_destruct(entry[count]);
            }
        }

        //! default initialisation on a zero memory
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
    public:
        mutable row r_aux1;  //!< size() = rows
        mutable row c_aux1;  //!< size() = cols
        row r_aux2;          //!< size() = rows
        row c_aux2;          //!< size() = cols
    };

}

#endif
