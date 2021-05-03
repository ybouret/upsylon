
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

}


namespace upsylon
{
    rtti :: rtti(const std::type_info &tid) :
    object(),
    counted(),
    suffix_address(*this),
    uuid( tid.name() ),
    user()
    {
        
    }

    
    rtti:: ~rtti() throw()
    {
        
    }

    const string & rtti:: name() const throw()
    {
        return (user.size>0) ?  user.head->name : uuid;
    }

    const char * rtti:: text() const throw()
    {
        return *name();
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
                os << ' ' << '[' << aka->name << ']';
            }
        }
        return os;
    }
    
    const rtti & rtti::of(const std::type_info &tid)
    {
        static repo &mgr = repo::instance();
        return mgr(tid);
    }
    
    const string  & rtti:: name_of(const std::type_info &tid)
    {
        static repo &mgr = repo::instance();
        return mgr(tid).name();
    }
    

    string rtti:: to_file_name(const string &source)
    {
        const size_t  len = source.size();
        string        target(len,as_capacity,false);
        for(size_t i=0;i<source.size();++i)
        {
            const char C = source[i];
            switch(C)
            {
                case '_':
                case '>':
                case '<':
                case ' ':
                case '.':
                    if(i+1<len)
                        target << '_';
                    break;
                    
                default:
                    target << C;
                    break;
            }
            
        }
        
        std::cerr << "[" << source << "] => [" << target <<  "]" << std::endl;
        return target;
    }
    
}


#include "y/exception.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    Y_SINGLETON_IMPL(rtti::repo);

    rtti:: repo:: repo() : db(), id(), mx(0)
    {
        import();
    }

    rtti:: repo:: ~repo() throw()
    {
    }
    
    const rtti &  rtti:: repo:: update_mx(const rtti &info) throw()
    {
        aliasing::_(mx) = max_of(mx,info.length());
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


#include "y/type/complex.hpp"
#include "y/type/point3d.hpp"
#include "y/yap.hpp"

namespace upsylon
{
#define Y_RTTI(NAME) use<NAME>( #NAME )
    
    
    void rtti:: repo:: import()
    {
        Y_RTTI(int8_t);
        Y_RTTI(int16_t);
        Y_RTTI(int32_t);
        Y_RTTI(int64_t);
        
        Y_RTTI(uint8_t);
        Y_RTTI(uint16_t);
        Y_RTTI(uint32_t);
        Y_RTTI(uint64_t);
        
        Y_RTTI(char);
        Y_RTTI(short);
        Y_RTTI(int);
        Y_RTTI(long);
        Y_RTTI(long long);
        Y_RTTI(unit_t);
        
        Y_RTTI(unsigned char);
        Y_RTTI(unsigned short);
        Y_RTTI(unsigned int);
        Y_RTTI(unsigned long);
        Y_RTTI(unsigned long long);
        Y_RTTI(size_t);

        
        Y_RTTI(float);
        Y_RTTI(double);
        Y_RTTI(long double);

        Y_RTTI(string);
        
        Y_RTTI(complex<float>);
        Y_RTTI(complex<double>);
        
        Y_RTTI(point2d<float>);
        Y_RTTI(point2d<double>);
        Y_RTTI(point2d<unit_t>);

        Y_RTTI(point3d<float>);
        Y_RTTI(point3d<double>);
        Y_RTTI(point3d<unit_t>);

        Y_RTTI(apn);
        Y_RTTI(apz);
        Y_RTTI(apq);
        
    }
}
