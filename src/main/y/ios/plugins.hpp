
//! \file
#ifndef Y_IOS_PLUGINS_INCLUDED
#define Y_IOS_PLUGINS_INCLUDED 1

#include "y/ios/plugin.hpp"
#include "y/associative/set.hpp"
#include <typeinfo>

namespace upsylon
{
    namespace ios
    {
        //----------------------------------------------------------------------
        //
        //
        //! lightweight type+pointer carrier
        //
        //
        //----------------------------------------------------------------------
        class class_plugin
        {
        public:
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const std::type_info   &tid; //!< system typeid
            const plugin::pointer   plg; //!< shared pointer

            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            const std::type_info & key() const throw();                             //!< key for database
            class_plugin( const std::type_info &, const plugin::pointer &) throw(); //!< setup
            class_plugin(const class_plugin &other) throw();                        //!< copy
            ~class_plugin() throw();                                                //!< cleanup

            //------------------------------------------------------------------
            //
            //! dedicated hasher
            //
            //------------------------------------------------------------------
            class hasher
            {
            public:
                hasher() throw();                                        //!< setup
                ~hasher() throw();                                       //!< cleanup
                size_t operator()(const std::type_info &) const throw(); //!< internal hashing
            private:
                Y_DISABLE_COPY_AND_ASSIGN(hasher);
            };

        private:
            Y_DISABLE_ASSIGN(class_plugin);
        };

        //----------------------------------------------------------------------
        //
        //
        //! to create database of plugins
        //
        //
        //----------------------------------------------------------------------
        typedef set<std::type_info,class_plugin,class_plugin::hasher> plugins_type;

        //! dedicated operations, factory design pattern
        class plugins : public plugins_type
        {
        public:
            explicit plugins() throw(); //!< setup
            virtual ~plugins() throw(); //!< cleanup

            //! register a plugin with its type
            template <typename T> inline
            void declare( plugin *P )
            {
                const plugin::pointer  ptr(P);
                declare( typeid(T), ptr );
            }

            //! network byte order plugin for T
            template <typename T> inline void declare_nbo() { declare<T>( new nbo_plugin<T>() ); }

            //! binary for T
            template <typename T> inline void declare_raw() { declare<T>( new raw_plugin<T>() ); }

            //! fully serializable T
            template <typename T> inline void declare_srz() { declare<T>( new srz_plugin<T>() ); }

            //! return a cloned plugin
            plugin * create_for( const std::type_info & ) const;

            //! helper
            template <typename T> inline plugin * query() const { return create_for( typeid(T) ); }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(plugins);
            void declare( const std::type_info &tid, const plugin::pointer  &ptr );
        };


    }
}

#endif

