
#include "y/jargon/token.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Token:: ~Token() throw()
        {
            erase();
        }
        
        Token:: Token(const Cache &shared) throw() :
        Char::List(),
        cache(shared)
        {
        }
        
        Token:: Token( const Token &other ) :
        Char::List(),
        cache( other.cache )
        {
            for( const Char *ch=other.head; ch; ch=ch->next)
            {
                push_back( Char::Copy(cache,*ch) );
            }
        }
        
        
        void Token:: erase() throw()
        {
            cache->store(*this);
        }
        
        
        
        void Token:: append(const Context &context,
                            const uint8_t  content)
        {
            push_back( Char::Make(cache,context,content) );
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
                cache->store(pop_front());
            }
        }
        
        void Token:: trim(size_t n) throw()
        {
            assert(n<=size);

            while(n-- >0)
            {
                cache->store(pop_back());
            }
        }

        
    }
}

