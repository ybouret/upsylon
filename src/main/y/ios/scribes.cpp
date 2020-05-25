
#include "y/ios/scribes.hpp"
#include "y/associative/be-key.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/spec.hpp"
#include "y/type/complex.hpp"
#include "y/type/point3d.hpp"

namespace upsylon {

    namespace ios {


        namespace
        {
#define Y_IOS_SCRIBE(TYPE,FMT,CAST) \
class scribe_##TYPE : public basic_scribe {\
public:\
inline explicit scribe_##TYPE() : basic_scribe( typeid(TYPE), FMT ) {}\
inline virtual ~scribe_##TYPE() throw() {}\
private:\
Y_DISABLE_COPY_AND_ASSIGN(scribe_##TYPE);\
inline virtual void write( ios::ostream &fp, const void *addr ) const \
{ assert(addr); fp(*fmt,static_cast<CAST>( *static_cast<const TYPE *>(addr) ) ); }\
}

            Y_IOS_SCRIBE(int8_t,   "%d", int);
            Y_IOS_SCRIBE(uint8_t,  "%u", unsigned);
            Y_IOS_SCRIBE(int16_t,  "%d", int);
            Y_IOS_SCRIBE(uint16_t, "%u", unsigned);
            Y_IOS_SCRIBE(int32_t,  "%d", long);
            Y_IOS_SCRIBE(uint32_t, "%u", unsigned long);

            Y_IOS_SCRIBE(char, "%c", char);
            typedef unsigned char __scribe_uchar;
            Y_IOS_SCRIBE(__scribe_uchar, "%u", unsigned);

            Y_IOS_SCRIBE(short, "%d", int);
            typedef unsigned short __scribe_ushort;
            Y_IOS_SCRIBE(__scribe_ushort, "%u", unsigned);

            Y_IOS_SCRIBE(int, "%d", int);
            typedef unsigned int __scribe_uint;
            Y_IOS_SCRIBE(__scribe_uint, "%u", unsigned);

            Y_IOS_SCRIBE(long, "%ld", long);
            typedef unsigned long __scribe_ulong;
            Y_IOS_SCRIBE(__scribe_ulong, "%lu", unsigned long);

            Y_IOS_SCRIBE(float, "%.15g",float);
            Y_IOS_SCRIBE(double,"%.15g",double);

            class scribe_string : public scribe
            {
            public:
                inline explicit scribe_string() : scribe( typeid(string), 1 )
                {
                }

                inline virtual ~scribe_string() throw()
                {}



            private:
                Y_DISABLE_COPY_AND_ASSIGN(scribe_string);
                inline virtual void write( ios::ostream &fp, const void *addr ) const
                {
                    assert(addr);
                    fp << *static_cast<const string *>(addr);
                }

            };


        }

#define Y_IOS_SCRIBE_(TYPE) do { if(!has_basic( typeid(TYPE) )) { const basic_writer w = new scribe_##TYPE(); insert(w); } } while(false)

        scribes:: scribes()
        {
            Y_IOS_SCRIBE_(int8_t);
            Y_IOS_SCRIBE_(uint8_t);
            Y_IOS_SCRIBE_(int16_t);
            Y_IOS_SCRIBE_(uint16_t);
            Y_IOS_SCRIBE_(int32_t);
            Y_IOS_SCRIBE_(uint32_t);

            Y_IOS_SCRIBE_(char);
            Y_IOS_SCRIBE_(__scribe_uchar);

            Y_IOS_SCRIBE_(short);
            Y_IOS_SCRIBE_(__scribe_ushort);

            Y_IOS_SCRIBE_(int);
            Y_IOS_SCRIBE_(__scribe_uint);

            Y_IOS_SCRIBE_(long);
            Y_IOS_SCRIBE_(__scribe_ulong);

            Y_IOS_SCRIBE_(float);
            Y_IOS_SCRIBE_(double);

            {
                const writer w = new scribe_string();
                insert(w);
            }

            tuple<complex,float>();
            tuple<complex,double>();

            tuple<point2d,float>();
            tuple<point3d,float>();
            tuple<point2d,double>();
            tuple<point3d,double>();

        }

        scribes:: ~scribes() throw()
        {

        }

        const scribe * scribes:: search( const std::type_info &t ) const throw()
        {
            const be_key  key = t;
            const writer *ppS = writers.search_by(key);
            if(ppS)
            {
                return & (**ppS);
            }
            else
            {
                return 0;
            }
        }




        const basic_scribe * scribes:: search_basic( const std::type_info &t ) const throw()
        {
            const be_key        key = t;
            const basic_writer *ppS = basic_writers.search_by(key);
            if(ppS)
            {
                return & (**ppS);
            }
            else
            {
                return 0;
            }
        }


        bool scribes:: has(const std::type_info &t) const throw()
        {
            const be_key key = t;
            return NULL != writers.search_by(key);
        }

        bool scribes:: has_basic(const std::type_info &t) const throw()
        {
            const be_key key = t;
            return NULL != basic_writers.search_by(key);
        }


        static const char fn[] = "ios::scribes: ";

        static inline const char *id_for(const std::type_info &t)
        {
            return *type_spec::declare(t).name();
        }

        template <typename PTR>
        static inline const char *get_id(const PTR &w)
        {
            return id_for(w->info);
        }

        void scribes:: insert(const basic_writer &w)
        {
            const be_key       key = w->info;
            basic_writers_db  &bdb = aliasing::_(basic_writers);
            if(!bdb.insert_by(key,w))
            {
                throw exception("%smultiple basic scribe for <%s>",fn,get_id(w));
            }

            const scribe &ref = *w;
            const writer W( &ref );
            try {
                if( ! aliasing::_(writers).insert_by(key,W) )
                {
                    throw exception("%sduplicate of basic scribe for <%s>",fn,get_id(w));
                }
            }
            catch(...)
            {
                bdb.remove_by(key);
                throw;
            }
        }

        void scribes:: insert(const writer &w)
        {
            const be_key       key = w->info;
            if( basic_writers.search_by(key) )
            {
                throw exception("%salready got a basic scribe for <%s>", fn,get_id(w) );
            }

            if( ! aliasing::_(writers).insert_by(key,w) )
            {
                throw exception("%smultiple scribe for <%s>", fn, get_id(w) );
            }
        }

        const scribe & scribes:: get( const std::type_info &t ) const
        {
            const scribe *p = search(t);
            if(!p) throw exception("%sno scribe for <%s>",fn,id_for(t));
            return *p;
        }

        string & scribes:: format(const std::type_info &t)
        {
            const basic_scribe *s = search_basic(t);
            if(!s)
            {
                throw exception("%sno format for <%s>",fn,id_for(t));
            }
            const string &fmt = s->fmt;
            return aliasing::_(fmt);
        }


        const basic_scribe & scribes:: get_basic( const std::type_info &t ) const
        {
            const basic_scribe *p = search_basic(t);
            if(!p) throw exception("%sno basic scribe for <%s>",fn,id_for(t));
            return *p;
        }


        
    }

}

