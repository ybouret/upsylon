//! \file
#ifndef Y_VFS_INCLUDED
#define Y_VFS_INCLUDED

#include "y/string.hpp"
#include "y/functor.hpp"
#include "y/ptr/auto.hpp"
#include "y/sequence/list.hpp"

namespace upsylon {
	
	//! Virtual File System interface
    class vfs : public object {
    public:
        virtual ~vfs() throw();
        //==================================================================
        //
        // entry API
        //
        //==================================================================
        //! one entry
        class entry : public object
        {
        public:
            typedef size_t attribute;          //!< attribute type
            static const attribute no_ent = 0; //!< no entry
            static const attribute is_reg = 1; //!< regular file
            static const attribute is_dir = 2; //!< directory
            static const attribute is_unk = 4; //!< unknown

            //! convert attribute to human readable text
            static const char *get_attr_text(const attribute a) throw();

            //! build entry
            explicit entry( const string &vfs_path, const vfs &vfs_from );

            //! destructor
            virtual ~entry() throw();

            //! copy
            entry( const entry &other );
            
            const string     path;      //!< full path
            const char      *cstr;      //!< full path, low-level
            const char      *base_name; //!< base name
            const char      *extension; //!< extension, may be null
            const bool       link;      //!< if recognized as symbolic link
            const attribute  attr;      //!< the attribute
            const char      *attr2text() const throw(); //!< the human readable attribute

            //! callback to act on entries
            typedef functor<bool,TL1(const entry &)> callback;
            
            bool is_regular()   const throw(); //!< attr == is_reg
            bool is_directory() const throw(); //!< attr == is_dir
            bool has_extension( const string &ext ) const throw(); //!< extension!=NULL && extention == ext
            bool has_extension( const char   *ext ) const throw(); //!< extension!=NULL && extention == ext
            bool is_dot()  const throw();        //!< '.'
            bool is_ddot() const throw();        //!< '..'
            bool is_dot_or_ddot() const throw(); //!< '.' or '..'
            bool is_subdir() const throw();      //!< a directory not '.' or '..'
        private:
            Y_DISABLE_ASSIGN(entry);
        };
        
        //======================================================================
        //
        // scanner API
        //
        //======================================================================
        //! scanning directory entries
        class scanner : public object
        {
        public:
            //! destructor
            virtual ~scanner() throw();
            //! interface: get next entry
            virtual  const entry * next() = 0;
            
            const vfs    &fs;     //!< owner fs
            const string  folder; //!< directory
            
        protected:
            //! common initializer
            explicit scanner( const string &dirname, const vfs &owner );

            //! make an entry in current with its name
            const entry * make_entry( const char *entry_name );
            
            
        private:
            auto_ptr<entry> current;
            Y_DISABLE_COPY_AND_ASSIGN(scanner);
        };

        
        //==================================================================
        //
        // virtual interface
        //
        //==================================================================
        //! query attribute for path
        virtual entry::attribute query_attribute( const string &path, bool &is_link ) const throw() = 0;
        //! create a directory
        virtual void             create_dir(    const string &dirname, const bool allow_already_exists ) = 0;
        //! remove a directory
        virtual void             remove_dir(    const string &dirname  ) = 0 ;
        //! remove a file
        virtual void             remove_file(   const string &filename ) = 0;
        //! create a scanner for a given directory
        virtual scanner *        new_scanner(   const string &dirname ) const = 0;
        //! get a regular file size
        virtual uint64_t         get_file_size( const string &path ) const = 0;
        
        //==================================================================
        //
        // non virtual interface
        //
        //==================================================================
        //! mkdir -p dirname
        void         create_sub_dir( const string &dirname );
        //! test if regilar file
        bool         is_reg( const string &path ) const throw();
        //! test if directory
        bool         is_dir( const string &path ) const throw();
        //! try to remove a file
        void         try_remove_file( const string &path) throw();
        
        //! remove file matching filter in dirname
        template <typename ACCEPT>
        inline void remove_files( const string &dirname, ACCEPT &to_remove )
        {
            list<string> matching;
            //__________________________________________________________________
            //
            // scan the directory
            //__________________________________________________________________
            {
                auto_ptr<vfs::scanner> scan( new_scanner(dirname) );
                const vfs::entry *ep = 0;
                while( 0 != (ep=scan->next()) )
                {
                    if(to_remove(*ep) )
                    {
                        matching.push_back( ep->path );
                    }
                }
            }

            //__________________________________________________________________
            //
            // remove selected, shouldn't complain!
            //__________________________________________________________________
            while( matching.size() )
            {
                remove_file( matching.back() );
                matching.pop_back();
            }
        }
        
        //! remove files with given extension in dirname
        void         remove_files_with_extension_in( const string &dirname, const string &extension);
        
        //==================================================================
        //
        // utilities
        //
        //==================================================================
        //! base name
        static const char * get_base_name( const char *buffer, size_t buflen ) throw();
        //! extension
        static const char * get_extension(  const char *buffer, size_t buflen ) throw();
        //! file directory
        static const char * get_file_dir( size_t &dirlen, const char *buffer, size_t buflen ) throw();

        //! base name
        static const char * get_base_name( const char   *path ) throw();
        //! base name
        static const char * get_base_name( const string &path ) throw();
        //! extension
        static const char * get_extension( const char   *path ) throw();
        //! extension
        static const char * get_extension( const string &path ) throw();
        //! file directory
        static const char * get_file_dir( size_t &dirlen, const char   *path) throw();
        //! file directory
        static const char * get_file_dir( size_t &dirlen, const string &path) throw();

        //! file directory
        static string get_file_dir( const char   *path, size_t pathlen );
        //! file directory
        static string get_file_dir( const char   *path );
        //! file directory
        static string get_file_dir( const string &path );
        
        //! check for good syntax
        static string &as_directory( string &path );
        //! copy of a checked syntax
        static string  to_directory( const string &path );
        //! copy of a checked syntax
        static string  to_directory( const char   *path );
        //! in place change extension
        static void    change_extension( string &path, const string &ext );
        //! copy of a filename with a changed extension
        static inline string  with_new_extension( const string &path, const string &ext)
        {
            string tmp = path; change_extension(tmp,ext); return tmp;
        }
        //! remove current extension, in place
        static void    remove_extension( string &path ) throw();
        //! copy of a filename without extension
        static inline string without_extension( const string &path )
        {
            string tmp = path; remove_extension(tmp); return tmp;
        }
        static string  base_name_from(const string &name);  //!< make an acceptable file name
        static string  cpp_label_from(const string &name);  //!< C Pre Processor label
        static string  class_name_for(const string &name);  //!< acceptable class name

    protected:
        explicit vfs() throw();
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(vfs);
    };
	
}

#endif
