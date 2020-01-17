
#include "y/type/spec.hpp"
#include "y/longevities.hpp"
#include "y/memory/pooled.hpp"
#include "y/exception.hpp"
#include "y/hashing/type-info.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{


    void type_spec:: setName()
    {
        string &id = aliasing::_(name);
        id = name_;
    }

    type_spec :: ~type_spec() throw()
    {

    }

    

    type_spec :: type_spec( const std::type_info &i ) :
    info(i),
    name_( info.name() ),
    name()
    {
    }

    const std::type_info & type_spec:: key() const throw()
    {
        return info;
    }


    class type_specs : public singleton<type_specs>
    {
    public:
        typedef intr_ptr<std::type_info,type_spec> type_spec_ptr;

        typedef hashing::type_info_hasher<>                     tHasher;    //!< dedicated hasher
        typedef memory::pooled                                   mAlloc;
        typedef set<std::type_info,type_spec_ptr,tHasher,mAlloc> dataBase;  //!< set of specs
        static const at_exit::longevity life_time = longevity_for::memory_pooled - 2;

        inline const type_spec & decl_( const std::type_info &tid )
        {
            const type_spec_ptr *ppts = db.search(tid);
            if(ppts)
            {
                return **ppts;
            }
            else
            {
                type_spec          *ts  = new type_spec(tid);
                const type_spec_ptr pts = ts;
                if(!db.insert(pts)) throw exception( "unexpected multiple type spec <%s>", *(ts->name) );
                return *ts;
            }
        }




    private:
        Y_DISABLE_COPY_AND_ASSIGN(type_specs);
        friend class singleton<type_specs>;
        inline explicit type_specs() : singleton<type_specs>(), db(32,as_capacity)
        {
        }

        inline virtual ~type_specs() throw() {}


        dataBase db;
    };


    
    const type_spec & type_spec:: of( const std::type_info &tid )
    {
        static type_specs &tss = type_specs::instance();
        return tss.decl_(tid);
    }


    
}


