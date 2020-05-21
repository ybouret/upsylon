
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



        typedef set<string,_Field> FieldsDB; //!< store fields by name
        typedef vector<_Field>     FieldsIO_; //!< store fields
        
        class FieldsIO : public FieldsIO_
        {
        public:
            explicit FieldsIO() throw();
            explicit FieldsIO(const size_t n);
            virtual ~FieldsIO() throw();
            FieldsIO( const FieldsIO &);
            FieldsIO & operator=( const FieldsIO &);
            
            void sort() throw();
            FieldsIO & operator<<( Field &);
            
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
                const FieldsIO_ fields;     //!< all async

                bool owns(const _Field &) const throw();
                bool ownsAll(const accessible<_Field> &) const throw();

            protected:
                explicit Workspace(const unsigned) throw();          //!< setup
                void add(const _Field &, const FieldClass cls); //!< register a field

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Workspace);
            };
        }

        //! workspace for fields
        template <typename COORD>
        class Workspace :
        public Kernel::Workspace,
        public Topology<COORD>,
        public Fragment<COORD>
        {
        public:

            // types and definition
            static const unsigned Dimensions = Fragment<COORD>::Dimensions; //!< Dimesions
            typedef struct FieldFor<COORD> __Field; //!< Field type selector

            //! setup
            inline explicit Workspace(const Layout<COORD> &fullLayout,
                                      const COORD          mapping,
                                      const size_t         globalRank,
                                      const COORD          boundaries,
                                      const Coord1D        numGhosts) :
            Kernel::Workspace(Dimensions),
            Topology<COORD>(mapping),
            Fragment<COORD>(fullLayout,
                            this->getLocalRanks(globalRank),
                            *this,
                            boundaries,
                            numGhosts)
            {
            }

            //! cleanup
            inline virtual ~Workspace() throw() {}

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


            //! activate I/O for all async fields
            inline void  activateFor( const Transfer &transfer )
            {
                for(size_t i=fields.size();i>0;--i)
                {
                    transfer.activate( *aliasing::_(fields[i]) );
                }
            }

            //! localSwap of someFields
            inline void localSwap(addressable<_Field> &someFields,
                                  const Transfer      &transfer )
            {
                assert(ownsAll(someFields));
                transfer.localSwap(someFields,*this);
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

