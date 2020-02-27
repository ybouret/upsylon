#include "y/type/spec.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    type_spec::alias:: ~alias() throw() {}

    type_spec:: alias:: alias(const string &id) : next(0), prev(0), name(id) {}
    

    type_spec:: ~type_spec() throw() {}


#define Y_TYPE_SPEC_CTOR() \
info( tid ),               \
uuid( tid.name() ),        \
user() 

    type_spec:: type_spec(const std::type_info &tid) : Y_TYPE_SPEC_CTOR() { }

    type_spec:: type_spec(const std::type_info &tid,
                          const string         &known) :
    Y_TYPE_SPEC_CTOR()
    {
        (void)aka(known);
    }

    type_spec:: type_spec(const std::type_info &tid,
                          const char           *known) :
    Y_TYPE_SPEC_CTOR()
    {
        (void)aka(known);
    }


    bool type_spec:: aka( const string &usr )
    {
        if( usr == uuid ) {
            return true;
        }
        else {
            for(const alias *scan=user.head; scan; scan=scan->next)
            {
                if(usr==scan->name) return true;
            }
            aliasing::_(user).push_back( new alias(usr) );
            return false;
        }
    }

    bool type_spec:: aka(const char *usr)
    {
        const string _(usr); return aka(_);
    }

    const string & type_spec:: name() const throw()
    {
#if defined(_MSC_VER)
        return uuid;
#else
        if( user.has_nodes() )
        {
            return user.head->name;
        }
        else
        {
            return uuid;
        }
#endif

    }

    const std::type_info & type_spec:: key() const throw()
    {
        return info;
    }


}

#include "y/associative/set.hpp"
#include "y/associative/map.hpp"
#include "y/ptr/intr.hpp"
#include "y/memory/pooled.hpp"
#include "y/exception.hpp"
#include "y/string/display.hpp"
#include "y/hashing/fnv.hpp"

namespace upsylon {

    namespace {
        
        class type_info_hasher
        {
        public:
            inline  type_info_hasher() throw() : H() {}
            inline ~type_info_hasher() throw() {}

            hashing::fnv H;

            inline size_t operator()( const std::type_info &tid ) throw()
            {
                H.set();
                H(tid.name());
                return H.key<size_t>();
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(type_info_hasher);
        };

    }

    typedef intr_ptr<std::type_info,type_spec> type_spec_pointer;
    typedef memory::pooled                     type_spec_memory;
    typedef set<
    std::type_info,
    type_spec_pointer,
    type_info_hasher,
    type_spec_memory>                          type_spec_db;

    typedef map<
    string,
    type_spec_pointer,
    key_hasher<string>,
    type_spec_memory> type_name_db;


    static const char fn[] = "type_spec";

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
                if(!insert(tsp)) throw exception("%s: nexpected insert failure",fn);

                //--------------------------------------------------------------
                // insert uuid in dictionary
                //--------------------------------------------------------------
                try
                {
                    if( !dict.insert(tsp->uuid,tsp) )
                    {
                        throw exception("%s: unexpected multiple uuid <%s>", fn, *(tsp->uuid) );
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
                        throw exception("%s: unexpected multiple alias <%s> for <%s>", fn, *known, *ts.uuid);
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
        static const at_exit::longevity life_time;

        friend class singleton<type_specs>;

        inline virtual ~type_specs() throw() {}

        void initialize();


        inline explicit type_specs() :
        type_spec_db(64,as_capacity),
        dict(64,as_capacity),
        max_uuid(0)
        {
            initialize();
        }

    };

    const at_exit::longevity type_specs::life_time =memory::pooled::life_time-2;


    std::ostream & operator<<(std::ostream &os, const type_spec &t)
    {
        static const char        prefix[] = "[";
        static const char        suffix[] = "]";
        static const type_specs &tss      = type_specs::instance();
        string_display::align(os,t.uuid,tss.max_uuid,prefix,suffix) << " :";
        for( const type_spec::alias *a = t.user.head; a; a=a->next )
        {
            os << ' ' << prefix << a->name << suffix;
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
        static type_specs &tss = type_specs::instance();
        return tss.name(tid,known);
    }

    const type_spec &  type_spec:: aka(const std::type_info &tid, const char *known)
    {
        const string _(known); return aka(tid,_);
    }

    const type_spec & type_spec:: query(const string &id)
    {
        static const type_specs &tss = type_specs::instance();
        const type_spec_pointer *pp  = tss.dict.search(id);
        if(!pp)
        {
            throw exception("no upsylon::type_spec<%s>", *id );
        }
        return **pp;
    }

    const type_spec & type_spec:: query(const char *id)
    {
        const string _(id); return query(_);
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

#include "y/type/complex.hpp"
#include "y/mpl/rational.hpp"
#include "y/type/point3d.hpp"


namespace upsylon {

#define TS(TYPE) (void) name( typeid(TYPE), #TYPE )

    void type_specs::initialize()
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
        TS(unsigned);

        TS(long);
        TS(unsigned long);

        TS(long long);
        TS(unsigned long long);

        TS(float);
        TS(double);
        TS(long double);

        TS(bool);

        TS(string);

        TS(complex<float>);
        TS(complex<double>);
        TS(complex<long double>);

        TS(mpn);
        TS(mpz);
        TS(mpq);

        TS(size_t);
        TS(unit_t);

        TS(point2d<float>);
        TS(point2d<double>);
        TS(point2d<unit_t>);

        TS(point3d<float>);
        TS(point3d<double>);
        TS(point3d<unit_t>);
    }

}
