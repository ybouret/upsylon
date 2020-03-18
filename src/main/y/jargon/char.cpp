#include "y/jargon/char.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"
#include "y/code/utils.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Char:: ~Char() throw()
        {
            _bzset(code);
        }
        
        Char:: Char(const Context &context,
                    const uint8_t  content) throw() :
        Object(),
        inode<Char>(),
        Context(context),
        code(content)
        {
        }
        
        Char * Char:: Make(Cache         &cache,
                           const Context &context,
                           const uint8_t  content)
        {
            if(cache->size>0)
            {
                Char *ch = cache->query();
                aliasing::_(ch->code)    = content;
                aliasing::_(ch->stamp)   = context.stamp;
                aliasing::_(ch->line)    = context.line;
                aliasing::_(ch->column)  = context.column;
                return ch;
            }
            else
            {
                return new Char(context,content);
            }
        }
        
        Char * Char:: Copy(Cache &cache, const Char &ch)
        {
            return Make(cache,ch,ch.code);
        }
        
        std::ostream & operator<<( std::ostream &os, const Char &ch)
        {
            os << visible_char[ch.code];
            return os;
        }
        
        
        
        
        Char::Pool::  Pool() throw() : CountedObject(), PoolType() {}
        Char::Pool:: ~Pool() throw()  {}
        
        
        
        
    }
    
}

