//! \file
#ifndef Y_SPADE_VTK_INCLUDED
#define Y_SPADE_VTK_INCLUDED 1

#include "y/spade/fields.hpp"
#include "y/associative/suffix-tree.hpp"

namespace upsylon {

    namespace Spade
    {

        //! vtk API
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
                virtual ~Writer() throw();
                const unsigned components;

                virtual ios::ostream & write(ios::ostream &, const void *) const = 0;

            protected:
                explicit Writer(const unsigned) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Writer);
            };

            typedef arc_ptr<Writer> IWriter;

            //------------------------------------------------------------------
            //
            //! native types writer
            //
            //------------------------------------------------------------------
            class Native : public Writer
            {
            public:
                virtual ~Native() throw();
                string   format;

            protected:
                explicit Native(const char *);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Native);
            };
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
                static const size_t COMPONENTS = sizeof(TUPLE<T>)/sizeof(T);

                inline virtual ~Tuples() throw() {}

                inline explicit Tuples(const Native &nativeRef) :
                Writer(COMPONENTS), native(nativeRef)
                {
                }


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

            const Native & getNative(const std::type_info &) const;

            template <typename T> inline
            const Native  &getNative() const
            {
                static const Native &_ = getNative(typeid(T));
                return _;
            }

            const Writer & getWriter(const std::type_info &) const;

            template <typename T> inline
            const Writer  &getWriter() const
            {
                static const Writer &_ = getWriter(typeid(T));
                return _;
            }

            template <typename T> inline
            ios::ostream & operator()(ios::ostream &fp, const T &args ) const
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
