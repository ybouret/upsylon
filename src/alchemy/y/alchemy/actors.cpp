#include "y/alchemy/actors.hpp"
#include "y/exception.hpp"

#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"
#include "y/code/decimal-chars.hpp"

namespace upsylon
{
    namespace alchemy
    {

        const char actors:: clid[] = "alchemy::actors";

        actors:: actors() : db(), cwidth(0)
        {
        }

        actors:: ~actors() throw()
        {
        }
        
        void actors:: operator()(const species &sp, const unsigned long nu)
        {
            if(nu<=0) throw exception("%s(nul coefficient for '%s')", clid, *sp.name);
            const actor a(sp,nu);
            if( !db.insert(a) ) throw exception("%s(multiple '%s')", clid, *sp.name);
            aliasing::_(cwidth) = max_of(cwidth,decimal_chars_for(nu));
        }
        
        bool actors:: search(const string &id) const throw()
        {
            return NULL != db.search(id);
        }

        const actors::db_type & actors::operator*()  const throw()
        {
            return db;
        }

        const actors::db_type * actors::operator->()  const throw()
        {
            return &db;
        }



    }

}

