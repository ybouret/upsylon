//! \file
#ifndef Y_IPSO_FIELD_INCLUDED
#define Y_IPSO_FIELD_INCLUDED 1

#include "y/ipso/layout.hpp"
#include "y/string.hpp"

namespace upsylon
{
    namespace ipso
    {
        class ghost; //!< forward declaration

        //! base class for fields
        class field_info : public counted_object
        {
        public:
            const string name;        //!< identifier
            const size_t item_size;   //!< sizeof one item
            const size_t bytes;       //!< local linear bytes

            virtual ~field_info() throw(); //!< destructor

            virtual void        *address() throw()       = 0; //!< address of first item
            virtual const void  *address() const throw() = 0; //!< address of first item
            virtual  size_t      size() const throw()    = 0; //!< held items

            const string & key() const throw(); //!< for fields

        protected:
            explicit field_info(const string &id, const size_t sz); //!< setup
            explicit field_info(const char   *id, const size_t sz); //!< setup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(field_info);
        };

        //! wrapper for field constructor
#define Y_IPSO_FIELD_CTOR() field_info(id,sizeof(T)), entry(0), allocated(0), workspace(0)

        //! field base class
        template <typename T>
        class field : public field_info
        {
        public:
            Y_DECL_ARGS(T,type); //!< aliases
            type        *entry;  //!< entry of the linear part

            //! destructor
            inline virtual ~field() throw() {}

            //! for database if needed
            inline const string & key() const throw() { return name; }

            //! private memory
            inline size_t reserved() const throw() { return allocated; }

            //! field_info : address
            inline virtual void *address() throw() { return entry; }

            //! field_info : address, const
            inline virtual const void *address() const throw() { return entry; }

            //! load same items
            inline void ld( param_type args )
            {
                assert(entry);
                const size_t n = size();
                for(size_t i=0;i<n;++i) entry[i] = args;
            }

            //! debug function to check indices
            inline bool is_acceptable( const coord1D idx ) const throw()
            {
                return (idx>=0) && (idx<=static_cast<ptrdiff_t>(size()));
            }
            
            
        protected:
            //! setup
            inline explicit field(const string &id) : Y_IPSO_FIELD_CTOR() {}
            
            //! setup
            inline explicit field(const char   *id) : Y_IPSO_FIELD_CTOR() {}

            size_t allocated; //!< if owns local memory
            void  *workspace; //!< the local data

            //! allocate required memory
            inline void allocate()
            {
                assert(allocated);
                assert(!workspace);
                workspace = memory::global::instance().acquire(allocated);
            }

            //! try to build items if field is allocated
            inline void   __make(const size_t items)
            {
                assert(allocated);
                assert(items>0);
                assert(entry);
                mutable_type *p = (mutable_type *)entry;
                size_t count = 0;
                try
                {
                    while(count<items)
                    {
                        new (p+count) type();
                        ++count;
                    }
                }
                catch(...)
                {
                    __release(count);
                    throw;
                }
            }

            //! free count built items
            void   __free(size_t count) throw()
            {
                assert(entry);
                assert(allocated);
                mutable_type *p = (mutable_type *)entry;
                while(count-->0) destruct( &p[count] );
            }

            //! release count and memory
            void __release(const size_t count) throw()
            {
                assert(allocated);
                assert(workspace);
                __free(count);
                memory::global::instance().release(workspace,allocated);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(field);
        };
    }
}

#endif

