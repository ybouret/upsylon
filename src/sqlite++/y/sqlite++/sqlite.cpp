#include "y/sqlite++/sqlite.hpp"
#include "y/exception.hpp"

#include "y/sqlite/sqlite3.h"

namespace upsylon
{
    namespace SQLite
    {
        DataBase:: ~DataBase() throw()
        {
            assert(impl);
            sqlite3  *db  = static_cast<sqlite3 *>(impl);
            const int err = sqlite3_close_v2(db);
            if( err != SQLITE_OK )
            {

            }
            impl=0;
        }

        static inline
        sqlite3 * OpenDataBase( const string &fn, const OpenMode how )
        {
            int flags = 0;
            switch(how)
            {
                case OpenReadOnly:
                    flags = SQLITE_OPEN_READONLY;
                    break;

                case OpenReadWrite:
                    flags = SQLITE_OPEN_READWRITE;
                    break;

                case OpenReadWriteOrCreate:
                    flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
                    break;
            }
            sqlite3  *db  = 0;
            const int err = sqlite3_open_v2(*fn, &db, flags, 0);
            if( err != SQLITE_OK )
            {
                throw exception("sqlite3_open_v2: [%s] for '%s'", sqlite3_errstr(err), *fn);
            }
            return db;
        }

        DataBase:: DataBase( const string &filename, const OpenMode how ) :
        impl( OpenDataBase(filename,how) )
        {

        }

        DataBase:: DataBase( const char *filename, const OpenMode how ) :
        impl(0 )
        {
            const string _(filename);
            impl = OpenDataBase(_,how);
        }


    }
}
