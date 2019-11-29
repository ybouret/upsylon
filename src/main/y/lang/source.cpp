#include "y/lang/source.hpp"
#include "y/ptr/auto.hpp"
#include "y/exception.hpp"
#include "y/code/utils.hpp"

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
            return (iobuf.size>0) ? iobuf.pop_front() : module->get();
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
            //std::cerr << "Still got char " << iobuf.head->code << std::endl;
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


        bool Source:: query( char &C )
        {
            Char *ch = get();
            if(ch)
            {
                C = ch->code;
                delete ch;
                return true;
            }
            else
            {
                return false;
            }
        }

        void Source:: store( char C )
        {
            //Char *ch = new Char(C,*module);
            throw exception("Lang::Source::store('%s') is forbidden", visible_char[uint8_t(C)]);
        }


#if 0
        string Source:: load_binary()
        {
            const size_t n = read<uint32_t>();
            string ans(n,as_capacity);
            assert(ans.capacity()>=n);
            input( *ans, n);
            ans.force(n);
            return ans;
        }
#endif
        

    }
}

