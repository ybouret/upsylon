
//! \file
#ifndef Y_FS_FIND_INCLUDED
#define Y_FS_FIND_INCLUDED

#include "y/fs/vfs.hpp"
#include "y/ptr/arc.hpp"
#include "y/core/inode.hpp"
#include "y/core/list.hpp"

namespace upsylon {

    //--------------------------------------------------------------------------
    //
    //! File System find algorithm
    //
    //--------------------------------------------------------------------------
    struct fs_find
    {
        //----------------------------------------------------------------------
        //
        //! look up directory: name+depth
        //
        //----------------------------------------------------------------------
        class look_up : public object, public inode<look_up>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef core::list_of_cpp<look_up> list; //!< base type for list

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! generic build
            template <typename LABEL> inline
            explicit look_up(const LABEL &_dpath, const int  _depth) :
            dpath(_dpath),
            depth(_depth)
            {
            }

            //! cleanup
            virtual ~look_up() throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string dpath; //!< directory path
            const int    depth; //!< directory depth
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(look_up);
        };

        //----------------------------------------------------------------------
        //
        //! list of directories to scan
        //
        //----------------------------------------------------------------------
        class directories : public look_up::list
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit directories() throw(); //!< setup
            virtual ~directories() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! add a new directory to scan
            template <typename LABEL> inline
            void add(const LABEL &dpath, const int depth)
            {
                push_back( new look_up(dpath,depth) );
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(directories);
        };

        //----------------------------------------------------------------------
        //
        //! non recursive algorithm
        //
        //----------------------------------------------------------------------
        template <typename LABEL, typename FUNC> static inline
        bool in(vfs         &fs,
                const LABEL &dirname,
                FUNC        &func,
                const int    max_depth)
        {
            //__________________________________________________________________
            //
            // initialize
            //__________________________________________________________________
            const bool  halt = (max_depth>=0);
            directories work;
            work.add(dirname,0);

            //__________________________________________________________________
            //
            // loop
            //__________________________________________________________________
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
                        case vfs::entry::is_reg: if( !func(*ep) ) return false;
                            break;
                        
                        default:
                            break;
                    }
                }
            }
            return true;
        }

        //! tell how many entries would be accepted
        template <typename LABEL, typename ACCEPT> static inline
        size_t tell(vfs         &fs,
                    const LABEL &dirname,
                    ACCEPT      &accept,
                    const int   max_depth)
        {

            tell_context<ACCEPT> ctx = { 0, &accept };
            if(!in(fs,dirname,ctx,max_depth)) return 0;
            return ctx.number;
        }

        //! push back accepted entries and return number of new entries
        template <typename LABEL, typename ACCEPT> static inline
        size_t collect(sequence<vfs::entry> &seq,
                       vfs                  &fs,
                       const LABEL          &dirname,
                       ACCEPT               &accept,
                       const int             max_depth)
        {

            collect_context<ACCEPT> ctx = { 0, &accept , &seq};
            if(!in(fs,dirname,ctx,max_depth)) return 0;
            return ctx.number;
        }

        //! push back accepted entries and return number of new entries
        template <typename LABEL, typename ACCEPT> static inline
        size_t collect(sequence<string    > &seq,
                       vfs                  &fs,
                       const LABEL          &dirname,
                       ACCEPT               &accept,
                       const int             max_depth)
        {

            collect_paths_context<ACCEPT> ctx = { 0, &accept , &seq};
            if(!in(fs,dirname,ctx,max_depth)) return 0;
            return ctx.number;
        }

        //! true
        static bool accept_any(const vfs::entry&) throw();

    private:
        template <typename ACCEPT> struct tell_context
        {
            size_t  number;
            ACCEPT *accept;
            bool operator()(const vfs::entry &ent)
            {
                assert(accept);
                if( (*accept)(ent) )
                {
                    ++number;
                }
                return true;
            }
        };

        template <typename ACCEPT> struct collect_context
        {
            size_t                number;
            ACCEPT               *accept;
            sequence<vfs::entry> *target;

            bool operator()(const vfs::entry &ent)
            {
                assert(accept);
                assert(target);
                if( (*accept)(ent) )
                {
                    target->push_back(ent);
                    ++number;
                }
                return true;
            }
        };

        template <typename ACCEPT> struct collect_paths_context
        {
            size_t                number;
            ACCEPT               *accept;
            sequence<string>     *target;

            bool operator()(const vfs::entry &ent)
            {
                assert(accept);
                assert(target);
                if( (*accept)(ent) )
                {
                    target->push_back(ent.path);
                    ++number;
                }
                return true;
            }
        };
     };
    
}

#endif

