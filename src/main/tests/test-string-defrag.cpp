
#include "y/string/defrag.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "support.hpp"
#include "y/memory/pooled.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/intr.hpp"

using namespace upsylon;

namespace
    {

        class noise
        {
        public:
            static const size_t extent = 1000;

            vector<string> content;

            noise() : content( extent, as_capacity  )
            {
                std::cerr << "\t** tadding noise" << std::endl;
                for(size_t i=0;i<extent;++i)
                {
                    const string s( alea.leq(100), as_capacity, false);
                    content.push_back(s);
                }
            }

            ~noise()
            {
                std::cerr << "\t** remove noise" << std::endl;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(noise);

        };

        template <typename SEQUENCE>
        static inline
        void do_defrag( string_defrag &SD, SEQUENCE &strings )
        {
            std::cerr << "defrag sequence of " << strings.size() << std::endl;
            const size_t nd = SD.cycle_on(strings);
            std::cerr << "could do " << nd << std::endl;
        }

        class kDummy : public counted_object
        {
        public:
            typedef intr_ptr<string,kDummy> Ptr;
            const string name;


            const string & key() const throw() { return name; }

            kDummy(const string &id ) : counted_object(), name(id)
            {
            }

            virtual ~kDummy() throw()
            {
            }

            kDummy(const kDummy &_) : counted_object(), name(_.name) {}

            Y_DISABLE_ASSIGN(kDummy);


        };
    }

Y_UTEST(string_defrag)
{
    const size_t nv = 20;
    const size_t nl = 30;
    vector<string>                vs(nv,as_capacity);
    vector<string,memory::pooled> ps(nv,as_capacity);

    list<string>   ls(nl,as_capacity);

    {
        volatile noise space;

        for(size_t i=0;i<nl;++i)
        {
            const string tmp = support::get<string>();
            ls.push_back(tmp);
        }

        for(size_t i=0;i<nv;++i)
        {
            const string tmp = support::get<string>();
            vs.push_back(tmp);
        }

        for(size_t i=0;i<nv;++i)
        {
            const string tmp = support::get<string>();
            ps.push_back(tmp);
        }

    }

    string_defrag SD;

    std::cerr << "-- global vector" << std::endl;
    do_defrag(SD,vs);
    vs.release();

    std::cerr << "-- list" << std::endl;
    do_defrag(SD,ls);
    ls.release();

    std::cerr << "-- pooled vector" << std::endl;
    do_defrag(SD,ps);
    ps.release();


    size_t ndb = 25;
    {

        set<string,kDummy> db;
        {
            volatile noise space;
            std::cerr << "adding keys" << std::endl;
            for(size_t i=0;i<ndb;++i)
            {
                const string id = support::get<string>();
                const kDummy tmp(id);
                (void) db.insert(tmp);
            }
        }

        std::cerr << "-- standard set " << db.size() << std::endl;
        const size_t nd = SD.cycle_on_keys(db);
        std::cerr << "\tdefrag " << nd << std::endl;
    }


    {
        set<string,kDummy::Ptr> db;
        {
            volatile noise space;
            std::cerr << "adding keys" << std::endl;
            for(size_t i=0;i<ndb;++i)
            {
                const string id = support::get<string>();
                const kDummy::Ptr tmp = new kDummy(id);
                (void) db.insert(tmp);
            }
        }

        std::cerr << "-- shared set " << db.size() << std::endl;
        const size_t nd = SD.cycle_on_keys(db);
        std::cerr << "\tdefrag " << nd << std::endl;
    }

}
Y_UTEST_DONE()

