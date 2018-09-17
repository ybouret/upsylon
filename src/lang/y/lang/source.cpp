#include "y/lang/source.hpp"
#include "y/ptr/auto.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        Source:: ~Source() throw()
        {
        }

        void Source:: unget(Char *ch) throw()
        {
            iobuf.push_front(ch);
        }

        void Source:: unget( Token &t ) throw()
        {
            iobuf.merge_front(t);
        }

        void Source:: ungetCopy( const Token &t )
        {
            Token tmp(t);
            iobuf.merge_front(tmp);
        }


        static inline
        Module *check_module_addr(Module *m)
        {
            if(!m) throw exception("NULL module for Lang::Source");
            return m;
        }

        Source:: Source(Module *m)  :
        module(check_module_addr(m)),
        iobuf()
        {
        }


        Char *Source:: get()
        {
            return (iobuf.size) ? iobuf.pop_front() : module->get();
        }

        void Source:: prefetch(size_t n)
        {
            while(n-->0)
            {
                Char *ch = module->get();
                if(!ch) return;
                iobuf.push_back(ch);
            }
        }

        void Source:: skip()
        {
            assert(iobuf.size>0);
            delete iobuf.pop_front();
        }



        size_t Source:: loaded() const throw()
        {
            return iobuf.size;
        }

        void Source:: forward(size_t n) throw()
        {
            assert(n<=iobuf.size);
            while(n-->0)
            {
                skip();
            }
        }

        bool Source::active()
        {
            if(iobuf.size<=0)
            {
                Char *ch = module->get();
                if(!ch)
                {
                    return false;
                }
                else
                {
                    iobuf.push_front(ch);
                }
            }
            assert(iobuf.size>0);
            //C = iobuf.head->code;
            return true;
        }

        const Char * Source:: peek()
        {
            if( active() )
            {
                return iobuf.head;
            }
            else
            {
                return NULL;
            }
        }
        

        const Module * Source:: operator*() const throw()
        {
            assert(module.is_valid());
            return & *module;
        }


        size_t Source:: try_input(void *buffer, const size_t buflen )
        {
            assert(!(buffer==NULL&&buflen>0));
            size_t   count = 0;
            uint8_t *p     = static_cast<uint8_t*>(buffer);
            while(count<buflen)
            {
                Char *ch = get();
                if(!ch)
                {
                    break;
                }
                else
                {
                    *(p++) = ch->code;
                    ++count;
                    delete ch;
                }
            }
            return count;
        }

        void Source:: input(void *buffer,const size_t buflen,const char *field)
        {
            const size_t count = try_input(buffer,buflen);
            if(count<buflen)
            {
                const char *id = field ? field : "user field";
                throw exception("Source::input: missing #byte=%u for '%s",unsigned(buflen-count),id);
            }
        }

        string Source:: load_binary()
        {
            const size_t n = read<uint32_t>("string size/32-bits");
            string ans(n,as_capacity);
            assert(ans.capacity()>=n);
            input( *ans, n, "string content");
            ans.force(n);
            return ans;
        }


    }
}

