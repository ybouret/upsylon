
#include "y/graphic/memory.hpp"
#include "y/memory/global.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace Graphic {

        static inline size_t *create_shared()
        {
            size_t *nref = object::acquire1<size_t>();
            *nref = 1;
            return nref;
        }

        static inline void delete_shared( size_t * & nref ) throw()
        {
            assert( nref );
            assert( 0 == *nref );
            object::release1(nref);
        }


        Memory:: ~Memory() throw()
        {
            assert(nref);
            assert(*nref>0);
            if( --(*nref) <= 0 )
            {
                switch(kind)
                {
                    case Global: memory::global::location().release(data,size); break;
                    case Static:
                        data   = 0;
                        size   = 0;
                        break;
                }
                delete_shared(nref);
            }
        }

        Memory:: Memory( const size_t required, const Mode access ) :
        kind(Global),
        mode(access),
        nref( create_shared() ),
        size( required ),
        data(0),
        addr(0)
        {
            try
            {
                data  = memory::global::instance().acquire(size);
                addr  = data;
            }
            catch(...)
            {
                -- (*nref) ;
                delete_shared( nref );
                throw;
            }
        }


        Memory:: Memory(void *buffer, const size_t length) :
        kind(Static),
        mode(RW),
        nref( create_shared() ),
        size(length),
        data(buffer),
        addr(data)
        {

        }

        Memory:: Memory(const void *buffer, const size_t length) :
        kind(Static),
        mode(RO),
        nref( create_shared() ),
        size(length),
        data( (void*)buffer ),
        addr(data)
        {

        }


        Memory:: Memory(const Memory &other ) throw() :
        kind(other.kind),
        mode(other.mode),
        nref(other.nref),
        size(other.size),
        data(other.data),
        addr(other.addr)
        {
            assert(nref);
            ++(*nref);
        }

        void Memory:: displayMemoryInfo() const
        {
            switch(kind)
            {
                case Global: std::cerr << "Global"; break;
                case Static: std::cerr << "Static"; break;
            }
            std::cerr << ' ';
            switch(mode)
            {
                case RO: std::cerr << "RO"; break;
                case RW: std::cerr << "RW"; break;
            }
            std::cerr << "@ [" << (void*)data << "]\t+" << size << " : #ref=" << *nref << std::endl;
        }



    }

}

