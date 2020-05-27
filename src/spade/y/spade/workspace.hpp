
//! \file

#ifndef Y_SPADE_WORKSPACE_INCLUDED
#define Y_SPADE_WORKSPACE_INCLUDED 1

#include "y/spade/layout/fragment.hpp"
#include "y/spade/network/transfer.hpp"

#include "y/associative/set.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon {

    namespace Spade {


        //! Field class for I/O
        enum FieldClass
        {
            AsyncField, //!< global field
            LocalField  //!< helper field
        };

        typedef set<string,_Field> FieldsDB;  //!< store fields by name
        typedef vector<_Field>     FieldsIO_; //!< base class to handle fields
        
        //! Fields for I/O
        class FieldsIO : public FieldsIO_
        {
        public:
            explicit FieldsIO() throw();             //!< setup
            explicit FieldsIO(const size_t n);       //!< setup with memory
            virtual ~FieldsIO() throw();             //!< cleanup
            FieldsIO( const FieldsIO &);             //!< copy
            FieldsIO & operator=( const FieldsIO &); //!< assign
            void sort() throw();                     //!< sort by name
            FieldsIO & operator<<( Field &);         //!< check dynamic
        };
        
        
        namespace Kernel
        {
            //! common Workspace stuff
            class Workspace : public Object
            {
            public:

                virtual ~Workspace() throw(); //!< cleanup

                const unsigned  dimensions; //!< dimensions
                const FieldsDB  fdb;        //!< all fields
                const FieldsIO  fields;     //!< all async

                bool owns(const Field  &) const throw();                //!< check ownership
                bool owns(const _Field &) const throw();                //!< check ownership
                bool ownsAll(const accessible<_Field> &) const throw(); //!< check ownershipt
                
                const Field & getField(const string &id) const; //!< get field by name
                const Field & getField(const char   *id) const; //!< get field by name

                
            protected:
                explicit Workspace(const unsigned) throw();                     //!< setup
                void add(const _Field &, const FieldClass cls);                //!< register a field
                static size_t CheckRank(const size_t size, const size_t rank); //!< validate size/rank
                
                //! check that the mapping has the right size
                template <typename COORD> static inline
                const COORD &CheckMapping(const size_t size, const COORD &C)
                {
                    CheckMappingValue(size, Coord::Product(C) );
                    return C;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Workspace);
                static void CheckMappingValue(const size_t size, const Coord1D prod);

            };
        }

        //----------------------------------------------------------------------
        //
        //! workspace for fields
        /**
         compute local topology and fragment characteristics to
         handle fields bases on this fragment of the full layout
         */
        //
        //----------------------------------------------------------------------
        template <typename COORD>
        class Workspace :
        public Kernel::Workspace,
        public Dispatch<COORD>,
        public Fragment<COORD>
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definition
            //
            //------------------------------------------------------------------
            static const unsigned Dimensions = Fragment<COORD>::Dimensions; //!< Dimensions
            static const unsigned Levels     = Fragment<COORD>::Levels;     //!< Levels = (3^Dimensions-1)/2
            typedef struct FieldFor<COORD> __Field;                         //!< Field type selector

            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------

            //! setup
            /**
             \param fullLayout the global layout
             \param mapping    the mapping to split the layout => size=product(mapping)
             \param globalRank the globalRank < size
             \param boundaries the periodic boundary conditions
             \param numGhosts  the size of the ghost zone
             */
            inline explicit Workspace(const Layout<COORD> &fullLayout,
                                      const COORD          mapping,
                                      const size_t         globalRank,
                                      const COORD          boundaries,
                                      const Coord1D        numGhosts) :
            Kernel::Workspace(Dimensions),
            Dispatch<COORD>(mapping),
            Fragment<COORD>(fullLayout,
                            this->getLocalRanks( CheckRank(this->size,globalRank) ),
                            *this,
                            boundaries,
                            numGhosts)
            {
            }

            //! cleanup
            inline virtual ~Workspace() throw() {}

            //------------------------------------------------------------------
            //
            // fields creation and access
            //
            //------------------------------------------------------------------
            //! get field by name
            template <typename LABEL> inline
            Field & operator[](const LABEL &id)
            {
                return (Field &)getField(id);
            }
            
            //! get field by name
            template <typename LABEL> inline
            const Field & operator[](const LABEL &id) const
            {
                return getField(id);
            }
            
            //! create a new field
            template <typename T>
            typename __Field:: template Of<T>::Type & create(const string &id, const FieldClass cls=AsyncField)
            {
                // create a field base on outer layout
                typedef typename __Field:: template Of<T>::Type FieldType;
                FieldType    *F = new FieldType(id,this->outer);
                const _Field  P = F;

                // insert into database/fields
                add(P,cls);

                // done
                return *F;
            }

            //! create a new field
            template <typename T>
            typename __Field:: template Of<T>::Type & create( const char *id, const FieldClass cls=AsyncField)
            {
                const string _(id); return create<T>(_,cls);
            }

            //------------------------------------------------------------------
            //
            // sequential operation based on a transfer object
            //
            //------------------------------------------------------------------

            //! activate I/O for all async fields
            inline void  setupWith( const Transfer &transfer )
            {
                for(size_t i=fields.size();i>0;--i)
                {
                    transfer.setup( *aliasing::_(fields[i]) );
                }
            }

            //! localSwap of someFields
            inline void localSwap(addressable<_Field> &someFields,
                                  const Transfer      &transfer )
            {
                assert(ownsAll(someFields));
                transfer.localSwap(someFields,*this);
            }

            //! localSwap of one field
            inline void localSwap(Field               &field,
                                  const Transfer      &transfer)
            {
                assert(owns(field));
                transfer.localSwap(field,*this);
            }

            //! localSwap of all fields
            inline void localSwap(const Transfer &transfer)
            {
                localSwap( aliasing::_(fields), transfer );
            }


            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Workspace);

        };

    }

}


#endif

