//! \file
#ifndef Y_SPARSE_ARRAY_INCLUDED
#define Y_SPARSE_ARRAY_INCLUDED 1

#include "y/sparse/dok.hpp"
#include "y/container/const-field.hpp"
#include "y/comparison.hpp"

namespace upsylon
{

    namespace sparse
    {
        //______________________________________________________________________
        //
        //
        //! info for a sparse array
        //
        //______________________________________________________________________
        class array_info
        {
        public:
            virtual ~array_info() throw();                 //!< cleanup

        protected:
            explicit array_info(const size_t n);           //!< initialize virtual size
            size_t   size_;                                //!< virtual size
            void     check_index(const size_t n)    const; //!< throw exception on bad index
            void     insert_failure(const size_t i) const; //!< throw exception on insert failure (unexpected)
            void     create_failure(const size_t i) const; //!< throw exception on create failure (multiple index)
        private:
            Y_DISABLE_COPY_AND_ASSIGN(array_info);
        };
    }

    //__________________________________________________________________________
    //
    //
    //! sparse array implementation
    //
    //__________________________________________________________________________
    template <
    typename T,
    typename ALLOCATOR  = memory::global
    >
    class sparse_array : public sparse::array_info, public const_field<T>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);                                               //!< aliases
        typedef sparse::dok<size_t,T,key_dumper,ALLOCATOR> dok_type;       //!< alias
        typedef typename dok_type::iterator                iterator;       //!< alias
        typedef typename dok_type::const_iterator          const_iterator; //!< alias
        typedef typename dok_type::item_type               item_type;      //!< alias
        typedef typename dok_type::item_ptr                item_ptr;       //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        //! destructor
        inline virtual ~sparse_array() throw(){}


        //! initialize with a virtual size
        inline explicit sparse_array(const size_t n=0) :
        sparse::array_info(n), const_field<T>(), items(), core(items)
        {
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! set virtual size
        void set_size( const size_t n)
        {
            if(n>=size_)
            {
                // ok
            }
            else
            {
                // remove keys greater than n
                const is_bad_key is_greater_than_virtual_size = { n };
                items.remove_key_if(is_greater_than_virtual_size);
            }
            size_ = n;
        }

        //! virtual size
        inline size_t size() const throw() { return size_; }

        //! sort keys by increasing ordere
        inline void update()
        {
            items.sort_keys( comparison::increasing<size_t> );
        }

        //! direct creation, no multiple indices
        type & operator()( const size_t i, param_type args )
        {
            check_index(i);
            typename dok_type::item_ptr p = new typename dok_type::item_type(i,args);
            if(!items.insert(p)) insert_failure(i);
            return p->value;
        }

        //! look for existence
        type * operator()(const size_t i) throw()
        {
            typename dok_type::item_ptr *ppI = items.search(i);
            return (ppI ? & (***ppI) : NULL );
        }

        //! look for existence, const
        const_type * operator()(const size_t i) const throw()
        {
            const typename dok_type::item_ptr *ppI = items.search(i);
            return (ppI ? & (***ppI) : NULL );
        }


        //! access or on the fly 'zero' creation
        type & operator[]( const size_t i)
        {
            check_index(i);
            typename dok_type::item_ptr *ppI = items.search(i);
            if(ppI)
            {
                return ***ppI;
            }
            else
            {
                typename dok_type::item_ptr p = new typename dok_type::item_type(i,this->value);
                if(!items.insert(p)) insert_failure(i);
                return p->value;
            }
        }

        //! const access
        const_type & operator[](const size_t i) const throw()
        {
            check_index(i);
            const typename dok_type::item_ptr *ppI = items.search(i);
            if(ppI)
            {
                return ***ppI;
            }
            else
            {
                return this->value;
            }
        }

        //! display
        inline friend std::ostream & operator<<( std::ostream &os, const sparse_array &arr )
        {
            os << '[';
            const size_t n = arr.size();
            for(size_t i=1;i<=n;++i)
            {
                os << arr[i];
                if(i<n) os << ' ';
            }
            os << ']' << '\'';
            return os;
        }

        inline iterator       begin() throw() { return items.begin(); } //!< forward iterator begin
        inline iterator       end()   throw() { return items.end();   } //!< forward iterator end

        inline const_iterator begin() const throw() { return items.begin(); } //!< forward iterator begin, const
        inline const_iterator end()   const throw() { return items.end();   } //!< forward iterator end, const

        //! no throw swap
        inline void swap_with(sparse_array &other) throw()
        {
            if( this != &other)
            {
                cswap(size_,other.size_);
                items.swap_table_with(other.items);
            }
        }

        //! set to '0'
        inline void ldz() throw() { items.free(); }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(sparse_array);
        dok_type    items; //!< data handling

        //! wrapper to adjust size
        struct is_bad_key
        {
            size_t n;
            inline bool operator()(const size_t k) const throw() { return (k>n); }
        };
        
    public:
        container & core; //!< interface for internal memory management
    };

}

#endif
