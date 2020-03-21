#include "y/jargon/pattern/posix.hpp"
#include "y/associative/suffix-tree.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {

        namespace {
         
#define Y_JPOSIX(EXPR) do {        \
const char    key[] = #EXPR;       \
const creator arg   = posix::EXPR; \
if( !insert_with(key,sizeof(key)/sizeof(key[0])-1,arg) )\
throw exception(fmt,key);\
} while(false)
            
            typedef Pattern * (*creator)();
            static const char fmt[] = "unexpected multiple %s";
            
            class posix_db :
            public singleton<posix_db>,
            public suffix_tree<creator>
            {
            public:
                
            private:
                virtual ~posix_db() throw() {}
                inline   posix_db() : suffix_tree<creator>()
                {
                    Y_JPOSIX(lower);
                    Y_JPOSIX(upper);
                    Y_JPOSIX(alpha);
                    Y_JPOSIX(digit);
                    Y_JPOSIX(alnum);
                    Y_JPOSIX(xdigit);
                    Y_JPOSIX(blank);
                    Y_JPOSIX(space);
                    Y_JPOSIX(punct);
                    
                    Y_JPOSIX(word);
                    Y_JPOSIX(endl);
                    Y_JPOSIX(dot);
                    Y_JPOSIX(core);

                }
                
                friend class singleton<posix_db>;
                static const at_exit::longevity life_time = 0;
                
            };
            
           
        }
        
        Pattern * posix::get(const string &id)
        {
            const posix_db &db = posix_db::instance();
            const creator  *cb = db.look_for(*id, id.size());
            if(!cb) throw exception("no posix::%s",*id);
            return (*cb)();
        }
        
        Pattern * posix::get(const char *id)
        {
            const string _(id);
            return get(_);
        }
        
        Pattern * posix::query(const string &id)
        {
            const posix_db &db = posix_db::instance();
            const creator  *cb = db.look_for(*id, id.size());
            if( !cb )
            {
                return NULL;
            }
            else
            {
                return (*cb)();
            }
        }
        
    }
    
}

