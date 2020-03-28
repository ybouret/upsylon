
#include "y/jargon/source.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Source:: ~Source() throw()
        {
        }
        
        
        Source:: Source(const Cache &cache, Module *module) throw() :
        handle(module),
        iobuff(cache),
        history()
        {
            
        }

        void Source:: load(Module *module)
        {
            assert(module);
            const Module::Handle h = module;
            history.push(handle);
            handle = h;
        }
        
        size_t Source:: depth() const throw()
        {
            return history.size();
        }

        void Source:: newLine() throw()
        {
            handle->newLine();
        }
        
        Char * Source:: tryGet()
        {
        TRY_GET_CHAR:
            Char *ch = handle->getChar();
            if( (NULL==ch) && history.size() > 0)
            {
                handle = history.peek();
                history.pop();
                goto TRY_GET_CHAR;
            }
            return ch;
        }

        Char * Source:: get()
        {
            return (iobuff.size>0) ? iobuff.pop_front() : tryGet();
        }
        
        bool Source:: isAlive()
        {
            if(iobuff.size>0)
            {
                return true;
            }
            else
            {
                Char *ch = tryGet();
                if(ch)
                {
                    iobuff.push_back(ch);
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
        
        bool Source:: isEmpty()
        {
            return !isAlive();
        }
        

        const Char::List  & Source:: IO() const throw()
        {
            return iobuff;
        }
        
        Cache & Source:: cache() const throw()
        {
            return aliasing::_(iobuff.cache);
        }
        
        size_t  Source:: prefetch(size_t n)
        {
            size_t count = 0;
            while(n-- > 0 )
            {
                Char *ch = tryGet();
                if(!ch) break;
                iobuff.push_back(ch);
                ++count;
            }
            return count;
        }


        bool Source:: query(char &C)
        {
            Char *ch = get();
            if(ch)
            {
                C = char(ch->code);
                iobuff.cache->store(ch);
                return true;
            }
            else
            {
                return false;
            }
        }
        
        void Source:: store(char C)
        {
            Context ctx(*handle);
            --aliasing::_(ctx.column);
            iobuff.push_front( Char::Make(iobuff.cache, ctx, C));
        }
        
        void  Source:: unget(Char *ch) throw()
        {
            assert(ch);
            iobuff.push_front(ch);
        }
        
        void  Source:: unget(Char::List &l) throw()
        {
            while(l.size)
            {
                unget( l.pop_back() );
            }
        }
        
        void Source:: uncpy(const Char::List &l)
        {
            Cache &cache = iobuff.cache;
            size_t done = 0;
            try
            {
                for(const Char *ch=l.tail;ch;ch=ch->prev)
                {
                    unget( Char::Copy(cache,*ch) );
                    ++done;
                }
            }
            catch(...)
            {
                iobuff.skip(done);
                throw;
            }
        }
        
        void  Source:: skip(const size_t n) throw()
        {
            assert(n<=iobuff.size);
            iobuff.skip(n);
        }

        
    }
    
}

