
//! \file

#ifndef Y_ALCHEMY_EQUILIBRIA_INCLUDED
#define Y_ALCHEMY_EQUILIBRIA_INCLUDED 1

#include "y/alchemy/equilibrium.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace alchemy
    {
        
        //______________________________________________________________________
        //
        //
        //! database of equilibria
        //
        //______________________________________________________________________
        class equilibria : public compilable
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef hash_set<string,equilibrium::pointer> db_type;        //!< alias
            typedef db_type::const_iterator               const_iterator; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit equilibria();         //!< setup, empty
            virtual ~equilibria() throw(); //!< cleanup
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
           
            //! register a new equilibrium
            equilibrium & operator()(equilibrium *);
            
            //! helper for a new constant equilibrium
            template <typename ID> inline
            equilibrium & operator()(const ID &id, const double K)
            {
                return (*this)( new constant_equilibrium(id,K) );
            }

            //! output formatted
            template <typename OSTREAM> inline
            OSTREAM & display(OSTREAM &os,const double t=0.0) const
            {
                const size_t neqs = db.size();
                vector<string,memory::pooled> info(neqs,as_capacity);
                vector<string,memory::pooled> kval(neqs,as_capacity);
                size_t max_info = 0;
                size_t max_kval = 0;
                
                for(const_iterator it=db.begin();it!=db.end();++it)
                {
                    const equilibrium &eq = **it;
                    const string fmt = eq.format();
                    const string val = vformat("%.15g",eq.K(t));
                    info.push_back_(fmt);
                    kval.push_back_(val);
                    max_info = max_of(max_info,fmt.size());
                    max_kval = max_of(max_kval,val.size());
                }

                for(size_t i=1;i<=neqs;++i)
                {
                    os << info[i];
                    for(size_t j=info[i].size();j<=max_info;++j) os << ' ';
                    os << "| (";
                    os << kval[i];
                    for(size_t j=kval[i].size();j<max_kval;++j) os << ' ';
                    os << ")" << '\n';
                }

                return os;
            }

            const db_type & operator*()  const throw(); //!< content
            const db_type * operator->() const throw(); //!< content
            
            //! display compact format
            template <typename OSTREAM> inline
            OSTREAM & display_code(OSTREAM &os) const
            {
                for(const_iterator it=db.begin();it!=db.end();++it)
                {
                    const equilibrium &eq = **it;
                    eq.display_code(os) << '\n';
                }
                return os;
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(equilibria);
            void         look_up(size_t &rlen, size_t &plen) const throw();
            virtual void on_compile();
            
            db_type db;
            
        public:
            const size_t max_name; //!< max equilibrium name length


        };
        
    }
}

#endif

