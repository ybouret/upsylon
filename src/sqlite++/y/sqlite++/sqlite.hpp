//! \file
#ifndef Y_SQLITE_INCLUDED
#define Y_SQLITE_INCLUDED 1

#include "y/string.hpp"
#include "y/ptr/counted.hpp"

namespace upsylon
{
    namespace SQLite
    {

        enum OpenMode
        {
            OpenReadOnly,
            OpenReadWrite,
            OpenReadWriteOrCreate
        };

        class DataBase : public counted_object
        {
        public:
            virtual ~DataBase() throw();
            explicit DataBase( const string &filename, const OpenMode how = OpenReadWriteOrCreate );
            explicit DataBase( const char   *filename, const OpenMode how = OpenReadWriteOrCreate );
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(DataBase);
            void *impl;
        };

    }
}


#endif

