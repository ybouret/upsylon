
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
        
        
        bool rc::io:: put_all( descriptor::type handle, const void *data, const size_t size)
        {
            if(size<=0)
            {
                return true;
            }
            else
            {
                size_t done = 0;
                descriptor::put(handle, data, size, done);
                return (done==size);
            }
            
        }
        
    }
    
}


#include "y/ios/icstream.hpp"

namespace upsylon
{
    namespace ios
    {
        
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
            ios::icstream inp(datafile);
           
            mark();
            emit(identifier);
            hasher.set();
            hasher(identifier);
            char C = 0;
            while( inp.query(C) )
            {
                hasher.run(&C,1);
                if(!put_all<uint8_t>(*fp,C))
                {
                    throw exception("rc::write::append_file(error for [%s])", *name );
                }
                ++total;
            }
            sign();
        }
        
        void rc::writer:: append_file(const string &datafile,
                                      const char   *identifier )
        {
            const string _(identifier);
            append_file(datafile,_);
        }
        
        void rc::writer:: append_file( const char   *datafile, const char   *identifier )
        {
            const string _(datafile), __(identifier);
            append_file(_,__);
        }


        
        void rc::writer:: mark()
        {
            if(!put_all<uid_t>(*fp,MAGIC))
            {
                throw exception("rc::write::mark(error for [%s])", *name );
            }
            total += sizeof(uid_t);
        }
        
        void rc::writer:: emit(const string &id)
        {
            if(!put_all<ssz_t>(*fp,id_be_length(id)))
            {
                throw exception("rc::write::emit(error for [%s] identifier length)", *name );
            }
            total += sizeof(ssz_t);
            
            {
                const  size_t size = id.length();
                if( !put_all( *fp, id.ro(), size) )
                {
                    throw exception("rc::write::sign(error for [%s] identifier content)", *name );
                }
                total += size;
            }
            
        }
        
        void rc::writer:: sign()
        {
            if( ! put_all<key_t>(*fp,hasher.key<key_t>()))
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
            
            if(!put_all(*fp,data,size))
            {
                throw exception("rc::write::append_data(error for [%s] <- '%s')", *name, *identifier);
            }
            
            sign();
            total += size;
            
            
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
        
        void rc::writer:: append_data( const memory::ro_buffer &buf, const string &identifier)
        {
            append_data( buf.ro(), buf.length(), identifier);
        }
        
        void rc::writer:: append_data( const memory::ro_buffer &buf, const char *identifier )
        {
            const string _(identifier);
            append_data(buf,_);
        }
        
        void rc::writer:: finish()
        {
            if( !put_all<len_t>(*fp,total) )
            {
                throw exception("rc::write::finish(error for [%s] <- total)", *name);
            }
            mark();
        }
        
    }
}
