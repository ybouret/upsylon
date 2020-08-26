
#include "y/jive/tags.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/exception.hpp"

namespace upsylon {

    Y_SINGLETON_IMPL_WITH(memory::pooled::life_time-1,Jive::Tags);

    namespace Jive
    {

        Tags:: Tags() : singleton<Tags>(), TagsTree()
        {

        }

        Tags:: ~Tags() throw()
        {
        }

        const  string * Tags:: get(const char *buffer, const size_t buflen)
        {
            assert(!(NULL==buffer&&buflen>0));
            const Tag *tag = look_for(buffer,buflen);
            if( tag )
            {
                return & **tag;
            }
            else
            {
                const string *p = new string(buffer,buflen);
                const Tag     t(p);
                if(!insert_by(buffer,buflen,t))
                {
                    throw exception("Jive::Tags(unexpected failure for '%s')", **p);
                }
                return p;
            }
        }

        const string * Tags:: get(const char *buffer)
        {
            return get( buffer, length_of(buffer) );
        }

        const string * Tags:: get(const string &s)
        {
            return get(*s,s.size());
        }

        const string * Tags:: get(const Tag &tag)
        {
            return get( **tag, tag->size() );
        }

        const string * Tags:: get(const string *s)
        {
            const Tag t(s);
            return get(t);
        }

        const string * Tags:: get( const char C )
        {
            return get( &C, 1 );
        }

        void Tags:: Display()
        {



            
        }


    }

}


