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
                aliasing::_(ch->tag)     = context.tag;
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
        
        void Char::Pool:: store(Char *ch) throw()
        {
            assert(ch);
            (void) push_back(ch);
        }
        
        void  Char::Pool:: store( List &l  ) throw()
        {
            merge_back(l);
        }
        
        Char * Char::Pool:: query() throw() { assert(size>0); return pop_back(); }

    }
    
}

#include "y/sort/merge.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        void Char::Pool:: optimize()
        {
            merging<Char>::sort_by_addr(*this);
        }
        
        void Char::Pool:: maxChars(const size_t max_chars)
        {
            optimize();
            while(size>max_chars)
            {
                delete pop_back();
            }
        }
        
        void Char::Pool:: maxBytes(const size_t max_bytes)
        {
            maxChars(max_bytes/sizeof(Char));
        }

    }
    
}

