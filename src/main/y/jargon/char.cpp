#include "y/jargon/char.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"

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
        
        Char * Char:: Create(const Context &context,
                             const uint8_t  content,
                             SharedPool    &pool)
        {
            if(pool->size>0)
            {
                Char *ch = pool->query();
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

      
        
        Char::Pool::  Pool() throw() : CountedObject(), PoolType() {}
        Char::Pool:: ~Pool() throw()  {}
        
        
        
        
    }
    
}

