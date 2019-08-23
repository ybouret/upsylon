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

            //! test if diagonal term
            bool is_diagonal() const throw();

            //! wrapper
            static bool isDiagonal( const matrix_key &k ) throw();

            //! test if extra diagonal
            bool is_extra_diagonal() const throw();

            //! wrapper
            static bool isExtraDiagonal( const matrix_key & ) throw();

        private:
            Y_DISABLE_ASSIGN(matrix_key);
        };

        //! shared info
        class matrix_info
        {
        public:
            virtual ~matrix_info() throw();                    //!< cleanup
            explicit matrix_info(const size_t nr,              //|
                                 const size_t nc);             //!< setup
            explicit matrix_info(const matrix_info &) throw(); //!< copy

            const size_t rows;  //!< number of rows
            const size_t cols;  //!< number of columns
            const size_t count; //!< rows*cols
            
            void check_indices(const size_t r,const size_t c) const; //!< check indices
            void insert_failure(const sparse::matrix_key  &k) const; //!< unexpected

            bool has_same_sizes_than( const matrix_info &other) const throw(); //!< check same sizes
            void check_sames_sizes_than(const matrix_info &other) const;       //!< exception if not
            
        private:
            Y_DISABLE_ASSIGN(matrix_info);
        };
    }

    //! front-end to an internal dictionay of keys
    template <typename T, typename ALLOCATOR = memory::global>
    class sparse_matrix :
    public sparse::matrix_info,
    public const_field<T>
    {
    public:
        //----------------------------------------------------------------------
        //
        // types
        //
        //----------------------------------------------------------------------
        Y_DECL_ARGS(T,type);                                                       //!< aliases
        typedef sparse::matrix_key                                 key_type;       //!< key type
        typedef sparse::dok<key_type,T,key_type::hasher,ALLOCATOR> dok_type;       //!< alias
        typedef typename dok_type::iterator                        iterator;       //!< alias
        typedef typename dok_type::const_iterator                  const_iterator; //!< alias
        typedef typename dok_type::item_type                       item_type;      //!< alias
        typedef typename dok_type::item_ptr                        item_ptr;       //!< alias

        //----------------------------------------------------------------------
        //
        // setup and access
        //
        //----------------------------------------------------------------------

        //! setup with rows=nr and cols=nc
        inline explicit sparse_matrix( const size_t nr, const size_t nc ) :
        sparse::matrix_info(nr,nc), const_field<T>(), items(), core(items)
        {
        }


        //! copy
        inline explicit sparse_matrix( const sparse_matrix &other ) :
        sparse::matrix_info(other), const_field<T>(), items(other.items), core(items)
        {
            
        }



        //! destructor
        inline virtual ~sparse_matrix() throw() {}

        //! raw insertion
        inline type & create_at(const key_type &k, param_type v)
        {
            item_ptr p = new item_type(k,v);
            if(!items.insert(p)) insert_failure(k);
            return p->value;
        }

        //! access with on-the-fly creation
        inline type & operator()(const size_t r, const size_t c)
        {
            check_indices(r,c);
            const key_type ik(r,c);
            item_ptr *ppI = items.search( ik );
            if( ppI )
            {
                return ***ppI;
            }
            else
            {
                return create_at(ik,this->value);
            }
        }

        //! access with 'zero' by default
        inline const_type & operator()(const size_t r, const size_t c) const
        {
            check_indices(r,c);
            const key_type  ik(r,c);
            const item_ptr *pp = items.search( ik );
            return (pp!=NULL) ? (***pp) : this->value;
        }


        //! sort keys by increasing order
        inline void update()
        {
            items.sort_keys( comparison::increasing<key_type> );
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
                    if(c>1) os << ' ';
                    os << M(r,c);
                }
                if(r<M.rows) os << ';';
            }
            os << ']';
            return os;
        }

        //----------------------------------------------------------------------
        //
        // algebraic tools
        //
        //----------------------------------------------------------------------

        //! simulate cleanup
        inline void ldz() { items.free(); }

        //! load diagonal, doesn't remove other items
        inline void load_diagonal( param_type value )
        {
            sparse_matrix &self = *this;
            const size_t   m    = min_of(rows,cols);
            for(size_t i=1;i<=m;++i)
            {
                self(i,i) = value;
            }
        }

        //! remove diagonal terms
        inline void keep_diagonal()
        {
            items.remove_key_if( key_type::isExtraDiagonal );
        }

        //! clean diagonal
        inline void zero_diagonal()
        {
            items.remove_key_if( key_type::isDiagonal );
        }

        //! copy diagonal
        inline void copy_diagonal( const sparse_matrix &other )
        {
            if(this!=&other)
            {
                sparse_matrix &self = *this;
                self.check_sames_sizes_than(other);
                self.zero_diagonal();
                size_t         n    = other.items.size();
                const_iterator i    = other.items.begin();
                while(n-->0)
                {
                    const item_ptr &p = *i;
                    const key_type &k = p->__key;
                    if( k.is_diagonal() )
                    {
                        (void) create_at(k,p->value);
                    }
                    ++i;
                }
            }

        }

    private:
        Y_DISABLE_ASSIGN(sparse_matrix);
        dok_type    items; //!< data handling

    public:
        container & core; //!< interface for internal memory management
    };
    

}

#endif

