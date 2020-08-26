
#include "y/jive/tags.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/exception.hpp"
#include "y/code/utils.hpp"
#include "y/type/utils.hpp"

namespace upsylon {

    Y_SINGLETON_IMPL_WITH(memory::pooled::life_time-1,Jive::Tags);

    namespace Jive
    {

        Tags:: Tags() : singleton<Tags>(), TagsTree(), maxLength(0)
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
                aliasing::_(maxLength) = max_of(maxLength,buflen);
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

        static inline
        bool DisplayCallback(const suffix_path &path,
                             const Tag         &tag)
        {
            const size_t len = Tags::location().maxLength;
            std::cerr << '\t' << '<' << tag << '>' << ' ';
            for(size_t i=tag->size();i<len;++i) std::cerr << ' ';
            std::cerr << '(';
            for(suffix_path::const_iterator i=path.begin();i!=path.end();++i)
            {
                std::cerr << cchars::visible[*i];
            }
            std::cerr << ')' << std::endl;
            return true;
        }

        void Tags:: Display()
        {
            static const TagsTree &tree = Tags::instance();
            std::cerr << "<Jive::Tags count=\"" << tree.entries() << "\">" << std::endl;
            tree.for_each(DisplayCallback);
            std::cerr << "<Jive::Tags/>" << std::endl;
        }


    }

}


