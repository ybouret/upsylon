//! \file
#ifndef Y_IOS_PLUGIN_INCLUDED
#define Y_IOS_PLUGIN_INCLUDED 1

#include "y/ios/serializable.hpp"
#include "y/ios/istream.hpp"
#include "y/ios/ostream.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    namespace ios
    {

        class plugin
        {
        public:
            typedef void (*save_proc)( ios::ostream &, const void *, size_t *);
            typedef void (*load_proc)( ios::istream &, void       *, size_t *);

            const save_proc save;
            const load_proc load;

            plugin(const save_proc on_save, const load_proc on_load ) throw();
            plugin(const plugin &other) throw();
            virtual ~plugin() throw();

            template <typename T> static inline
            void save_net( ios::ostream &fp, const void *addr, size_t *ns )
            {
                assert(addr);
                fp.emit_net<T>( *static_cast<const T*>(addr) );
                gist::add_to(ns,sizeof(T));
            }

            template <typename T> static inline
            void load_net( ios::istream &fp, void *addr, size_t *nl )
            {
                assert(addr);
                *static_cast<T*>(addr) = fp.read_net<T>();
                gist::add_to(nl,sizeof(T));
            }

            template <typename T> static inline
            void save_raw( ios::ostream &fp, const void *addr, size_t *ns )
            {
                assert(addr);
                fp.output(addr,sizeof(T));
                gist::add_to(ns,sizeof(T));
            }

            template <typename T> static inline
            void load_raw( ios::istream &fp, void *addr, size_t *nl )
            {
                assert(addr);
                fp.input(addr,sizeof(T));
                gist::add_to(nl,sizeof(T));
            }

            template <typename T> static inline
            void save_srz( ios::ostream &fp, const void *addr, size_t *ns )
            {
                assert(addr);
                const T            *pObj = static_cast<const T*>(addr);
                const serializable *pSrz = pObj;
                gist::add_to(ns,pSrz->serialize(fp));
            }

            template <typename T> static inline
            void load_srz( ios::ostream &fp, void *addr, size_t *nl )
            {
                assert(addr);
                size_t count = 0;
                *static_cast<T*>(addr) = T::read(fp,&count);
                gist::add_to(nl,count);
            }


        private:
            Y_DISABLE_ASSIGN(plugin);
        };

#define Y_IOS_PLUGIN(TYPE) \
template <typename T> \
class plugin_##TYPE : public plugin\
{\
public:\
Y_DECL_ARGS(T,type);\
inline plugin_##TYPE() throw() : \
plugin( save_##TYPE<mutable_type>, load_##TYPE<mutable_type> ) {}\
inline plugin_##TYPE(const plugin_##TYPE &other) throw() : plugin(other) {}\
inline virtual ~plugin_##TYPE() throw() {}\
private:\
Y_DISABLE_ASSIGN(plugin_##TYPE);\
}

        Y_IOS_PLUGIN(net);
        Y_IOS_PLUGIN(raw);


    }
}



#endif

