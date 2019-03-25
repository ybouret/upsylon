#include "y/ios/rc/io.hpp"
#include "y/string/io.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace ios
    {
        rc:: writer:: ~writer() throw()
        {
        }

#define Y_RC_CTOR()  io(filename), closed(false), fp(filename,true), sz(0)

        rc:: writer:: writer( const string &filename ) :
        Y_RC_CTOR()
        {
        }

        rc:: writer:: writer( const char *filename ) :
        Y_RC_CTOR()
        {
        }


        void rc::writer:: mark()
        {
            fp.emit<uid_t>( magic );
            sz += sizeof(uid_t);
        }

        void rc:: writer:: sign()
        {
            fp.emit<key_t>( hash.key<key_t>() );
            sz += sizeof(key_t);
        }

        void rc::writer:: append_data( const string &identifier, const void *data, const size_t size )
        {
            static const char fn[] = "rc.writer.append_data";

            // initialize
            if(closed) throw exception("%s([%s] is closed)",fn,*name);
            hash.set();

            // mark
            size_t written = sz;
            mark();

            // identifier
            sz += string_io::save_binary(fp,identifier);

            // data length
            {
                size_t shift = 0;
                fp.emit_upack<size_t>(size,&shift);
                sz += shift;
            }

            // data
            fp.output((const char *)data,size);
            sz += size;
            std::cerr << "bytes=" << size << std::endl;

            // sign
            hash(identifier);
            hash(data,size);
            sign();
            written = sz - written;
            std::cerr << "#written=" << written << std::endl;
        }


        void rc::writer:: append_data( const char *identifier, const void *data, const size_t size )
        {
            const string _(identifier);
            append_data(_, data, size);
        }


        void  rc::writer:: append_data( const string &identifier, const char *text)
        {
            append_data(identifier, text, length_of(text) );
        }

        void  rc::writer:: append_data( const char *identifier, const char *text)
        {
            const string _(identifier);
            append_data(_,text);
        }

        void rc::writer:: append_data( const string &identifier, const memory::ro_buffer &buf )
        {
            append_data(identifier, buf.ro(), buf.length());
        }

        void rc::writer:: append_data( const char   *identifier, const memory::ro_buffer &buf )
        {
            const string _(identifier);
            append_data(_,buf);
        }


        void rc::writer:: finalize()
        {
            static const char fn[] = "rc.writer.finalize";
            if(closed) throw exception("%s([%s] is closed)",fn,*name);

            (bool&)closed=true;
            std::cerr << "total=" << sz << std::endl;
            fp.emit<len_t>(sz);
            mark();
        }


    }

}
