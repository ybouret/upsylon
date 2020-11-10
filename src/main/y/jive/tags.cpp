
#include "y/jive/tags.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/exception.hpp"
#include "y/code/utils.hpp"
#include "y/type/utils.hpp"
#include "y/type/aliasing.hpp"
#include "y/suffix/node-to.hpp"

namespace upsylon {

    Y_SINGLETON_IMPL_WITH(object::life_time-1,Jive::Tags);

    namespace Jive
    {

        Tags:: Tags() : upsylon::singleton<Tags>(), TagsTree(), maxLength(0)
        {

        }

        Tags:: ~Tags() throw()
        {
        }

        const  string * Tags:: get(const char *buffer, const size_t buflen)
        {
            Y_LOCK(access);
            assert(!(NULL==buffer&&buflen>0));
            const Tag *tag = search_by(buffer,buflen);
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

        
        void Tags:: Display()
        {
            static const Tags  &tree = Tags::instance();
            Y_LOCK(tree.access);
            std::cerr << "<Jive::Tags count=\"" << tree.size() << "\">" << std::endl;

            const size_t len = tree.maxLength;
            for(const data_node *node=tree.head();node;node=node->next)
            {
                const Tag &tag = **node;
                std::cerr << '\t' << '<' << tag << '>' << ' ';
                for(size_t i=tag->size();i<len;++i) std::cerr << ' ';
                std::cerr << '(';
                const string path = suffix_node_::to_string( static_cast<const tree_node *>(node->hook) );
                std::cerr << path;
                std::cerr << ')' << std::endl;
            }

            std::cerr << "<Jive::Tags/>" << std::endl;
        }

        void Tags:: Release() throw()
        {
            static Tags  &tree = Tags::instance();
            Y_LOCK(tree.access);
            tree.release();
        }

    }

}


