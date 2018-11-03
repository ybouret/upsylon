//! \file
#ifndef Y_IPSO_FIELD_INCLUDED
#define Y_IPSO_FIELD_INCLUDED 1

#include "y/ipso/layout.hpp"
#include "y/string.hpp"

namespace upsylon
{
    namespace ipso
    {

#define Y_IPSO_FIELD_CTOR() name(id), entry(0), bytes(0), allocated(0), workspace(0)
        //! field base class
        template <typename T>
        class field
        {
        public:
            Y_DECL_ARGS(T,type);
            const string name;   //!< indentifier
            type        *entry;  //!< entry of the linear part
            const size_t bytes;  //!< local bytes, based on items

            //! destructor
            inline virtual ~field() throw() {}

            //! for database if needed
            inline const string & key() const throw() { return name; }

        protected:
            inline explicit field(const string &id) : Y_IPSO_FIELD_CTOR() {}
            inline explicit field(const char   *id) : Y_IPSO_FIELD_CTOR() {}

            size_t allocated; //!< if owns local memory
            void  *workspace; //!< the local data

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

