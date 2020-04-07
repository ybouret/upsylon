
#include "y/jargon/source.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Source:: ~Source() throw()
        {
        }
        
        
        Source:: Source(Module *module) throw() :
        handle(module),
        io( handle->cache ),
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
            return (io.size>0) ? aliasing::_(io).pop_front() : tryGet();
        }
        
        bool Source:: isAlive()
        {
            if(io.size>0)
            {
                return true;
            }
            else
            {
                Char *ch = tryGet();
                if(ch)
                {
                    aliasing::_(io).push_back(ch);
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
        

      
        
       
        
        size_t  Source:: prefetch(size_t n)
        {
            size_t count = 0;
            while(n-- > 0 )
            {
                Char *ch = tryGet();
                if(!ch) break;
                aliasing::_(io).push_back(ch);
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
                io.cache->store(ch);
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
            aliasing::_(io).push_front( Char::Make(io.cache, ctx, C));
        }
        
        void  Source:: unget(Char *ch) throw()
        {
            assert(ch);
            aliasing::_(io).push_front(ch);
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
            Cache &cache = io.cache;
            size_t done  = 0;
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
                aliasing::_(io).skip(done);
                throw;
            }
        }
        
        void  Source:: skip(const size_t n) throw()
        {
            assert(n<=io.size);
            aliasing::_(io).skip(n);
        }

        void Source:: collectNext(Token &bad)
        {
            unget(bad);
            for(Char *ch=get();ch;ch=get())
            {
                const uint8_t code = ch->code;
                if(code>='0'&&code<='9') goto GROW;
                if(code=='_')            goto GROW;
                if(code>='a'&&code<='z') goto GROW;
                if(code>='A'&&code<='Z') goto GROW;
                
                io.cache->store(ch);
                continue;
                
            GROW:
                bad.push_back(ch);
                
            }
        }

    }
    
}

