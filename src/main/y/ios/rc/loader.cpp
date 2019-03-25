
#include "y/ios/rc/io.hpp"
#include "y/string/io.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace ios
    {
        rc:: loader:: ~loader() throw()
        {
        }

#define Y_RC_CTOR() io(filename), fp(filename), items()

        rc:: loader:: loader( const string &filename ) :
        Y_RC_CTOR()
        {
            initialize();
        }

        rc:: loader:: loader( const char *filename ) :
        Y_RC_CTOR()
        {
            initialize();
        }


        void rc:: loader:: initialize()
        {
            static const char fn[] = "rc.loader.initialize";
            static const offset_t off_uid = sizeof(uid_t);
            static const offset_t off_len = sizeof(len_t)+off_uid;

            std::cerr << "length=" << fp.length() << std::endl;
            
            // goto end and look for magick
            fp.seek(-off_uid,from_end);
            const uint32_t mark = fp.read<uint32_t>();
            if(mark!=magic)
            {
                throw exception("%s([%s] invalid final magic)",fn,*name);
            }
            std::cerr << "found magic" << std::endl;

            // read total written size
            const offset_t off_end = fp.seek(-off_len,from_end);
            const len_t total = fp.read<len_t>();
            std::cerr << "found total=" << total << std::endl;
            const offset_t shift = total+off_len;

            // prepare reading
            (void) fp.seek(-shift,from_end);
            while( fp.tell()<off_end )
            {
                if( magic != fp.read<uint32_t>() )
                {
                    throw exception("%s([%s] bad magic header)",fn,*name);
                }
                const string id = string_io::load_binary(fp);
                std::cerr << "found '" << id << "'" << std::endl;
                offset_t start = fp.tell();
                size_t   extra = 0;
                const size_t   bytes = fp.read_upack<size_t>(&extra);
                start += extra;
                std::cerr << "bytes=" << bytes << "@" << start << std::endl;

                hash.set();
                hash(id);
                for(size_t i=0;i<bytes;++i)
                {
                    char C = 0;
                    if(!fp.query(C)) throw exception("%s([%s] missing data for '%s')",fn,*name,*id);
                    hash(&C,1);
                }
                const key_t k = hash.key<key_t>();
                if( k != fp.read<key_t>() )
                {
                    throw exception("%s([%s] corrupted '%s')",fn,*name,*id);
                }
                std::cerr << "...checked" << std::endl;
                const item::ptr it = new item(id,start,bytes);
                if( !items.insert(it) )
                {
                    throw exception("%s([%s] multiple resource '%s')", fn, *name, *id);
                }
            }

            std::cerr << items << std::endl;


        }


        string rc::loader:: load_string( const string &id )
        {
            static const char fn[] = "rc.loader.load_string";

            const item::ptr *ppItem = items.search(id);
            if(!ppItem) throw exception("%s(no <%s> in [%s])", fn, *id, *name );

            const item &it = **ppItem;
            string ans(it.bytes,as_capacity);
            fp.seek(it.start,from_set);
            size_t n = it.bytes;
            char   C = 0;
            while(n-->0)
            {
                if(!fp.query(C)) throw exception("%s(missing data for <%s> in [%s])", fn, *id, *name);
                ans << C;
            }

            return ans;
        }


    }

}


