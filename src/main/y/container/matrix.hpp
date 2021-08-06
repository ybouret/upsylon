//! \file
#ifndef Y_MATRIX_INCLUDED
#define Y_MATRIX_INCLUDED 1

#include "y/container/matrix_.hpp"
#include "y/type/collapse.hpp"
#include <cstdlib>

namespace upsylon
{

    //__________________________________________________________________________
    //
    //! special for transposition
    //__________________________________________________________________________
    struct matrix_transpose_t {};

    //__________________________________________________________________________
    //
    //! helper for transposition
    //__________________________________________________________________________
    extern const matrix_transpose_t matrix_transpose;

    //__________________________________________________________________________
    //
    //! constructor helper
    //__________________________________________________________________________
#define Y_MATRIX_CTOR(NR,NC) matrix_(NR,NC,sizeof(T)), row_ptr(0), r_aux1(), c_aux1(), r_aux2(), c_aux2()

    //__________________________________________________________________________
    //
    //
    //! versatile matrix
    //
    //__________________________________________________________________________
    template <typename T>
    class matrix : public matrix_
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);                             //!< alias
        typedef lightweight_array<type>       row;       //!< internal row type
        typedef lightweight_array<const_type> const_row; //!< internal row type

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! default constructor
        inline matrix(const size_t nr, const size_t nc) : Y_MATRIX_CTOR(nr,nc)
        {
            initialize();
        }

        //! zero matrix needs no memory
        inline matrix() throw() : Y_MATRIX_CTOR(0,0) {}


        //! destructor
        inline virtual ~matrix() throw()
        {
            destroy(total_items);
        }

        //! copy
        inline matrix(const matrix &other) : Y_MATRIX_CTOR(other.rows,other.cols)
        {
            initialize();
            assert(items==other.items);
            mutable_type *target = get_entry();
            const_type   *source = other.get_entry();
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
                destroy(total_items);
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
                destroy(total_items);
                throw;
            }
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! release memory
        inline virtual void release() throw() { matrix _; swap_with(_); }


        //! temporary array of all items
        inline row as_array() throw()
        {
            if(items>0)
            {
                matrix &self = *this;
                return row(&self[1][1],items);
            }
            else
            {
                return row(0,0);
            }
        }

        //! temporary array of all items
        inline const_row as_array() const throw()
        {
            if(items>0)
            {
                const matrix &self = *this;
                return const_row(&self[1][1],items);
            }
            else
            {
                return const_row(0,0);
            }
        }


        //! row access, use array for contiguous access
        inline array<type>        & operator[](const size_t r) throw()       { assert(r>0); assert(r<=rows); return row_ptr[r]; }

        //! row access, use array for contiguous access
        inline const array<type>  & operator[](const size_t r) const throw() { assert(r>0); assert(r<=rows); return row_ptr[r]; }


        //! display Julia style
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
        
        //! display for maxima
        inline std::ostream & maxima(std::ostream &os) const
        {
            os << "matrix(";
            for(size_t i=1;i<=rows;++i)
            {
                const accessible<T> &R = (*this)[i];
                os << '[';
                for(size_t j=1;j<=cols;++j)
                {
                    os << R[j];
                    if(j<cols) os << ',';
                }
                os << ']';
                if(i<rows) os << ',';
            }
            return os << ")";
        }

        //! display as tableau
        inline std::ostream &tableau(std::ostream &os)
        {
            for(size_t j=rows;j>0;--j)
            {
                os << '|';
                for(size_t i=1;i<=cols;++i)
                {
                    os << ' ' << (*this)[i][j];
                }
                os << ' ' << '|' << '\n';
            }
            return os;
        }



        //! swap and link everything
        inline void swap_with(matrix &other) throw()
        {
            exchange(other);
            setup();
            other.setup();
        }

        //! manual assignment
        template <typename U>
        inline void assign(const matrix<U> &other)
        {
            assert( same_size_than(other) );
            for(size_t i=rows;i>0;--i)
            {
                addressable<type>      &target = (*this)[i];
                const accessible<U>    &source = other[i];
                for(size_t j=cols;j>0;--j)
                {
                    target[j] = source[j];
                }
            }
        }

        //! manual assignment
        template <typename U, typename FUNC>
        inline void assign(const matrix<U> &other, FUNC &func)
        {
            assert( same_size_than(other) );
            for(size_t i=rows;i>0;--i)
            {
                addressable<type>      &target = (*this)[i];
                const accessible<U>    &source = other[i];
                for(size_t j=cols;j>0;--j)
                {
                    target[j] =  func(source[j]);
                }
            }
        }

        //! manual assignment of transpose matrix
        template <typename U>
        inline void assign_transpose(const matrix<U> &other)
        {
            assert( rows==other.cols );
            assert( cols==other.rows );
            for(size_t i=rows;i>0;--i)
            {
                addressable<type> &target = (*this)[i];
                for(size_t j=cols;j>0;--j)
                {
                    target[j] =  other[j][i];
                }
            }
        }

        //! manual assignment of transpose matrix
        template <typename U, typename FUNC>
        inline void assign_transpose(const matrix<U> &other, const FUNC &func)
        {
            assert( rows==other.cols );
            assert( cols==other.rows );
            for(size_t i=rows;i>0;--i)
            {
                addressable<type> &target = (*this)[i];
                for(size_t j=cols;j>0;--j)
                {
                    target[j] = func(other[j][i]);
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
            mutable_type *p = get_entry();
            for(size_t i=0;i<items;++i)
            {
                p[i] = value;
            }
        }

        //! take the opposite (when possible) value
        inline void neg() throw()
        {
            mutable_type *p = get_entry();
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
                addressable<type> &R = self[r];
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
                            self[ir][jr] =  M[i][j];
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

        //! apply to a vector of same type, using internal backup
        template <typename TARGET>
        inline void operator()(TARGET &v) const
        {
            const size_t     nr   = rows;
            const size_t     nc   = cols;
            row             &u    = r_aux2;
            const matrix<T> &self = *this;

            assert(u.size()==rows);
            assert(v.size()==rows);

            for(size_t i=nr;i>0;--i)
            {
                mutable_type    sum = 0;
                const array<T> &r_i = self[i];
                for(size_t j=nc;j>0;--j)
                {
                    sum += v[j] * r_i[j];
                }
                u[i] = sum;
            }
            for(size_t i=nr;i>0;--i)
            {
                v[i]=u[i];
            }
        }

    private:
        row *row_ptr; //! [1..rows]

        inline mutable_type  *get_entry()       throw() { return _data.as<mutable_type>(workspace); }
        inline const_type    *get_entry() const throw() { return _data.as<const_type>(workspace);   }

        //! initialize memory and set every item to default contructor
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
                mutable_type *extra = get_entry()+items;
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
            row_ptr = _rows.as<row>(workspace); //aliasing::cast<row>(workspace,_rows.offset);
            {
                mutable_type *p = get_entry();
                for(size_t r=0;r<rows;++r,p+=cols)
                {
                    new (row_ptr+r) row(p,cols);
                }
            }
            --row_ptr;
        }

        //! destroy allocated objects
        inline void destroy(size_t count) throw()
        {
            mutable_type *entry = get_entry();
            while(count-- > 0)
            {
                collapse(entry[count]);
            }
        }

        //! default initialisation on a zero memory
        inline void init0()
        {
            size_t        count = 0;
            mutable_type *entry = get_entry();
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
                destroy(count);
                throw;
            }
        }
    public:
        mutable row r_aux1;  //!< size() = rows
        mutable row c_aux1;  //!< size() = cols
        mutable row r_aux2;  //!< size() = rows
        mutable row c_aux2;  //!< size() = cols
    };

}

#endif
