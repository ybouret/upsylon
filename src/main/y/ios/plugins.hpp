
//! \file
#ifndef Y_IOS_PLUGINS_INCLUDED
#define Y_IOS_PLUGINS_INCLUDED 1

#include "y/ios/plugin.hpp"
#include "y/associative/set.hpp"
#include "y/hashing/type-mark.hpp"

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
            const type_mark         tid; //!< system typeid
            const plugin::pointer   plg; //!< shared pointer
            
            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            const type_mark & key() const throw();                             //!< key for database
            class_plugin( const type_spec &, const plugin::pointer &) throw(); //!< setup
            class_plugin(const class_plugin &other) throw();                        //!< copy
            ~class_plugin() throw();                                                //!< cleanup
            
            //------------------------------------------------------------------
            //
            //! dedicated hasher
            //
            //------------------------------------------------------------------
            typedef  hashing::type_mark_hasher<> hasher;
            
            
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
        typedef set<type_mark,class_plugin,class_plugin::hasher> plugins_type;
        
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
                declare( type_spec_of<T>(), ptr );
            }
            
            //! network byte order plugin for T
            template <typename T> inline void declare_nbo() { declare<T>( new nbo_plugin<T>() ); }
            
            //! binary for T
            template <typename T> inline void declare_raw() { declare<T>( new raw_plugin<T>() ); }
            
            //! fully serializable T
            template <typename T> inline void declare_srz() { declare<T>( new srz_plugin<T>() ); }
            
            //! return a cloned plugin
            plugin * create_for( const type_spec & ) const;
            
            //! helper
            template <typename T> inline plugin * query() const { return create_for( type_spec_of<T>() ); }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(plugins);
            void declare(const type_spec &,const plugin::pointer &);
        };
        
        
    }
}

#endif

