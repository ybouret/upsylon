#include "y/memory/groove.hpp"

#include "y/object.hpp"
#include "y/memory/pooled.hpp"
#include "y/memory/global.hpp"
#include "y/exceptions.hpp"

#include <cstring>
#include <cerrno>

namespace upsylon {

    namespace memory {

        groove:: groove() throw() :
        model(storage::unused),
        entry(0),
        bytes(0),
        owned(0),
        count(0),
        width(0),
        clear(0),
        label(0)
        {
        }

        groove:: ~groove() throw()
        {
            release();
        }

        const std::type_info & groove:: type_id() const throw()
        {
            return label ? *label : typeid(null_type);
        }

        void groove:: release() throw()
        {
            if(entry)
            {
                assert(bytes);
                assert(owned>=bytes);

                // C++ clear
                free();
                assert(0==count);
                assert(0==width);
                assert(0==clear);
                assert(0==label);

                // release memory, set entry=0 and owned=0
                switch(model)
                {
                    case storage::unused: break; //!< shouldn't happen

                    case storage::shared:
                        object:: operator delete((void*)entry,owned);
                        aliasing::_(owned) = 0;
                        entry              = 0;
                        break;

                    case storage::pooled: {
                        static memory::allocator & mgr = memory::pooled::location();
                        mgr.release( *(void **) &entry, aliasing::_(owned) );
                        assert(0==entry);
                        assert(0==owned);
                    } break;

                    case storage::global: {
                        static memory::allocator & mgr = memory::global::location();
                        mgr.release( *(void **) &entry, aliasing::_(owned) );
                        assert(0==entry);
                        assert(0==owned);
                    } break;

                }

                assert(0==aliasing::_(owned));
                assert(0==entry);

                aliasing::_(model) = storage::unused;
                aliasing::_(bytes) = 0;


            }
            else
            {
                assert(storage::unused==model);
                assert(0==bytes);
                assert(0==owned);
                assert(0==count);
                assert(0==width);
                assert(0==clear);
                assert(0==label);
            }
        }

        void groove:: free() throw()
        {

            if(count)
            {
                assert(model!=storage::unused);
                assert(entry);
                assert(bytes);
                assert(owned>=bytes);

                assert(width>=1);
                assert(clear!=NULL);
                assert(label!=NULL);
                assert(width*count>=bytes);

                char *p = ((char*)entry) + (width*count);
                for(size_t i=count;i>0;--i)
                {
                    p -= width;
                    clear(p);
                }

                aliasing::_(count) = 0;
                aliasing::_(width) = 0;
                aliasing::_(clear) = 0;
                aliasing::_(label) = 0;
            }
            else
            {
                assert(0==count);
                assert(0==width);
                assert(0==clear);
                assert(0==label);
            }
            memset((void*)entry,0,owned);
        }

        static const char cls[] = "memory::groove";

        void groove:: acquire(const storage::model which,const size_t n)
        {
            if(n==bytes&&which==model)
            {
                free();
            }
            else
            {
                release();
                if(n>0)
                {
                    switch(which)
                    {
                        case storage::unused:
                            throw libc::exception(EINVAL,"%s.acquire(%lu>0,%s)",cls,(unsigned long)n, storage::text(which));

                        case storage::shared:
                            entry              = object:: operator new(n);
                            aliasing::_(owned) = n;
                            break;

                        case storage::pooled: {
                            static memory::allocator & mgr = memory::pooled::instance();
                            entry = mgr.acquire( aliasing::_(owned) = n );
                        } break;

                        case storage::global: {
                            static memory::allocator & mgr = memory::global::instance();
                            entry = mgr.acquire( aliasing::_(owned) = n );
                        } break;
                    }

                    assert(entry!=NULL);
                    assert(owned>0);
                    aliasing::_(bytes) = n;
                    aliasing::_(model) = which;
                }
            }
        }

        bool groove:: is_cplusplus() const throw()
        {
            if(label)
            {
                assert(clear);
                assert(width);
                assert(count);
                return true;
            }
            else
            {
                assert(!clear);
                assert(!width);
                assert(!count);
                return false;
            }
        }

        bool groove:: is_zeroed() const throw()
        {
            const unsigned char *p = static_cast<const unsigned char*>(entry);
            for(size_t i=0;i<bytes;++i)
            {
                if(p[i]>0) return false;
            }
            return true;
        }

        const char * groove:: model_text()   const throw()
        {
            return storage::text(model);
        }

    }
}

#include "y/type/spec.hpp"

namespace upsylon {

    namespace memory {

        std::ostream & operator<<(std::ostream &os, const groove &g)
        {
            os << '(';
            os << g.bytes << '/' << g.owned << ':';
            os << '<' << type_name_for( g.type_id() ) << '>';
            if(g.count>0)
            {
                os << 'x' << g.count;
            }
            os << ')';
            return os;
        }

        bool groove:: is_built_from( const std::type_info &tid ) const throw()
        {
            if(!label) return false;
            assert(entry);
            assert(bytes);
            return *label == tid;
        }
    }
}
