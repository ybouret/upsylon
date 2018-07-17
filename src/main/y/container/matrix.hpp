//! \file
#ifndef Y_MATRIX_INCLUDED
#define Y_MATRIX_INCLUDED 1

#include "y/sequence/array.hpp"

namespace upsylon
{

    //! special for transposition
    struct matrix_transpose_t {};
    //! helper for transposition
    extern const matrix_transpose_t matrix_transpose;

    //! data for matrix layout, whith enough space for alebra
    class matrix_data : public object
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
#define Y_MATRIX_CTOR(NR,NC) matrix_data(NR,NC,sizeof(T)), row_ptr(0), r_scalars(), c_scalars(), r_auxiliary(), c_auxiliary()

    //! versatile matrix
    template <typename T>
    class matrix : public matrix_data
    {
    public:
        Y_DECL_ARGS(T,type); //!< alias
        typedef lightweight_array<T> row; //!< internal row type



        //! default constructor
        inline matrix(const size_t nr=0, const size_t nc=0) : Y_MATRIX_CTOR(nr,nc)
        {
            initialize();
        }

        //! row access
        inline array<type>  & operator[](const size_t r) throw()             { assert(r>0); assert(r<=rows); return row_ptr[r]; }

        //! row access
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

        //! assignment
        inline matrix & operator=(const matrix &other )
        {
            if( this != &other )
            {
                matrix tmp(other);
                swap_with(tmp);
            }
            return *this;
        }

        //! change dimension, no further initialization
        inline matrix & make(const size_t nr, const size_t nc)
        {
            if(nr!=rows||nc!=rows)
            {
                matrix tmp(nr,nc);
                swap_with(tmp);
            }
            return *this;
        }

        //! load value
        void ld( param_type value )
        {
            mutable_type *p = memory::io::cast<T>(workspace,data_offset);
            for(size_t i=0;i<items;++i)
            {
                p[i] = value;
            }
        }

        //! load values
        void diag(param_type diag_value,
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
            matrix<T> &self = *this;
            array<T>  &R1   = self[r1];
            array<T>  &R2   = self[r2];
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
    public:
        row r_scalars;   //!< size() = rows
        row c_scalars;   //!< size() = cols
        row r_auxiliary; //!< size() = rows
        row c_auxiliary; //!< size() = cols
    };

}

#endif
