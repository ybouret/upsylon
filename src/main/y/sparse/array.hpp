#ifndef Y_SPARSE_ARRAY_INCLUDED
#define Y_SPARSE_ARRAY_INCLUDED 1

#include "y/sparse/dok.hpp"
#include "y/comparison.hpp"

namespace upsylon
{

    namespace sparse
    {
        class array_info
        {
        public:
            virtual ~array_info() throw();

        protected:
            explicit array_info(const size_t n, const size_t item_size);
            size_t       size_; //!< virtual size
            size_t       itsz_; //!< item size
            void        *item_; //!< 'zero'

            void   check_index(const size_t n) const;
            void   insert_failure(const size_t i) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(array_info);
        };
    }


    template <
    typename T,
    typename ALLOCATOR  = memory::global
    >
    class sparse_array : public sparse::array_info
    {
    public:
        Y_DECL_ARGS(T,type);

        typedef sparse::dok<size_t,T,key_dumper,ALLOCATOR> dok_type;
        typedef typename dok_type::iterator       iterator;
        typedef typename dok_type::const_iterator const_iterator;

        inline virtual ~sparse_array() throw()
        {
            kill_();
        }

        const_type & zero() const throw()
        {
            return zero_;
        }

        inline explicit sparse_array() throw() :
        sparse::array_info(0,sizeof(T)),
        items(),
        zero_( live_() )
        {
        }

        inline explicit sparse_array(const size_t n) :
        sparse::array_info(n,sizeof(T)),
        items(),
        zero_( live_() )
        {
        }

        //! set virtual size
        void set_size( const size_t n)
        {
            if(n>=size_)
            {
                
            }
            else
            {
                // remove keys greater than n
                const is_bad_key is_greater_than_virtual_size = { n };
                items.remove_key_if(is_greater_than_virtual_size);
            }
            size_ = n;
        }

        inline size_t size() const throw() { return size_; }

        inline void   update()
        {
            items.sort_keys( comparison::increasing<size_t> );
        }

        //! access or on the fly 'zero' creation
        type & operator[]( const size_t i)
        {
            check_index(i);
            typename dok_type::item_ptr *ppI = items.search(i);
            if(ppI)
            {
                return (**ppI).value;
            }
            else
            {
                typename dok_type::item_ptr p = new typename dok_type::item_type(i,zero_);
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
                return (**ppI).value;
            }
            else
            {
                return zero_;
            }
        }

        inline friend std::ostream & operator<<( std::ostream &os, const sparse_array &arr )
        {
            os << '[';
            const size_t n = arr.size();
            for(size_t i=1;i<=n;++i)
            {
                os << arr[i];
                if(i<n) os << ' ';
            }
            os << ']';
            return os;
        }

        inline iterator       begin() throw() { return items.begin(); }
        inline iterator       end()   throw() { return items.end();   }

        inline const_iterator begin() const throw() { return items.begin(); }
        inline const_iterator end()   const throw() { return items.end();   }

        container &       core()       throw() { return items; }
        const container & core() const throw() { return items; }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(sparse_array);
        dok_type    items; //!< data handling
        const_type &zero_; //!< one shared zero

        inline const_type &live_()         { return * ( new (item_) type() );                }
        inline void        kill_() throw() { destruct( static_cast<mutable_type *>(item_) ); }


        struct is_bad_key
        {
            size_t n;
            inline bool operator()(const size_t k) const throw() { return (k>n); }
        };
    };

}

#endif
