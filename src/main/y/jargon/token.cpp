
#include "y/jargon/token.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Token:: ~Token() throw()
        {
        }
        
        Token:: Token() throw() : Char::List()
        {
        }
        
        Token:: Token( const Token &other ) :
        Char::List(other),
        Serializable()
        {
           
        }
        
        
        
        void Token:: append(const Context &context,
                            const uint8_t  content)
        {
            push_back( new Char(context,content) );
        }
        
        void Token:: append(const Context &context, const string &content)
        {
            const size_t n = content.size();
            for( size_t i=0;i<n;++i)
            {
                push_back( new Char(context,content[i]) );
            }
        }
        
        
        std::ostream & operator<<( std::ostream &os, const Token &t)
        {
            for( const Char *ch=t.head;ch;ch=ch->next)
            {
                os << *ch;
            }
            return os;
        }
        
        void Token:: skip(size_t n) throw()
        {
            assert(n<=size);
            while(n-- >0)
            {
                delete pop_front();
            }
        }
        
        void Token:: trim(size_t n) throw()
        {
            assert(n<=size);
            
            while(n-- >0)
            {
                delete pop_back();
            }
        }
        
        
    }
}

#include "y/ios/ostream.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        const char *Token::className() const throw()
        {
            return "Token";
        }
        
        size_t Token:: serialize( ios::ostream &fp ) const
        {
            const size_t written = fp.write_upack(size) + size;
            for(const Char *ch=head;ch;ch=ch->next)
            {
                fp.write(ch->code);
            }
            return written;
        }
        
    }
    
}

#include "y/exception.hpp"
#include "y/code/utils.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        void Token:: cat(exception &excp) const throw()
        {
            
            for(const Char *ch=head;ch;ch=ch->next)
            {
                excp.cat("%s",cchars::encoded[ch->code]);
            }
           
        }

    }
}
