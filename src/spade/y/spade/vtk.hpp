//! \file
#ifndef Y_SPADE_VTK_INCLUDED
#define Y_SPADE_VTK_INCLUDED 1

#include "y/spade/fields.hpp"
#include "y/associative/suffix-tree.hpp"

namespace upsylon {

    namespace Spade
    {
        //----------------------------------------------------------------------
        //
        //
        //! vtk API
        //
        //
        //----------------------------------------------------------------------
        class vtk : public singleton<vtk>
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            static const at_exit::longevity life_time = 0; //!< for singleton

            //------------------------------------------------------------------
            //
            //
            // Formated type writing
            //
            //
            //------------------------------------------------------------------

            //------------------------------------------------------------------
            //
            //! base class for writers
            //
            //------------------------------------------------------------------
            class Writer : public Object
            {
            public:
                //--------------------------------------------------------------
                //
                // virtual interface
                //
                //--------------------------------------------------------------
                virtual ~Writer() throw(); //!< cleanup
                
                //! write a given object
                virtual ios::ostream & write(ios::ostream &, const void *) const = 0;
               
                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                const unsigned components; //!< number of native components

            protected:
                explicit Writer(const unsigned) throw(); //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Writer);
            };
            
            //! alias to use in database
            typedef arc_ptr<Writer> IWriter;

            //------------------------------------------------------------------
            //
            //! native types writer
            //
            //------------------------------------------------------------------
            class Native : public Writer
            {
            public:
                virtual ~Native() throw(); //!< cleanup
                string   format;           //!< C-style string format

            protected:
                explicit Native(const char *); //!< setup Writer(1) and format

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Native);
            };
            
            //! alias to use in database
            typedef arc_ptr<Native> INative;

            //------------------------------------------------------------------
            //
            //! tuples writer
            //
            //------------------------------------------------------------------
            template <template <typename> class TUPLE, typename T>
            class Tuples : public Writer
            {
            public:
                //! global definition
                static const size_t COMPONENTS = sizeof(TUPLE<T>)/sizeof(T);

                //! cleanup
                inline virtual ~Tuples() throw() {}
                
                //! setup from a Native reference
                inline explicit Tuples(const Native &nativeRef) :
                Writer(COMPONENTS), native(nativeRef) {}

            private:
                const Native &native;
                Y_DISABLE_COPY_AND_ASSIGN(Tuples);
                inline virtual ios::ostream & write(ios::ostream &fp, const void *addr) const
                {
                    const T *p = static_cast<const T *>(addr);
                    native.write(fp,&p[0]);
                    for(unsigned c=1;c<COMPONENTS;++c)
                    {
                        native.write(fp<<' ', &p[c]);
                    }
                    return fp;
                }
            };

            //------------------------------------------------------------------
            //
            //
            // accessing writer and data formatting
            //
            //
            //------------------------------------------------------------------

            
            //! get matching native writer
            const Native & getNative(const std::type_info &) const;

            //! get matching native writer
            template <typename T> inline
            const Native  &getNative() const
            {
                static const Native &_ = getNative(typeid(T));
                return _;
            }

            //! get native or defined writer
            const Writer & getWriter(const std::type_info &) const;

            //! get native or defined writer
            template <typename T> inline
            const Writer  &getWriter() const
            {
                static const Writer &_ = getWriter(typeid(T));
                return _;
            }

            //! write formatted output of registered type
            template <typename T> inline
            ios::ostream & operator()(ios::ostream &fp, const T &args) const
            {
                static const Writer &_ = getWriter<T>();
                return _.write(fp,&args);
            }
            

        private:
            explicit vtk();
            virtual ~vtk() throw();
            friend class singleton<vtk>;
            Y_DISABLE_COPY_AND_ASSIGN(vtk);

            suffix_tree<INative> natives;
            suffix_tree<IWriter> writers;

            void registerNatives();
            void registerWriters();
        };

    }

}

#endif
