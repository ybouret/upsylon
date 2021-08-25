
#include "y/alchemy/eqs/db.hpp"

namespace upsylon
{

    Y_SINGLETON_IMPL(Alchemy::EqDB);
    
    namespace Alchemy
    {



        EqDB:: ~EqDB() throw()
        {}

        EqDB:: EqDB() : db()
        {
        }

        EqDB::const_type & EqDB:: bulk() const throw() { return db; }


    }

}
