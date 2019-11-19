
#include "y/graphic/memory.hpp"
#include "y/memory/global.hpp"
#include "y/type/aliasing.hpp"
#include "y/exception.hpp"

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
        entry(0),
        count(0)
        {
            try
            {
                data   = memory::global::instance().acquire(size);
                entry  = data;
                aliasing::_(count) = size;
            }
            catch(...)
            {
                -- (*nref) ;
                delete_shared( nref );
                throw;
            }
        }

        void Memory:: checkStaticData()  
        {
            if(size>0&&0==data)
            {
                --(*nref);
                delete_shared(nref);
                throw exception("Graphic::Memory(NULL,%u)", unsigned(size));
            }
        }



        Memory:: Memory(void *buffer, const size_t length) :
        kind(Static),
        mode(RW),
        nref( create_shared() ),
        size(length),
        data(buffer),
        entry(data),
        count(size)
        {
            checkStaticData();
        }

        Memory:: Memory(const void *buffer, const size_t length) :
        kind(Static),
        mode(RO),
        nref( create_shared() ),
        size(length),
        data( (void*)buffer ),
        entry(data),
        count(size)
        {
            checkStaticData();
        }


        Memory:: Memory(const Memory &other ) throw() :
        kind(other.kind),
        mode(other.mode),
        nref(other.nref),
        size(other.size),
        data(other.data),
        entry(other.entry),
        count(other.count)
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
            std::cerr << "@ [" << (void*)data << "]\t+" << size << " \t: #ref=" << *nref << " : count=" << count << std::endl;
        }

        Memory:: Memory(const Memory &other, const size_t shift ) :
        kind(other.kind),
        mode(other.mode),
        nref(other.nref),
        size(other.size),
        data(other.data),
        entry(other.entry),
        count(other.count)
        {
            if(shift>count) throw exception("Graphic::Memory(shift>count)");
            entry               = aliasing::anonymous(entry,shift);
            aliasing::_(count) -= shift;
            ++(*nref);
        }



    }

}

