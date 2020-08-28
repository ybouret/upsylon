

#include "y/jive/pattern/posix.hpp"
#include "y/associative/suffix/tree.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Jive {

        namespace {

            typedef Pattern * (*creator)();

            class PosixDB :
            public singleton<PosixDB>,
            public suffix_tree<creator>
            {
            public:


            private:

                inline void record(const char *name, creator proc)
                {
                    assert(proc);
                    if(!insert_by(name, length_of(name),proc))
                    {
                        throw exception("Jive::PosixDB multiple name");
                    }
                }

#define Y_JIVE_POSIX(NAME) record( #NAME, posix::NAME )

                inline explicit  PosixDB()
                {
                    Y_JIVE_POSIX(lower);
                    Y_JIVE_POSIX(upper);
                    Y_JIVE_POSIX(alpha);
                    Y_JIVE_POSIX(digit);
                    Y_JIVE_POSIX(alnum);
                    Y_JIVE_POSIX(xdigit);
                    Y_JIVE_POSIX(blank);
                    Y_JIVE_POSIX(space);
                    Y_JIVE_POSIX(punct);

                    Y_JIVE_POSIX(word);
                    Y_JIVE_POSIX(endl);
                    Y_JIVE_POSIX(dot);
                    Y_JIVE_POSIX(core);
                }

                inline virtual ~PosixDB() throw()
                {

                }

                Y_SINGLETON_DECL_WITH(memory::pooled::life_time-1,PosixDB);
            };


        }
        Y_SINGLETON_IMPL(Jive::PosixDB);

        bool posix:: exists(const string &id)
        {
            static const PosixDB &db = PosixDB::instance();
            Y_LOCK(db.access);
            return db.search_by(id);
        }

        bool posix:: exists(const char *id)
        {
            const string _(id);
            return exists(_);
        }


        Pattern *posix:: create(const string &id)
        {
            static const PosixDB &db = PosixDB::instance();
            Y_LOCK(db.access);
            const creator *proc = db.search_by(id);
            if(!proc) throw exception("no posix::%s", *id);
            assert(*proc);
            return (*proc)();
        }

        Pattern *posix:: create(const char *id)
        {
            const string _(id);
            return create(_);
        }

        const Vizible & posix::root()
        {
            static const PosixDB &db = PosixDB::instance();
            return db.get_root();
        }
    }

}


