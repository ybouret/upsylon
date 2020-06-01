
//! \file
#ifndef Y_FS_FIND_INCLUDED
#define Y_FS_FIND_INCLUDED

#include "y/fs/vfs.hpp"
#include "y/ptr/arc.hpp"
#include "y/core/inode.hpp"
#include "y/core/list.hpp"

namespace upsylon {
    
    struct fs_find
    {
        class look_up : public object, public inode<look_up>
        {
        public:
            typedef core::list_of_cpp<look_up> list;
            
            template <typename LABEL> inline
            explicit look_up(const LABEL &_dpath, const int  _depth) :
            dpath(_dpath),
            depth(_depth)
            {
            }
            
            virtual ~look_up() throw();
            
            const string dpath;
            const int    depth;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(look_up);
        };
        
        class directories : public look_up::list
        {
        public:
            explicit directories() throw();
            virtual ~directories() throw();
            
            template <typename LABEL> inline
            void add(const LABEL &dpath, const int depth)
            {
                push_back( new look_up(dpath,depth) );
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(directories);
        };
        
        template <typename LABEL, typename FUNC> static inline
        void in(vfs         &fs,
                const LABEL &dirname,
                FUNC        &func,
                const int    max_depth)
        {
            const bool  halt = (max_depth>=0);
            directories work;
            work.add(dirname,0);
            while(work.size>0)
            {
                auto_ptr<look_up>      node  = work.pop_front();
                auto_ptr<vfs::scanner> scan  = fs.scan(node->dpath);
                const int              below = node->depth+1;
                const bool             stop  = halt && below>max_depth;
                const bool             go    = !stop;
                for(const vfs::entry *ep = scan->next(); ep; ep=scan->next())
                {
                    if(ep->is_dot_or_ddot()) continue;
                    if(ep->link)             continue;
                    switch(ep->attr)
                    {
                        case vfs::entry::is_dir: if(go) work.add(ep->path,below); // FALLTHRU
                        case vfs::entry::is_reg: func(*ep);
                            break;
                        
                        default:
                            break;
                    }
                }
            }
        }
        
    };
    
}

#endif

