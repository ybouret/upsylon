
//! \file

#ifndef Y_IOS_SCRIBE_TUPLE_INCLUDED
#define Y_IOS_SCRIBE_TUPLE_INCLUDED 1

#include "y/ios/scribe/basic.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {

    namespace ios {

        class tuple_scribe : public scribe
        {
        public:
            typedef arc_ptr<const basic_scribe> basic_ptr;

            virtual ~tuple_scribe() throw();

        protected:
            explicit tuple_scribe(const std::type_info &t,
                                  const size_t          n,
                                  const basic_ptr      &h);

            const basic_ptr handle;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tuple_scribe);
        };

        template <template <typename> class TUPLE,typename T>
        class tuple_scribe_for : public tuple_scribe
        {
        public:
            inline virtual ~tuple_scribe_for() throw()
            {
            }

            inline explicit tuple_scribe_for( const basic_ptr &h ) :
            tuple_scribe( typeid( TUPLE<T> ), sizeof( TUPLE<T> )/sizeof(T), h )
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(tuple_scribe_for);
            inline virtual void write( ios::ostream &fp, const void *addr ) const
            {
                assert(addr);
                assert(count>0);
                const T *p = static_cast<const T *>(addr);
                handle->put(fp,p[0]);
                for(size_t i=1;i<count;++i)
                {
                    fp << ' ';
                    handle->put(fp,p[1]);
                }
            }
        };



    }

}

#endif
