#include "y/sqlite++/sqlite.hpp"
#include "y/exception.hpp"

#include "y/sqlite/sqlite3.h"

namespace upsylon
{
    namespace SQLite
    {
        ////////////////////////////////////////////////////////////////////////
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


        ////////////////////////////////////////////////////////////////////////
        Statement:: ~Statement() throw()
        {
            assert(impl);
            const int err = sqlite3_finalize(static_cast<sqlite3_stmt*>(impl));
            if( err != SQLITE_OK )
            {

            }
            impl = 0;
        }

        static inline
        sqlite3_stmt *Prepare( DB &target, const string &sql )
        {
            sqlite3 *db = static_cast<sqlite3*>(**target);
            const char   *zsql = *sql;
            const int     nsql = sql.size();
            sqlite3_stmt *stmt = 0;
            const int err = sqlite3_prepare_v2(db, zsql, nsql, &stmt, NULL);
            if( err != SQLITE_OK )
            {
                throw exception("sqlite3_open_v2: [%s] for '%s'", sqlite3_errstr(err), zsql);
            }
            return stmt;
        }

        Statement:: Statement( const DB &target, const string &sql ) :
        db(target),
        impl( Prepare(db,sql) )
        {
        }

        Statement:: Statement( const DB &target, const char *sql ) :
        db(target),
        impl(0)
        {
            const string _(sql);
            impl = Prepare(db,_);
        }


    }
}
