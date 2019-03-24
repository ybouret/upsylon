
#include "y/ios/rc/io.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace ios
    {
        rc::io:: ~io() throw()
        {
        }

        rc::io:: io( const string &filename ) :
        be_magic( swap_be(MAGIC) ),
        name( filename )
        {
        }

        uint16_t rc::io:: id_be_length( const string &identifier) const
        {
            const size_t id_size = identifier.length();
            if( id_size > static_cast<size_t>(limit_of<uint16_t>::maximum) )
            {
                throw exception("rc::io( identifer size too long )");
            }
            return swap_be16(id_size);
        }
    }

}


namespace upsylon
{
    namespace ios
    {

        const uint32_t rc::MAGIC;

        rc::writer:: ~writer() throw()
        {
        }

        rc::writer:: writer( const string &filename ) :
        io( filename ),
        fp( name, ios::writable ),
        total(0),
        hasher()
        {
            fp.unwind();
        }

        void rc::writer:: append_file(const string &datafile,
                                      const string &identifier)
        {
        }

        void rc::writer:: mark()
        {
            static const size_t be_magic_size = sizeof(be_magic);
            size_t done = 0;
            descriptor::put( *fp, &be_magic, be_magic_size, done);
            if(done!=be_magic_size)
            {
                throw exception("rc::write::mark(error for [%s])", *name );
            }
            total += be_magic_size;
        }

        void rc::writer:: emit(const string &id)
        {
            const uint16_t be_sz = id_be_length(id);
            {
                size_t done = 0;
                descriptor::put( *fp, &be_sz, sizeof(be_sz), done);
                if( done != sizeof(be_sz) )
                {
                    throw exception("rc::write::emit(error for [%s] identifier length)", *name );
                }
                total += sizeof(be_sz);
            }

            {
                const  size_t size = id.length();
                size_t        done = 0;
                descriptor::put( *fp, id.ro(), size, done);
                if( done != size )
                {
                    throw exception("rc::write::sign(error for [%s] identifier content)", *name );
                }
                total += size;
            }
        
        }

        void rc::writer:: sign(const key_t k)
        {
            const key_t be_k = swap_be(k);
            size_t      done = 0;
            descriptor::put( *fp, &be_k, sizeof(key_t), done);
            if(done!=sizeof(key_t))
            {
                throw exception("rc::write::sign(error for [%s])", *name );
            }
            total += sizeof(key_t);
        }

        void rc::writer:: append_data(const void *data, const size_t size, const string &identifier)
        {
            assert(!(data==NULL&&size>0));
            mark();
            emit(identifier);
            hasher.set();
            hasher(identifier);
            hasher.run(data, size);

            sign(hasher.key<key_t>());
            {
                size_t done = 0;
                descriptor::put( *fp, data, size, done);
                if(done!=size)
                {
                    throw exception("rc::write::append_data(error for [%s] <- '%s')", *name, *identifier);
                }
                total += size;
            }
            
        }

        void rc::writer:: append_data(const void *data, const size_t size, const char *identifier)
        {
            const string _(identifier); append_data(data,size,_);
        }

        void rc::writer:: append_data( const char *text, const string &identifier )
        {
            append_data( text, length_of(text), identifier );
        }

        void rc::writer:: append_data( const char *text, const char *identifier )
        {
            const string _(identifier); append_data(text,_);
        }

    }
}
