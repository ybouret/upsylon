
#include "y/type/rtti.hpp"
#include "y/type/aliasing.hpp"
#include "y/ios/align.hpp"

namespace upsylon
{
    rtti:: alias:: alias(const string &_) :
    object(), next(0), prev(0), name(_), priv()
    {
    }

    rtti:: alias:: ~alias() throw()
    {
    }

    

    rtti :: rtti(const std::type_info &tid) :
    uuid( tid.name()    ),
    ulen( uuid.length() ),
    user()
    {
    }

    const void * rtti:: ro() const throw()
    {
        return *uuid;
    }
    
    
    size_t rtti:: length() const throw()
    {
        return ulen;
    }
    
    rtti:: ~rtti() throw()
    {
    }

    const string & rtti:: name() const throw()
    {
        return (user.size>0) ?  user.head->name : uuid;
    }

    const string & rtti:: key() const throw()
    {
        return uuid;
    }
    
    bool operator==(const rtti &lhs, const rtti &rhs) throw()
    {
        return lhs.uuid == rhs.uuid;
    }
    
    bool operator!=(const rtti &lhs, const rtti &rhs) throw()
    {
        return lhs.uuid != rhs.uuid;
    }

    std::ostream & operator<<(std::ostream &os, const rtti &info)
    {
        static const rtti::repo &mgr = rtti::repo::instance();
        os << ios::align(info.uuid,ios::align::left,mgr.mx);
        if(info.user.size)
        {
            os << ':';
            for(const rtti::alias *aka=info.user.head;aka;aka=aka->next)
            {
                os << ' ' << aka->name;
            }
        }
        return os;
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
    
    const rtti &  rtti:: repo:: update_mx(const rtti &info) throw()
    {
        aliasing::_(mx) = max_of(mx,info.ulen);
        return info;
    }


    const rtti & rtti:: repo:: operator()(const std::type_info &tid)
    {
        const string         key = tid.name();
        const rtti::pointer *ppr = db.search(key);
        const rtti::pointer *ppa = id.search(key);
        if(ppr)
        {
            //__________________________________________________________________
            //
            // primary type already registered
            //__________________________________________________________________
            if(!ppa)         throw exception("%s(<%s> corrupted)",call_sign,*key);
            if(*ppa != *ppr) throw exception("%s(existing <%s> aliases by <%s>)",call_sign,*key, * (**ppa).uuid);
            return **ppr;
        }
        else
        {
            //__________________________________________________________________
            //
            // register a new primary type
            //__________________________________________________________________
            if(ppa)
                throw exception("%s(missing <%s> aliased by <%s>",call_sign,*key,* (**ppa).uuid );
            
            const rtti::pointer ptr = new rtti(tid);
            if(!aliasing::_(db).insert(ptr))
                throw exception("%s(<%s> unexpected insert failure)",call_sign,*key);
           
            try
            {
                if(!aliasing::_(id).insert(key,ptr))
                    throw exception("%s(<%s> unexpected aliasing failure)",call_sign,*key);
            }
            catch(...)
            {
                (void) aliasing::_(db).remove(key);
                throw;
            }
            
            return update_mx(*ptr);
        }
    }

    const rtti & rtti:: repo:: operator()(const std::type_info &tid, const string &tag)
    {
        const rtti          &info = (*this)(tid);
        const rtti::pointer *ppa  = id.search(tag);
        if(ppa)
        {
            //__________________________________________________________________
            //
            // tag already exist!
            //__________________________________________________________________
            if (**ppa != info)
                throw exception("%s(<%s> cannot be a.k.a. '%s' used for <%s>)",call_sign,*info.uuid,*tag, *(**ppa).uuid);
            return info;
        }
        else
        {
            //__________________________________________________________________
            //
            // new tag => alias for info
            //__________________________________________________________________
            aliasing::_(info.user).push_back( new alias(tag) );
            try
            {
                const rtti::pointer ptr = & aliasing::_(info);
                if(!aliasing::_(id).insert(tag,ptr))
                    throw exception("%s(<%s> aliasing failure by '%s')", call_sign, *info.uuid, *tag);
            }
            catch(...)
            {
                delete aliasing::_(info.user).pop_back();
                throw;
            }
            return info;
        }
        
        
    }

    const rtti & rtti:: repo:: operator()(const std::type_info &tid, const char  *tag)
    {
        const string _(tag);
        return (*this)(tid,_);
    }





}


