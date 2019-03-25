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

#define Y_RC_CTOR()  io(filename,v), closed(false), fp(filename,true), sz(0)
#define Y_RC_SHOW() if(verbose) std::cerr << "** rc.open.write [" << name << "]" << std::endl

        rc:: writer:: writer( const string &filename, const bool v ) :
        Y_RC_CTOR()
        {
            Y_RC_SHOW();
        }

        rc:: writer:: writer( const char *filename, const bool v ) :
        Y_RC_CTOR()
        {
            Y_RC_SHOW();
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

            if(verbose)
            {
                std::cerr << "** rc.append_data '" << identifier << "', #bytes=" << size << std::endl;
            }
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

            // sign
            hash(identifier);
            hash(data,size);
            sign();
            written = sz - written;
            if(verbose)
            {
                std::cerr << "** rc.append_data #written=" << written << std::endl;
            }
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
            if(verbose)
            {
                std::cerr << "** rc.finalize #total=" << sz << std::endl;
            }
            fp.emit<len_t>(sz);
            mark();
        }

        void rc:: writer:: append_file( const string &identifier, const string &filename )
        {
            static const char fn[] = "rc.writer.append_file";

            if(verbose)
            {
                std::cerr << "** rc.append_file '" << identifier << "' from [" << filename << "]" << std::endl;
            }

            // initialize
            if(closed) throw exception("%s([%s] is closed)",fn,*name);
            hash.set();
            ios::irstream inp(filename);

            // mark
            size_t written = sz;
            mark();

            // identifier
            sz += string_io::save_binary(fp,identifier);
            hash(identifier);
            const size_t size = inp.length();

            // data length
            {
                size_t shift = 0;
                fp.emit_upack<size_t>(size,&shift);
                sz += shift;
            }
            assert(0==inp.tell());

            // write data
            {
                char C = 0;
                for(size_t i=0;i<size;++i)
                {
                    if( !inp.query(C) )
                    {
                        throw exception("%s([%s] missing bytes!)",fn,*name);
                    }
                    hash(&C,1);
                    fp.write(C);
                }
            }
            sz += size;

            sign();
            written = sz - written;
            if(verbose)
            {
                std::cerr << "** rc.append_file #written=" << written << std::endl;
            }
        }

        void rc::writer:: append_file( const char   *identfier, const char *filename )
        {
            const  string _(identfier), __(filename);
            return append_file(_,__);
        }


    }

}
