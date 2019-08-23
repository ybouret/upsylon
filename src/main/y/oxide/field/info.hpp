
//! \file

#ifndef Y_OXIDE_FIELD_INFO_INCLUDED
#define Y_OXIDE_FIELD_INFO_INCLUDED 1

#include "y/oxide/types.hpp"
#include "y/string.hpp"

namespace upsylon
{
    namespace Oxide
    {

        class FieldInfo : public counted_object
        {
        public:
            const string name;

            virtual ~FieldInfo() throw();
            
        protected:
            explicit FieldInfo( const string & );
            explicit FieldInfo( const char   * );

        private:
            Y_DISABLE_COPY_AND_ASSIGN(FieldInfo);
        };

        template <typename T>
        class Field : public FieldInfo
        {
        public:
            inline virtual ~Field() throw() {}

        protected:
            inline explicit Field(const string &_) : FieldInfo(_) {}
            inline explicit Field(const char *  _) : FieldInfo(_) {}
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Field);
        };


    }
}

#endif

