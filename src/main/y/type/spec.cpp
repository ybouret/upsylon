#include "y/type/spec.hpp"
#include "y/type/aliasing.hpp"


namespace upsylon {

    type_spec::alias:: ~alias() throw()
    {
    }

    type_spec:: alias:: alias( const string &id ) :
    next(0), prev(0), name(id) {}
    

    type_spec:: ~type_spec() throw()
    {

    }


#define Y_TYPE_SPEC_CTOR() \
info( tid ),               \
uuid( tid.name() ),        \
user()

    type_spec:: type_spec( const std::type_info &tid ) :
    Y_TYPE_SPEC_CTOR()
    {

    }

    type_spec:: type_spec(const std::type_info &tid, const string &known) :
    Y_TYPE_SPEC_CTOR()
    {
        aka(known);
    }

    type_spec:: type_spec(const std::type_info &tid, const char *known) :
    Y_TYPE_SPEC_CTOR()
    {
        aka(known);
    }


    bool type_spec:: aka( const string &usr )
    {
        for(const alias *scan=user.head; scan; scan=scan->next)
        {
            if(usr==scan->name) return true;
        }
        aliasing::_(user).push_back( new alias(usr) );
        return false;
    }

    bool type_spec:: aka(const char *usr)
    {
        const string _(usr); return aka(_);
    }

    const string & type_spec:: name() const throw()
    {
        if( user.has_nodes() )
        {
            return user.head->name;
        }
        else
        {
            return uuid;
        }
    }

    const std::type_info & type_spec:: key() const throw()
    {
        return info;
    }


}

#include "y/associative/set.hpp"
#include "y/associative/map.hpp"
#include "y/hashing/type-info.hpp"
#include "y/ptr/intr.hpp"
#include "y/memory/pooled.hpp"
#include "y/exception.hpp"
#include "y/string/display.hpp"

#include "y/type/complex.hpp"
#include "y/mpl/rational.hpp"

namespace upsylon {

    typedef intr_ptr<std::type_info,type_spec> type_spec_pointer;
    typedef hashing::type_info_hasher<>        type_info_hasher;
    typedef memory::pooled                     type_spec_memory;
    typedef set<
    std::type_info,
    type_spec_pointer,
    type_info_hasher,
    type_spec_memory> type_spec_db;

    typedef map<
    string,
    type_spec_pointer,
    key_hasher<string>,
    type_spec_memory> type_name_db;


    class type_specs : public singleton<type_specs>, public type_spec_db
    {
    public:
        type_name_db dict;
        size_t       max_uuid;

        type_spec & decl( const std::type_info &tid )
        {
            type_spec_pointer *pp = search(tid);
            if(pp)
            {
                //--------------------------------------------------------------
                // already declared
                //--------------------------------------------------------------
                assert( tid.name() == (*pp)->uuid );
                return **pp;
            }
            else
            {
                //--------------------------------------------------------------
                // first creation
                //--------------------------------------------------------------
                type_spec              *ptr = new type_spec( tid );
                const type_spec_pointer tsp = ptr;
                if(!insert(tsp)) throw exception("unexpected insert failure");
                try
                {
                    if( !dict.insert(tsp->uuid,tsp) )
                    {
                        throw exception("unexpected multiple uuid <%s>", *(tsp->uuid) );
                    }
                }
                catch(...)
                {
                    no(tid);
                    throw;
                }
                max_uuid = max_of(max_uuid,tsp->uuid.size());
                return *ptr;
            }
        }

        type_spec & name( const std::type_info &tid, const string &known )
        {
            type_spec &ts = decl(tid);
            if(!ts.aka(known))
            {
                assert( known == ts.user.tail->name );
                try
                {
                    const type_spec_pointer tsp = &ts;
                    if( ! dict.insert(known,tsp) )
                    {
                        throw exception("unexpected multiple alias <%s> for <%s>", *known, *ts.uuid);
                    }
                }
                catch(...)
                {
                    delete aliasing::_(ts.user).pop_back();
                    throw;
                }
            }
            return ts;
        }

        inline void show( std::ostream &os ) const
        {
            os << "<type_specs count=\"" << size() << "\">" << std::endl;
            for(const_iterator it=begin();it!=end();++it)
            {
                os << **it << std::endl;
            }
            os << "<type_specs/>" << std::endl;
        }

        inline bool is( const type_spec &t, const string &name ) const throw()
        {
            const type_spec_pointer *pp = dict.search(name);
            return ( pp && ( t.info == (**pp).info ) );
        }

        inline bool is( const type_spec &t, const char *name ) const
        {
            const string _(name);
            return is(t,_);
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(type_specs);
        static const at_exit::longevity life_time = 0;

        friend class singleton<type_specs>;

        inline virtual ~type_specs() throw() {}

#define TS(TYPE) (void) name( typeid(TYPE), #TYPE )

        inline explicit type_specs() :
        type_spec_db(32,as_capacity),
        dict(32,as_capacity),
        max_uuid(0)
        {
            TS(uint8_t);
            TS(uint16_t);
            TS(uint32_t);
            TS(uint64_t);

            TS(int8_t);
            TS(int16_t);
            TS(int32_t);
            TS(int64_t);


            TS(char);
            TS(unsigned char);

            TS(short);
            TS(unsigned short);

            TS(int);
            TS(unsigned int);

            TS(long);
            TS(unsigned long);

            TS(long long);
            TS(unsigned long long);

            TS(float);
            TS(double);

            TS(string);

            TS(complex<float>);
            TS(complex<double>);

            TS(mpn);
            TS(mpz);
            TS(mpq);
        }




    };


    std::ostream & operator<<(std::ostream &os, const type_spec &t)
    {
        static const type_specs &ts = type_specs::instance();
        string_display::align(os,t.uuid,ts.max_uuid) << " :";
        for( const type_spec::alias *a = t.user.head; a; a=a->next )
        {
            os << ' ' << a->name;
        }
        return os;
    }


    const type_spec & type_spec:: declare( const std::type_info &tid )
    {
        static type_specs &ts = type_specs::instance();
        return ts.decl(tid);
    }

    const type_spec &  type_spec:: aka(const std::type_info &tid, const string &known)
    {
        static type_specs &ts = type_specs::instance();
        return ts.name(tid,known);
    }

    const type_spec &  type_spec:: aka(const std::type_info &tid, const char *known)
    {
        const string _(known); return aka(tid,_);
    }


    void type_spec:: display( std::ostream &os)
    {
        type_specs::instance().show(os);
    }

    bool operator==(const type_spec &lhs, const type_spec &rhs ) throw()
    {
        return lhs.info == rhs.info;
    }

    bool operator==(const type_spec &lhs, const string &rhs)
    {
        static const type_specs &tss = type_specs::instance();
        return tss.is(lhs,rhs);
    }

    bool operator==(const type_spec &lhs, const char *rhs )
    {
        static const type_specs &tss = type_specs::instance();
        return tss.is(lhs,rhs);
    }

    bool operator==(const string &lhs, const type_spec &rhs)
    {
        static const type_specs &tss = type_specs::instance();
        return tss.is(rhs,lhs);
    }

    bool operator==(const char *lhs, const type_spec &rhs)
    {
        static const type_specs &tss = type_specs::instance();
        return tss.is(rhs,lhs);
    }




}
