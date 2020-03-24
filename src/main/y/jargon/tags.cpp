
#include "y/jargon/tags.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Tags:: ~Tags() throw()
        {
        }
        
        Tags:: Tags() : suffix_tree<Tag>()
        {
        }
        
        string *Tags:: query(const string &s)
        {
            const Tag *pTag = search_by(s);
            if(pTag)
            {
                return & aliasing::_( **pTag );
            }
            else
            {
                string   *ans = new string(s);
                const Tag tag = ans;
                if(!insert_by(s,tag)) throw exception("unexpected multiple tags '%s'",*s);
                return ans;
            }
        }
        
        string  * Tags:: query( const char *s )
        {
            const Tag *pTag = search_by(s);
            if(pTag)
            {
                return & aliasing::_( **pTag );
            }
            else
            {
                string   *ans = new string(s);
                const Tag tag = ans;
                if(!insert_by(*ans,tag)) throw exception("unexpected multiple tags '%s'",**ans);
                return ans;
            }
        }


    }
    
}

