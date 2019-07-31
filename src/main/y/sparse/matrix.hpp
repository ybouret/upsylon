//! \file
#ifndef Y_SPARSE_MATRIX_INCLUDED
#define Y_SPARSE_MATRIX_INCLUDED 1

#include "y/sparse/dok.hpp"
#include "y/container/const-field.hpp"
#include "y/comparison.hpp"

namespace upsylon
{

    namespace sparse
    {
        //! (r,c) as key
        class matrix_key
        {
        public:
            typedef key_hasher<matrix_key,hashing::fnv> hasher;                     //!< alias
            const size_t r;                                                         //!< row index
            const size_t c;                                                         //!< column index
            matrix_key(const size_t R, const size_t C) throw();                     //!< setup
            ~matrix_key() throw();                                                  //!< cleanup
            matrix_key(const matrix_key &other) throw();                            //!< copy
            friend bool operator==(const matrix_key &, const matrix_key &) throw(); //!< equality
            friend bool operator!=(const matrix_key &, const matrix_key &) throw(); //!< difference
            friend bool operator< (const matrix_key &, const matrix_key &) throw(); //!< lexicographic

            //! display
            friend std::ostream & operator<<( std::ostream &os, const matrix_key &k );

        private:
            Y_DISABLE_ASSIGN(matrix_key);
        };

        //! shared info
        class matrix_info
        {
        public:
            virtual ~matrix_info() throw();        //!< cleanup
            explicit matrix_info(const size_t nr,  //|
                                 const size_t nc); //!< setup

            const size_t rows;  //!< number of rows
            const size_t cols;  //!< number of columns
            const size_t count; //!< rows*cols
            
            void check_indices(const size_t r,const size_t c) const; //!< check indices
            void insert_failure(const sparse::matrix_key  &k) const; //!< unexpected

        private:
            Y_DISABLE_COPY_AND_ASSIGN(matrix_info);
        };
    }

    //! front-end to an internal dictionay of keys
    template <typename T, typename ALLOCATOR = memory::global>
    class sparse_matrix :
    public sparse::matrix_info,
    public const_field<T>
    {
    public:
        Y_DECL_ARGS(T,type);                                                       //!< aliases
        typedef sparse::matrix_key                                 key_type;       //!< key type
        typedef sparse::dok<key_type,T,key_type::hasher,ALLOCATOR> dok_type;       //!< alias
        typedef typename dok_type::iterator                        iterator;       //!< alias
        typedef typename dok_type::const_iterator                  const_iterator; //!< alias
        typedef typename dok_type::item_type                       item_type;      //!< alias
        typedef typename dok_type::item_ptr                        item_ptr;       //!< alias

        //! setup with rows=nr and cols=nc
        inline explicit sparse_matrix( const size_t nr, const size_t nc ) :
        sparse::matrix_info(nr,nc), const_field<T>(), items(), core(items)
        {
        }

        //! cleainp
        inline virtual ~sparse_matrix() throw() {}

        //! access with on-the-fly creation
        inline type & operator()(const size_t r, const size_t c)
        {
            check_indices(r,c);
            const key_type ik(r,c);
            item_ptr *ppI = items.search( ik );
            if( ppI )
            {
                return (**ppI).value;
            }
            else
            {
                item_ptr p = new item_type(ik,this->value);
                if(!items.insert(p)) insert_failure(ik);
                return p->value;
            }
        }

        //! access with 'zero' by default
        inline const_type & operator()(const size_t r, const size_t c) const
        {
            check_indices(r,c);
            const key_type  ik(r,c);
            const item_ptr *ppI = items.search( ik );
            if(ppI)
            {
                return (**ppI).value;
            }
            else
            {
                return this->value;
            }
        }


        inline iterator       begin() throw() { return items.begin(); } //!< forward iterator begin
        inline iterator       end()   throw() { return items.end();   } //!< forward iterator end

        inline const_iterator begin() const throw() { return items.begin(); } //!< forward iterator begin, const
        inline const_iterator end()   const throw() { return items.end();   } //!< forward iterator end, const

        //! display as a dense matrix
        inline friend std::ostream & operator<<( std::ostream &os, const sparse_matrix &M )
        {
            os << '[';
            for(size_t r=1;r<=M.rows;++r)
            {
                for(size_t c=1;c<=M.cols;++c)
                {
                    os << ' ' << M(r,c);
                }
                if(r<M.rows) os << ';';
            }
            os << ']';
            return os;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(sparse_matrix);
        dok_type    items; //!< data handling


    public:
        container & core; //!< interface for internal memory management
    };
    

}

#endif

