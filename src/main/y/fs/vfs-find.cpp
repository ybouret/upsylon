#include "y/fs/vfs.hpp"
#include "y/sequence/pipe.hpp"

namespace upsylon {

    namespace {

        class lookUp
        {
        public:
            typedef lstack<lookUp> Stack;

            const string dname;
            const int    depth;


            inline lookUp(const string &id,
                          const int     n) : dname(id), depth(n)
            {
            }

            inline ~lookUp() throw()
            {
            }

            inline lookUp(const lookUp &other) : dname(other.dname), depth(other.depth)
            {
            }

            static inline
            void push( Stack &work, const string &_dname,const int _depth)
            {
                const lookUp tmp(_dname,_depth);
                work.push(tmp);
            }

#if 0
            inline friend std::ostream & operator<<( std::ostream &os, const lookUp &l)
            {
                os << '<' << l.dname << '>' << '@' << l.depth;
                return os;
            }
#endif
            
        private:
            Y_DISABLE_ASSIGN(lookUp);
        };


    }

    void vfs::find(const char   *dirname,
                   find_proc     proc,
                   void         *args,
                   int           max_depth) const
    {
        const string _(dirname);
        find(_,proc,args,max_depth);
    }

    void vfs::find(const string &dirname,
                     find_proc     proc,
                     void         *args,
                     int           max_depth) const
    {
        const bool    stop = (max_depth>=0);
        const bool    call = (0!=proc);
        lookUp::Stack work;

        // initialize
        lookUp::push(work, dirname, 0);

        // non-recursive work
        while( work.size() )
        {
            const lookUp todo = work.peek();
            work.pop();

            auto_ptr<vfs::scanner> dscan  = scan(todo.dname);
            const int              depth  = todo.depth;
            const int              below  = depth+1;
            for(const vfs::entry  *ep = dscan->next(); ep; ep=dscan->next() )
            {
                if( ep->is_dot_or_ddot() ) continue;
                if(ep->link)               continue;
                switch(ep->attr)
                {
                    case entry::is_reg:
                        if(call)
                        {
                            proc(*ep,args);
                        }
                        break;

                    case entry::is_dir:
                        if(stop&&depth>=max_depth)
                        {
                            // do nothing
                        }
                        else
                        {
                            // store
                            lookUp::push(work,ep->path,below);
                        }
                        break;

                    default:
                        break;
                }
            }

        }
        
    }

}
