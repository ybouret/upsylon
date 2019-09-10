//! \file
#ifndef Y_IOS_PLUGIN_INCLUDED
#define Y_IOS_PLUGIN_INCLUDED 1

#include "y/ios/serializable.hpp"
#include "y/ios/istream.hpp"
#include "y/ios/ostream.hpp"
#include "y/type/args.hpp"
#include "y/type/fourcc.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace ios
    {


        //! plugin interface
        class plugin : public counted_object
        {
        public:
            typedef arc_ptr<plugin> pointer;
            const uint32_t uuid;

            virtual size_t  load(ios::istream &, void       *) = 0;
            virtual size_t  save(ios::ostream &, const void *) = 0;
            virtual plugin *clone() const                      = 0;
            virtual        ~plugin() throw();

        protected:
            explicit plugin(const uint32_t ) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(plugin);

        };


        template <typename T>
        class nbo_plugin : public plugin
        {
        public:
            static const uint32_t UUID = Y_FOURCC('@','N','B','O');
            Y_DECL_ARGS(T,type);

            inline virtual ~nbo_plugin() throw() {}
            inline explicit nbo_plugin() throw() : plugin(UUID) {}

            inline virtual size_t load(ios::istream &fp, void *addr)
            {
                assert(addr);
                size_t tmp = 0;
                *static_cast<mutable_type*>(addr) = fp.read_net<mutable_type>(&tmp);
                return tmp;
            }

            inline virtual size_t save(ios::ostream &fp, const void *addr)
            {
                assert(addr);
                size_t tmp = 0;
                fp.emit_net<T>( *static_cast<const type*>(addr), &tmp);
                return tmp;
            }

            inline virtual plugin * clone() const { return new nbo_plugin<T>; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(nbo_plugin);
        };



        template <typename T>
        class raw_plugin : public plugin
        {
        public:
            Y_DECL_ARGS(T,type);
            static const uint32_t UUID = Y_FOURCC('@','R','A','W');

            inline virtual ~raw_plugin() throw() {}
            inline explicit raw_plugin() throw() : plugin(UUID) {}


            inline virtual size_t save(ios::ostream &fp, const void *addr)
            {
                assert(addr);
                fp.output(addr, sizeof(type));
                return sizeof(type);
            }

            inline virtual size_t load(ios::istream &fp, void *addr)
            {
                fp.input(addr,sizeof(type));
                return sizeof(type);
            }

            inline virtual plugin * clone() const { return new raw_plugin<T>; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(raw_plugin);
        };

        template <typename T>
        class srz_plugin : public plugin
        {
        public:
            Y_DECL_ARGS(T,type);
            static const uint32_t UUID = Y_FOURCC('@','S','R','Z');

            inline virtual ~srz_plugin() throw() {}
            inline explicit srz_plugin() throw() : plugin(UUID) {}


            inline virtual size_t save(ios::ostream &fp, const void *addr)
            {
                assert(addr);
                const T            *pObj = static_cast<const T*>(addr);
                const serializable *pSrz = pObj;
                return pSrz->serialize(fp);
            }

            inline virtual size_t load(ios::istream &fp, void *addr)
            {
                assert(addr);
                size_t count = 0;
                *static_cast<mutable_type*>(addr) = T::read(fp,&count);
                return count;
            }

            inline virtual plugin * clone() const { return new srz_plugin<T>; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(srz_plugin);
        };




        //! base class for I/O of type on [i|o]streams
        class _plugin
        {
        public:
            //! save procedure, should ADD bytes to count
            typedef void (*save_proc)( ios::ostream &, const void *, size_t *);
            //! load procedure, shoudl ADD bytes to count
            typedef void (*load_proc)( ios::istream &, void       *, size_t *);

            const save_proc save; //!< how to save
            const load_proc load; //!< how to load


            _plugin(const save_proc, const load_proc) throw();   //!< setup
            _plugin(const _plugin &other) throw();                //!< copy
            virtual ~_plugin() throw();                          //!< cleanup

            //! save in network byte order
            template <typename T> static inline
            void save_net( ios::ostream &fp, const void *addr, size_t *ns )
            {
                assert(addr);
                size_t tmp = 0;
                fp.emit_net<T>( *static_cast<const T*>(addr), &tmp);
                gist::add_to(ns,tmp);
            }

            //! load in network byte order
            template <typename T> static inline
            void load_net( ios::istream &fp, void *addr, size_t *nl )
            {
                assert(addr);
                size_t tmp = 0;
                *static_cast<T*>(addr) = fp.read_net<T>(&tmp);
                gist::add_to(nl,tmp);
            }

            //! save binary
            template <typename T> static inline
            void save_raw( ios::ostream &fp, const void *addr, size_t *ns )
            {
                assert(addr);
                fp.output(addr,sizeof(T));
                gist::add_to(ns,sizeof(T));
            }

            //! load binary
            template <typename T> static inline
            void load_raw( ios::istream &fp, void *addr, size_t *nl )
            {
                assert(addr);
                fp.input(addr,sizeof(T));
                gist::add_to(nl,sizeof(T));
            }

            //! save serializable
            template <typename T> static inline
            void save_srz( ios::ostream &fp, const void *addr, size_t *ns )
            {
                assert(addr);
                const T            *pObj = static_cast<const T*>(addr);
                const serializable *pSrz = pObj;
                gist::add_to(ns,pSrz->serialize(fp));
            }

            //! load serialized
            template <typename T> static inline
            void load_srz( ios::istream &fp, void *addr, size_t *nl )
            {
                assert(addr);
                size_t count = 0;
                *static_cast<T*>(addr) = T::read(fp,&count);
                gist::add_to(nl,count);
            }


        private:
            Y_DISABLE_ASSIGN(_plugin);
        };

        //! macro to implement some plugins
#define Y_IOS_PLUGIN(TYPE)                                 \
template <typename T>                                      \
class plugin_##TYPE : public _plugin {                      \
public:                                                    \
Y_DECL_ARGS(T,type);                                       \
inline plugin_##TYPE() throw() :                           \
_plugin( save_##TYPE<mutable_type>,                         \
load_##TYPE<mutable_type> ) {}                             \
inline plugin_##TYPE(const plugin_##TYPE &other) throw() : \
_plugin(other) {}                                           \
inline virtual ~plugin_##TYPE() throw() {}                 \
private: Y_DISABLE_ASSIGN(plugin_##TYPE);                  \
}

        Y_IOS_PLUGIN(net); //!< using network byte order
        Y_IOS_PLUGIN(raw); //!< using binary bloks
        Y_IOS_PLUGIN(srz); //!< using serialize/read

    }
}



#endif

