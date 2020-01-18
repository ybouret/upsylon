
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

#define Y_TYPE_SPEC(KNOWN) do {                          \
const string          known = #KNOWN;                    \
const std::type_info &tid   = typeid(KNOWN);             \
if(!search(tid)) {                                       \
const type_spec_pointer tmp = new type_spec(tid,known);  \
if(!insert(tmp)) throw exception(errfmt,*(tmp->name)); } \
} while(false)

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
                        return *ts;
                    }
                }




            private:
                Y_DISABLE_COPY_AND_ASSIGN(type_specs);
                friend class singleton<type_specs>;
                inline explicit type_specs() : singleton<type_specs>(), type_spec_db(32,as_capacity)
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

                }

                inline virtual ~type_specs() throw() {}

            };
        }


        const type_spec & type_spec:: of( const std::type_info &tid )
        {
            static type_specs &tss = type_specs::instance();
            return tss.decl_(tid);
        }



    }


