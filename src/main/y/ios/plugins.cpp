#include "y/ios/plugins.hpp"
#include "y/exception.hpp"

namespace upsylon
{

#if 0
    namespace ios
    {


        class_plugin:: hasher::  hasher() throw() {}
        class_plugin:: hasher:: ~hasher() throw() {}

    }
#endif
    
}

namespace upsylon
{

    namespace ios
    {
        class_plugin:: ~class_plugin() throw()
        {
        }


        class_plugin:: class_plugin( const std::type_info &t, const plugin::pointer  &p) throw() :
        tid(t), plg(p)
        {
        }

        class_plugin:: class_plugin(const class_plugin &other) throw() :
        tid( other.tid ),
        plg( other.plg )
        {
        }

        const std::type_info &  class_plugin:: key() const throw()
        {
            return tid;
        }

    }

}


namespace upsylon
{

    namespace ios
    {

        plugins:: plugins() throw() : plugins_type()
        {
        }

        plugins:: ~plugins() throw()
        {
        }

        void plugins:: declare(const std::type_info    &tid,
                               const plugin::pointer   &ptr )
        {
            static const char fn[] = "ios::plugins:declare";

            const class_plugin *p = search(tid);
            if(p)
            {
                if(ptr->uuid==p->plg->uuid)
                {
                    // ok, same types, same plugin, system dependent
                }
                else
                {
                    throw exception("%s(different plugins for same type_info<%s>)",fn,tid.name());
                }
            }
            else
            {
                const class_plugin tmp(tid,ptr);
                if(!insert(tmp)) throw exception("%s(unexpected insert failure for <%s>)",fn, tid.name() );
            }
        }

        plugin * plugins:: create_for( const std::type_info &tid ) const
        {
            const class_plugin *p = search(tid);
            if(!p) throw exception("no ios::plugin for <%s>", tid.name() );
            return p->plg->clone();
        }


    }

}

