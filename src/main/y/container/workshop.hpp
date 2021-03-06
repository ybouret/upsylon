//! \file

#ifndef Y_WORKSHOP_INCLUDED
#define Y_WORKSHOP_INCLUDED 1

#include "y/container/factory.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! workshop = factory<T,string,...> + utils
    //
    //__________________________________________________________________________
    template <
    typename T,
    typename CREATOR    = T * (*)(),
    typename KEY_HASHER = key_hasher<string>
    >
    class workshop : public factory<T,string,CREATOR,KEY_HASHER>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef factory<T,string,CREATOR,KEY_HASHER> factory_type; //!< alias
        typedef typename factory_type::creator_type  creator_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~workshop() throw() {}                          //!< cleanup
        inline explicit workshop() : factory_type() {}                 //!< setup
        inline explicit workshop(const size_t n) : factory_type(n) {}  //!< setup with capacity

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! automatic insertion
        inline void declare(const string &key, const CREATOR &creator)
        {
            if(!this->insert(key,creator))
            {
                throw exception("%s<%s> use(multiple '%s')",this->CLID,*(this->typeName),*key);
            }
        }

        //! automatic insertion wrapper
        inline void declare(const char *key, const CREATOR &creator)
        {
            const string _(key); declare(_,creator);
        }

        //! helper: create default object
        inline T * operator()(const string &key) const
        {
            const CREATOR *pCreator = this->search(key);
            if(!pCreator)
            {
                throw exception("%s<%s>(no '%s')",this->CLID,*(this->typeName),*key);
            }
            return (*pCreator)();
        }

        //! helper: create default object wrapper
        inline T * operator()(const char *key) const
        {
            const string _(key); return (*this)(_);
        }



    private:
        Y_DISABLE_COPY_AND_ASSIGN(workshop);
    };

}

#endif
