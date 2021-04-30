
#include "y/type/rtti.hpp"
#include "y/type/aliasing.hpp"
#include "y/hashing/fnv.hpp"

namespace upsylon
{
    rtti:: alias:: alias(const string &_) :
    object(), next(0), prev(0), name(_)
    {
    }

    rtti:: alias:: ~alias() throw()
    {
    }


    typedef hashing::fnv rtti_hasher;

    size_t rtti:: hash(const char *id) throw()
    {
        assert(id);
        return rtti_hasher::of(id);
    }

    size_t rtti:: hash(const std::type_info &tid) throw()
    {
        return hash( tid.name() );
    }

    size_t rtti:: hash(const string &id) throw()
    {
        return hash(*id);
    }

    rtti :: rtti(const std::type_info &tid) :
    uuid( tid.name() ),
    code( hash(uuid) ),
    user()
    {
    }

    rtti:: ~rtti() throw()
    {
        aliasing::_(code) = 0;
    }

    const string & rtti:: name() const throw()
    {
        return (user.size>0) ?  user.head->name : uuid;
    }

    const string & rtti:: key() const throw()
    {
        return uuid;
    }


}


#include "y/exception.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    Y_SINGLETON_IMPL(rtti::repo);

    rtti:: repo:: repo() : db(), id(), mx(0) {}

    rtti:: repo:: ~repo() throw()
    {
    }

    void rtti:: repo:: operator()(const std::type_info &tid)
    {
        const rtti::pointer   tptr = new rtti(tid);
        const string         &tkey = tptr->uuid;

        if( aliasing::_(db).insert(tptr) )
        {
            //__________________________________________________________________
            //
            // new one => register id
            //__________________________________________________________________
            if( !aliasing::_(id).insert(tkey,tptr) )
            {
                (void) aliasing::_(db).remove(tkey);
                throw  exception("%s(unexpected multiple <%s>)",call_sign,*tkey);
            }
        }
        else
        {
            //__________________________________________________________________
            //
            // already registered
            //__________________________________________________________________
            assert( NULL != id.search(tptr->uuid) );
        }
    }

    void rtti:: repo:: operator()(const std::type_info &tid, const string &tag)
    {
        const string         key = tid.name();
        const rtti::pointer *ppr = id.search(tag);
        if(ppr)
        {
            //__________________________________________________________________
            //
            // tag is not registered
            //__________________________________________________________________
            throw exception("%s('%s' already used for <%s> while aliasing <%s>)", call_sign, *tag, *( (*ppr)->name()), *key);
        }
        else
        {
            //__________________________________________________________________
            //
            // check if tid is registered
            //__________________________________________________________________
            ppr = db.search(key);
            if(!ppr)
            {
                const rtti::pointer   ptr = new rtti(tid);
                if(!aliasing::_(db).insert(ptr)) throw exception("%s(unexpected insert error for <%s>)",call_sign,*key);
                if( !(ppr = db.search(key) ) )   throw exception("%s(unexpected search error for <%s>)",call_sign,*key);
            }

            //__________________________________________________________________
            //
            // populate and register aliases
            //__________________________________________________________________
            aliases &aka = aliasing::_((**ppr).user);
            aka.push_back( new alias(tag) );
            try
            {
                if(! aliasing::_(id).insert(tag,*ppr) )
                {
                    throw exception("%s(unexpected insert error for <%s> as <%s>)",call_sign,*key,*tag);
                }
            }
            catch(...)
            {
                delete aka.pop_back();
                throw;
            }

            //__________________________________________________________________
            //
            // update
            //__________________________________________________________________
            aliasing::_(mx) = max_of(mx,tag.size());

        }

    }

    void rtti:: repo:: operator()(const std::type_info &tid, const char  *tag)
    {
        const string _(tag);
        (*this)(tid,_);
    }





}


