
#include "y/jargon/tags.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Jargon {
       
        Tags :: ~Tags() throw()
        {
        }
        
        Tags:: Tags() : singleton<Tags>(), TagsType()
        {
        }
        
        static string * __check(Tags &tags, string *ans)
        {
            const Tag tag = ans;
            if(!tags.insert_by(*ans,tag)) throw exception("Jargon::Tags(unexpected multiple '%s'",**ans);
            return ans;
        }
        
        string * Tags:: operator()(const Tag &tag)
        {
            (void) insert_by(*tag,tag);
            assert(has(*tag));
            return (string *)(tag.content());
        }
        
        string * Tags:: operator()(const string &s)
        {
            const Tag *pTag = search_by(s);
            if(pTag)
            {
                const string *ans = & **pTag;
                return (string *)ans;
            }
            else
            {
                return __check(*this, new string(s) );
            }
        }
        
        
        
        string * Tags:: operator()(const char *s)
        {
            const size_t n    = length_of(s);
            const Tag   *pTag = search_by(s,n);
            if(pTag)
            {
                const string    *ans = & **pTag;
                return (string *)ans;
            }
            else
            {
                return __check(*this,new string(s,n));
            }
        }
        
        string * Tags:: operator()(const char C)
        {
            const Tag   *pTag = search_by(&C,1);
            if(pTag)
            {
                const string    *ans = & **pTag;
                return (string *)ans;
            }
            else
            {
                return __check(*this,new string(C));
            }
        }
        
        void Tags:: display() const
        {
            std::cerr << "<Jargon::Tags entries=" << entries() << ">" << std::endl;
            for(const_iterator it=begin(); it != end(); ++it)
            {
                std::cerr << "\t<" << *it << ">" << std::endl;
            }
            std::cerr << "<Jargon::Tags/>" << std::endl;
        }
        
        void Tags::Display()
        {
            static const Tags & tags = instance();
            tags.display();
        }

        void Tags:: Release() throw()
        {
            if( exists() )
            {
                static Tags & tags = location();
                tags.release_all();
            }
        }
        
    }
    
}

