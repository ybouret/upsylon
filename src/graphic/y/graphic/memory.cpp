
#include "y/graphic/memory.hpp"
#include "y/type/aliasing.hpp"
#include "y/exception.hpp"
#include <iomanip>

namespace upsylon {

    namespace Graphic {

        Memory:: Memory( Metrics &metrics ) :
        kind(Global),
        mode(ReadWrite),
        nref(0),
        entry(0),
        count(0)
        {
            static const char fn[] = "Graphic::Memory: ";


            // tune memory flags
            {
                const unsigned memoryFlags = (metrics.flags&Metrics::MemoryMask);
                if(0==memoryFlags) throw exception("%sinvalid memory flags", fn );

                if( 0 != (memoryFlags&Metrics::GlobalMemory) )
                {
                    aliasing::_(kind) = Global;
                }
                else if( 0 != (memoryFlags&Metrics::StaticMemory) )
                {
                    aliasing::_(kind) = Static;
                }
                else
                {
                    throw exception("%sunexpected memory flags", fn);
                }
            }


            // tune access flags
            {
                const unsigned accessFlags = (metrics.flags&Metrics::AccessMask);
                if(0==accessFlags) throw exception("%sinvalid access flags", fn );
                if( 0 != (accessFlags&Metrics::ReadOnly) )
                {
                    aliasing::_(mode) = ReadOnly;
                }
                else if( 0 != (accessFlags&Metrics::ReadWrite) )
                {
                    aliasing::_(mode) = ReadWrite;
                }
                else
                {
                    throw exception("%sunexpected access flags", fn);
                }
            }



            // take ownership
            entry = (void*)(metrics.entry); metrics.entry = 0;
            count = metrics.count;          metrics.count = 0;
            try
            {
                nref = object::acquire1<size_t>();
            }
            catch(...)
            {
                release();
                throw;
            }
            ++ (*nref);

        }

        Memory:: Memory(const Memory &other) throw()  :
        kind(other.kind),
        mode(other.mode),
        nref(other.nref),
        entry(other.entry),
        count(other.count)
        {
            assert(nref);
            ++(*nref);
        }


        void Memory:: release() throw()
        {
            switch(kind)
            {
                case Static: break;
                case Global: Metrics::Release(entry,count);
                    break;
            }
        }

        void Memory:: displayMemory() const
        {
            switch(kind)
            {
                case Static: std::cerr << "Static"; break;
                case Global: std::cerr << "Global"; break;
            }

            switch(mode)
            {
                case ReadWrite: std::cerr << ".RW"; break;
                case ReadOnly:  std::cerr << ".RO"; break;
            }

            std::cerr << "@" << std::setw(16) << entry << "\t+" << count << "\t #ref=" << (*nref) << std::endl;
        }



        Memory:: ~Memory() throw()
        {
            assert(nref);
            assert( *nref > 0 );
            if( -- (*nref) <= 0 )
            {
                object::release1(nref); assert(0==nref);
                release();
            }
        }

    }

}

