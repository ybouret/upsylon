//! \file
#ifndef Y_IOS_PLUGIN_INCLUDED
#define Y_IOS_PLUGIN_INCLUDED 1

#include "y/ios/serializable.hpp"
#include "y/ios/istream.hpp"
#include "y/ios/ostream.hpp"
#include "y/ios/comms.hpp"
#include "y/type/args.hpp"
#include "y/type/fourcc.hpp"
#include "y/type/spec.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace ios
    {

        //----------------------------------------------------------------------
        //
        //
        //! plugin interface
        //
        //
        //----------------------------------------------------------------------
        class plugin : public counted_object
        {
        public:
            typedef arc_ptr<plugin> pointer; //!< shared pointer

            const uint32_t          uuid;    //!< identifier
            const comms::delivery   mode;    //!< for I/O information
            
            virtual size_t  load(ios::istream &, void       *) = 0; //!< load data
            virtual size_t  save(ios::ostream &, const void *) = 0; //!< save data
            virtual plugin *clone() const                      = 0; //!< clone this
            virtual        ~plugin() throw();                       //!< cleanup

        protected:
            explicit plugin(const uint32_t, const comms::delivery) throw(); //!< setup with identifier
            //! throw on missing data
            void missing_data(const char *pluginName,const char *typeName) const;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(plugin);

        };

        //----------------------------------------------------------------------
        //
        //! network byte order plugin
        //
        //----------------------------------------------------------------------
        template <typename T>
        class nbo_plugin : public plugin
        {
        public:
            static const uint32_t UUID = Y_FOURCC('@','N','B','O'); //!< identifier
            Y_DECL_ARGS(T,type);                                    //!< aliases

            inline virtual ~nbo_plugin() throw() {}                 //!< cleanup
            inline explicit nbo_plugin() throw() :
            plugin(UUID,comms::computed_block_size) {}  //!< setup

            //! load with swap big endian
            inline virtual size_t load(ios::istream &fp, void *addr)
            {
                assert(addr);
                size_t shift = 0;
                if(!fp.query_nbo(*static_cast<mutable_type*>(addr),shift))
                {
                    missing_data("nbo",*type_name_of<T>() );
                }
                return shift;
            }

            //! save with swap big endian
            inline virtual size_t save(ios::ostream &fp, const void *addr)
            {
                assert(addr);
                return fp.write_nbo( *static_cast<const type*>(addr) );
            }

            //! return a new nbo_plugin
            inline virtual plugin * clone() const { return new nbo_plugin<T>; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(nbo_plugin);
        };

        //----------------------------------------------------------------------
        //
        //! raw binary plugin
        //
        //----------------------------------------------------------------------
        template <typename T>
        class raw_plugin : public plugin
        {
        public:
            Y_DECL_ARGS(T,type);                                     //!< aliases
            static const uint32_t UUID = Y_FOURCC('@','R','A','W');  //!< identifier

            inline virtual ~raw_plugin() throw() {}                  //!< cleanup
            inline explicit raw_plugin() throw() :
            plugin(UUID,comms::computed_block_size) {}                       //!< setup

            //! direct write of bytes
            inline virtual size_t save(ios::ostream &fp, const void *addr)
            {
                assert(addr);
                fp.output(addr, sizeof(type));
                return sizeof(type);
            }

            //! direct read of bytes
            inline virtual size_t load(ios::istream &fp, void *addr)
            {
                if(!fp.try_query(addr, sizeof(type) )) missing_data("raw",*type_name_of<T>());
                return sizeof(type);
            }

            //! return a new raw_plugin
            inline virtual plugin * clone() const { return new raw_plugin<T>; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(raw_plugin);
        };

        //----------------------------------------------------------------------
        //
        //! plugin for serializable objects
        //
        //----------------------------------------------------------------------
        template <typename T>
        class srz_plugin : public plugin
        {
        public:
            Y_DECL_ARGS(T,type);                                       //!< aliases
            static const uint32_t UUID = Y_FOURCC('@','S','R','Z');    //!< identifier

            inline virtual ~srz_plugin() throw() {}                    //!< cleanup
            inline explicit srz_plugin() throw() :
            plugin(UUID,comms::flexible_block_size) {}                         //!< setup


            //! save using the serializable API
            inline virtual size_t save(ios::ostream &fp, const void *addr)
            {
                assert(addr);
                const T            *pObj = static_cast<const T*>(addr);
                const serializable *pSrz = pObj;
                return pSrz->serialize(fp);
            }

            //! load assuming the existence of a T::read function
            inline virtual size_t load(ios::istream &fp, void *addr)
            {
                assert(addr);
                size_t count = 0;
                *static_cast<mutable_type*>(addr) = T::read(fp,count,"srz");
                return count;
            }

            //! return a new serializable plugin
            inline virtual plugin * clone() const { return new srz_plugin<T>; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(srz_plugin);
        };

        
    }
}



#endif

