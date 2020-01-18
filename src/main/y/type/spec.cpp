
#include "y/type/spec.hpp"
#include "y/longevities.hpp"
#include "y/memory/pooled.hpp"
#include "y/exception.hpp"
#include "y/hashing/type-info.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/intr.hpp"
#include "y/type/complex.hpp"
#include "y/string/display.hpp"
#include "y/mpl/rational.hpp"

namespace upsylon {


    void type_spec:: setName()
    {
        string &id = aliasing::_(name);
        id = name_;
    }

    type_spec :: ~type_spec() throw() {}



    type_spec :: type_spec( const std::type_info &i ) :
    info(i),
    name_( info.name() ),
    name()
    {
        setName();
    }

    type_spec :: type_spec( const std::type_info &i, const string &known) :
    info(i),
    name_( info.name() ),
    name( known )
    {
    }

    const std::type_info & type_spec:: key() const throw()
    {
        return info;
    }


    namespace {

        static const char errfmt[] = "upsylon: unexpected multiple type spec <%s>";



        typedef intr_ptr<std::type_info,type_spec> type_spec_pointer; //!< for set
        typedef hashing::type_info_hasher<>        type_info_hasher;  //!< dedicated hasher
        typedef memory::pooled                     type_spec_memory;  //!< memory
        typedef set<
        std::type_info,
        type_spec_pointer,
        type_info_hasher,
        type_spec_memory> type_spec_db;

        class type_specs : public singleton<type_specs>, public type_spec_db
        {
        public:

            static const at_exit::longevity life_time = longevity_for::memory_pooled - 2;
            size_t sys_name_max;
            size_t usr_name_max;

            inline const type_spec & decl_( const std::type_info &tid )
            {
                Y_LOCK(access);
                const type_spec_pointer *ppts = this->search(tid);
                if(ppts)
                {
                    return **ppts;
                }
                else
                {
                    type_spec              *ts  = new type_spec(tid);
                    const type_spec_pointer pts = ts;
                    if(!insert(pts)) throw exception(errfmt,*(ts->name));
                    update_with(*ts);
                    return *ts;
                }
            }

            void known_(const std::type_info &tid,
                        const string         &known )
            {
                type_spec_pointer *ppts = this->search(tid);
                if(ppts)
                {
                    // multiple ID with different names
                    type_spec &ts = **ppts;
                    aliasing::_(ts.name) << '/' << known;
                    update_with(ts);
                }
                else
                {
                    // new ID
                    const type_spec_pointer tmp = new type_spec(tid,known);
                    if(!insert(tmp)) throw exception(errfmt,*(tmp->name));
                    update_with(*tmp);
                }
            }

            static inline
            int compare_type_spec_by_name(const type_spec_pointer &lhs,
                                          const type_spec_pointer &rhs) throw()
            {
                return string::compare(lhs->name_, rhs->name_);
            }

            void update_with( const type_spec &ts ) throw()
            {
                sys_name_max = max_of(ts.name_.size(),sys_name_max);
                usr_name_max = max_of(ts.name.size(), usr_name_max);

                sort_data(compare_type_spec_by_name);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(type_specs);
            friend class singleton<type_specs>;

#define Y_TYPE_SPEC(KNOWN) do {                          \
const string          known = #KNOWN;                    \
const std::type_info &tid   = typeid(KNOWN);             \
known_(tid,known);                                       \
} while(false)

            inline explicit type_specs() :
            singleton<type_specs>(),
            type_spec_db(64,as_capacity),
            sys_name_max(0),
            usr_name_max(0)
            {
                Y_LOCK(access);
                Y_TYPE_SPEC(float);
                Y_TYPE_SPEC(double);

                Y_TYPE_SPEC(uint8_t);
                Y_TYPE_SPEC(uint16_t);
                Y_TYPE_SPEC(uint32_t);
                Y_TYPE_SPEC(uint64_t);

                Y_TYPE_SPEC(int8_t);
                Y_TYPE_SPEC(int16_t);
                Y_TYPE_SPEC(int32_t);
                Y_TYPE_SPEC(int64_t);

                Y_TYPE_SPEC(char);
                Y_TYPE_SPEC(short);
                Y_TYPE_SPEC(int);
                Y_TYPE_SPEC(long);
                Y_TYPE_SPEC(long long);

                Y_TYPE_SPEC(unsigned char);
                Y_TYPE_SPEC(unsigned short);
                Y_TYPE_SPEC(unsigned int);
                Y_TYPE_SPEC(unsigned long);
                Y_TYPE_SPEC(unsigned long long);

                Y_TYPE_SPEC(complex<float>);
                Y_TYPE_SPEC(complex<double>);

                Y_TYPE_SPEC(string);

                Y_TYPE_SPEC(mpn);
                Y_TYPE_SPEC(mpz);
                Y_TYPE_SPEC(mpq);
            }

            inline virtual ~type_specs() throw() {}

        };
    }


    const type_spec & type_spec:: of( const std::type_info &tid )
    {
        static type_specs &tss = type_specs::instance();
        return tss.decl_(tid);
    }

    void type_spec:: display(std::ostream &os)
    {
        static const type_specs &tss = type_specs::instance();

        os << "<type_specs count=\"" << tss.size() << "\">" << std::endl;
        for( type_spec_db::const_iterator i = tss.begin(); i != tss.end(); ++i)
        {
            const type_spec &ts = **i;
            string_display::align(os<<"\t",ts.name_,tss.sys_name_max) << " => ";
            string_display::align(os,      ts.name, tss.usr_name_max) << std::endl;
        }
        os << "<type_specs/>" << std::endl;
    }

}


