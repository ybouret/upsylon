
#include "y/jargon/source.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Source:: ~Source() throw()
        {
        }
        
        
        Source:: Source(Module *module) throw() :
        handle(module),
        io(),
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
                delete ch;
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
            aliasing::_(io).push_front( new Char(ctx,C) );
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
            size_t done  = 0;
            try
            {
                for(const Char *ch=l.tail;ch;ch=ch->prev)
                {
                    unget( new Char(*ch) );
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

        

    }
    
}

