//! \file
#ifndef Y_SQLITE_INCLUDED
#define Y_SQLITE_INCLUDED 1

#include "y/string.hpp"
#include "y/ptr/counted.hpp"
#include "y/ptr/arc.hpp"

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

            void *       operator*() throw()       { return impl; }
            const void * operator*() const throw() { return impl; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DataBase);
            void *impl;
        };

        typedef arc_ptr<DataBase> DB;

        class Statement
        {
        public:
            explicit Statement( const DB &target, const string &sql );
            explicit Statement( const DB &target, const char   *sql );
            virtual ~Statement() throw();

        private:
            DB    db;
            void *impl;
        };

    }
}


#endif

