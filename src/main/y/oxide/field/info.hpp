
//! \file

#ifndef Y_OXIDE_FIELD_INFO_INCLUDED
#define Y_OXIDE_FIELD_INFO_INCLUDED 1

#include "y/oxide/layout.hpp"
#include "y/string.hpp"

namespace upsylon
{
    namespace Oxide
    {

        class FieldInfo : public counted_object
        {
        public:
            const string name;
            const size_t owned;

            virtual ~FieldInfo() throw();
            
        protected:
            explicit FieldInfo( const string & );
            explicit FieldInfo( const char   * );
            void    *privateData;
            size_t   privateSize;

            void acquirePrivate();
            void releasePrivate() throw();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(FieldInfo);
        };

        template <typename T>
        class Field : public FieldInfo
        {
        public:
            Y_DECL_ARGS(T,type);
            type        *entry; //!< linear data entry
            const size_t bytes; //!< expected inear bytes

            inline virtual ~Field() throw()
            {
                freeData( (void*) entry );
            }

        protected:
            inline explicit Field(const string &_, const LayoutInfo &l) : FieldInfo(_), entry(0), bytes(l.items*sizeof(T)) {}
            inline explicit Field(const char *  _, const LayoutInfo &l) : FieldInfo(_), entry(0), bytes(l.items*sizeof(T)) {}

            //! cleanup owned data
            void freeData(void *addr) throw()
            {

                mutable_type *p = static_cast<mutable_type *>(addr);
                size_t       &n = (size_t&)owned;
                while(n>0)
                {
                    destruct( &p[--n] );
                }
            }

            //! build data at address
            void makeData(void *addr, const LayoutInfo &l)
            {
                assert(!owned);
                mutable_type *p = static_cast<mutable_type *>(addr);
                const size_t  m = l.items;
                try
                {
                    size_t &n = (size_t&)owned;
                    while(n<m)
                    {
                        new (p+n) mutable_type();
                        ++n;
                    }
                }
                catch(...)
                {
                    freeData(addr);
                    throw;
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Field);
        };


    }
}

#endif

