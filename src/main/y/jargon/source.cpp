
#include "y/jargon/source.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Source:: ~Source() throw()
        {
        }
        
        
        Source:: Source( Module *module ) throw() :
        handle(module),
        iobuf(handle->cache),
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
            return (iobuf.size>0) ? iobuf.pop_front() : tryGet();
        }
        
        void  Source:: prefetch(size_t n)
        {
            while(n-- > 0 )
            {
                Char *ch = tryGet();
                if(!ch) break;
                iobuf.push_back(ch);
            }
        }


        bool Source:: query(char &C)
        {
            Char *ch = get();
            if(ch)
            {
                C = char(ch->code);
                iobuf.cache->store(ch);
                return true;
            }
            else
            {
                return false;
            }
        }
        
        void Source:: store(char C)
        {
            Context ctx = *handle;
            --aliasing::_(ctx.column);
            iobuf.push_front( Char::Make(iobuf.cache, ctx, C));
        }
        
        void  Source:: unget(Char *ch) throw()
        {
            assert(ch);
            iobuf.push_front(ch);
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
            Char::Cache &cache = iobuf.cache;
            size_t       done = 0;
            try
            {
                for(const Char *ch=l.tail;ch;ch=ch->prev)
                {
                    unget( Char::Copy(cache,*ch) );
                }
            }
            catch(...)
            {
                iobuf.skip(done);
                throw;
            }
        }
        
        void  Source:: skip(const size_t n) throw()
        {
            assert(n<=iobuf.size);
            iobuf.skip(n);
        }

        
    }
    
}

