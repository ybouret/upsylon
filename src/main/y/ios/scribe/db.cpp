#include "y/ios/scribe/db.hpp"
#include "y/type/aliasing.hpp"
#include "y/ios/scribe/tuple.hpp"
#include "y/type/complex.hpp"
#include "y/type/point3d.hpp"
#include "y/yap.hpp"

namespace upsylon
{
    Y_SINGLETON_IMPL(ios::scribes);
    
    namespace ios
    {

        namespace
        {
            template <typename T, typename U>
            class native_for : public native_scribe
            {
            public:
                inline explicit native_for(const char *how) :
                native_scribe(typeid(T), how)
                {
                }

                inline virtual ~native_for() throw()
                {
                }

                inline virtual string write(const void *addr) const
                {
                    assert(addr);
                    const U  cnv(*static_cast<const T *>(addr));
                    return vformat(*fmt,cnv);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(native_for);
            };

#define Y_SCRIBE_IMPL(CLASS,CODE) \
/**/  class CLASS##_scribe : public scribe1D {\
/**/  Y_DISABLE_COPY_AND_ASSIGN(CLASS##_scribe);\
/**/  public:\
/**/  inline explicit CLASS##_scribe() : scribe1D( typeid(CLASS) ) {}\
/**/  inline virtual ~CLASS##_scribe() throw() {}\
/**/  inline virtual string write(const void *addr) const {\
/**/      assert(addr); const CLASS &obj = *static_cast<const CLASS*>(addr);\
/**/      CODE;\
/**/      }\
/**/  }

            Y_SCRIBE_IMPL(string,return obj);
            Y_SCRIBE_IMPL(apn,return obj.to_dec());
            Y_SCRIBE_IMPL(apz,return obj.to_dec());

            class apq_writer : public scribe1D
            {
                Y_DISABLE_COPY_AND_ASSIGN(apq_writer);
                const scribe &dbl;

            public:
                inline explicit apq_writer(const scribe &_ ) :
                scribe1D(typeid(apq)),
                dbl(_)
                {
                }

                inline virtual ~apq_writer() throw()
                {

                }

                inline virtual string write(const void *addr) const
                {
                    assert(addr);
                    const double d = static_cast<const apq *>(addr)->to_double();
                    return dbl.write(&d);
                }

            };

        }

        scribes:: ~scribes() throw()
        {
        }

#define Y_SCRIBE_INAT(TYPE,CONV,FMT) insert_native( new native_for<TYPE,CONV>(FMT) )
#define Y_SCRIBE_UNAT(TYPE,CONV,FMT) insert_native( new native_for<unsigned TYPE, unsigned CONV>(FMT) )
#define Y_SCRIBE_BOTH(TYPE,CONV,I,U) Y_SCRIBE_INAT(TYPE,CONV,I); Y_SCRIBE_UNAT(TYPE,CONV,U)


#define Y_SCRIBE_NAT(TYPE,FMT) Y_SCRIBE_INAT(TYPE,TYPE,FMT)


#define Y_SCRIBE_TUPLE(TUPLE,TYPE) insert_common( new tuple_scribe<TUPLE,TYPE>( native<TYPE>() ) )

        scribes:: scribes() : all(), nat()
        {
            Y_SCRIBE_BOTH(char, int,"%d","%u");
            Y_SCRIBE_BOTH(short,int,"%d","%u");
            Y_SCRIBE_BOTH(int,int,"%d","%u");
            Y_SCRIBE_BOTH(long,long,"%ld","%lu");

            Y_SCRIBE_INAT(int8_t,long,"%ld");
            Y_SCRIBE_INAT(uint8_t,unsigned long,"%lu");

            Y_SCRIBE_INAT(int16_t,long,"%ld");
            Y_SCRIBE_INAT(uint16_t,unsigned long,"%lu");

            Y_SCRIBE_INAT(int32_t,long,"%ld");
            Y_SCRIBE_INAT(uint32_t,unsigned long,"%lu");


            Y_SCRIBE_INAT(size_t,unsigned long, "%lu" );
            Y_SCRIBE_INAT(ptrdiff_t,long, "%ld" );

            Y_SCRIBE_NAT(float,"%.15g");
            Y_SCRIBE_NAT(double,"%.15g");

            insert_common( new string_scribe() );
            insert_common( new apn_scribe()    );
            insert_common( new apz_scribe()    );
            insert_common( new apq_writer( native<double>() ) );

            Y_SCRIBE_TUPLE(complex,float);
            Y_SCRIBE_TUPLE(complex,double);

            Y_SCRIBE_TUPLE(point2d,float);
            Y_SCRIBE_TUPLE(point2d,double);
            Y_SCRIBE_TUPLE(point2d,unit_t);


            Y_SCRIBE_TUPLE(point3d,float);
            Y_SCRIBE_TUPLE(point3d,double);
            Y_SCRIBE_TUPLE(point3d,unit_t);

        }

        void scribes:: insert_common(scribe *ptr)
        {
            assert(ptr);
            const scribe_handle h = ptr;
            if(! aliasing::_(all).insert(h) ) throw exception("%s(multiple <%s> in all)", call_sign, h->info.text());
        }


        void scribes:: insert_native(native_scribe *ptr)
        {
            assert(ptr);
            const native_handle h = ptr;
            natives_store      &s = aliasing::_(nat);

            if( s.search(h->info) )
            {
                return; //!< same typeid
            }

            if( !s.insert(h) )
            {
                throw exception("%s(multiple <%s> in native)", call_sign, h->info.text() );
            }

            try
            {
                insert_common(ptr);
            }
            catch(...)
            {
                (void)s.remove(h->info);
                throw;
            }
        }

        const native_scribe & scribes:: native(const std::type_info &tid) const
        {
            const rtti          &key = rtti::of(tid);
            const native_handle *pps = nat.search(key);
            if(!pps)
            {
                throw exception("%s(no native for <%s>)", call_sign, key.text() );
            }
            return **pps;
        }

        const scribe & scribes:: get(const std::type_info &tid) const
        {
            const rtti          &key = rtti::of(tid);
            const scribe_handle *pps = all.search(key);
            if(!pps)
            {
                throw exception("%s(none for <%s>)", call_sign, key.text() );
            }
            return **pps;
        }

        const scribe & scribe:: query(const std::type_info &tid)
        {
            static const scribes &mgr = scribes::instance();
            return mgr.get(tid);
        }

        
    }
}
