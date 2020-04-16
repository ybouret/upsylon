
#include "y/jargon/grammar.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Grammar:: ~Grammar() throw()
        {
            root = 0;
        }
        
        void Grammar:: enroll(Axiom *axiom)
        {
            assert( axiom );
            const Dogma   dogma = axiom;
            const string &key   = *(dogma->label);
            if( ! axioms.insert_by(key,dogma) )
            {
                throw exception("[%s] enroll multiple <%s>", **title, *key);
            }
            if(NULL==root)
            {
                root = axiom;
            }
            aliasing::_(maxLength) = max_of(maxLength,key.size());
        }
        
        bool Grammar:: owns(const Axiom &axiom) const throw()
        {
            const Dogma *ppAxiom = axioms.search_by( *(axiom.label) );
            if(!ppAxiom ) return false;
            const Axiom  &local = **ppAxiom;
            return ( &local == &axiom );
        }
        
        const Axioms & Grammar:: operator*() const throw()
        {
            return axioms;
        }
        
        bool Grammar:: hasRoot() const throw()
        {
            return NULL != root;
        }
        
        
        const Axiom  & Grammar:: getRoot() const throw()
        {
            assert(root);
            assert(owns(*root));
            return *root;
        }
        
        void Grammar:: setRoot(const Axiom &axiom)
        {
            if(!owns(axiom)) throw exception("%s.setGround(foreign <%s>)", **title, **(axiom.label));
            root = & axiom;
        }
        
        
        bool Grammar:: displayAxiom(const Axioms::path &, const Dogma &axiom) const
        {
            if( !owns(*axiom) ) return false;
            const string &akey = * axiom->label;
            std::cerr << akey ; assert(akey.size()<=maxLength);
            for(size_t i=maxLength-akey.length();i>0;--i) std::cerr << ' ';
            std::cerr << " [" << fourcc_(axiom->uuid) << "]";
            std::cerr << std::endl;
            return true;
        }
        
        void Grammar:: displayAxioms() const
        {
            functor<bool,TL2(const Axioms::path&,const Dogma&)> cb( this, & Grammar::displayAxiom);
            (void) axioms.for_each(cb);
        }
        
        Axiom & Grammar:: oom(Axiom &axiom)
        {
            return declare( new OneOrMore(axiom) );
        }
        
        Axiom & Grammar:: zom(Axiom &axiom)
        {
            return declare( new ZeroOrMore(axiom) );
        }
        
        Axiom & Grammar:: opt(Axiom &axiom)
        {
            return declare( new Option(axiom) );
        }
        
        
        
        
        Alternate & Grammar::alt()
        {
            return declare( new Alternate(iAlt) );
        }
        
        Aggregate & Grammar:: agg()
        {
            return declare( new Aggregate(iAgg) );
        }
        
        Axiom     & Grammar:: cat(Axiom &a, Axiom &b)
        {
            Aggregate &compound = agg();
            compound << a << b;
            return compound;
        }
        
        
        Axiom     & Grammar:: cat(Axiom &a, Axiom &b, Axiom &c)
        {
            Aggregate &compound = agg();
            compound << a << b << c;
            return compound;
        }
        
        
        Axiom & Grammar:: choice(Axiom &a, Axiom &b)
        {
            Alternate &compound = alt();
            compound << a << b;
            return compound;
        }
        
        Axiom & Grammar:: choice(Axiom &a, Axiom &b, Axiom &c)
        {
            Alternate &compound = alt();
            compound << a << b << c;
            return compound;
        }
        
        void Grammar:: graphViz(const char *  dotFile, const bool keepFile) const
        {
            const  string _(dotFile);
            graphViz(_,keepFile);
        }
        
        
        
        
        
        AxiomStatus Grammar:: statusOf(const Tag &label, uint32_t &uuid) const throw()
        {
            uuid = 0;
            const Dogma *dogma = axioms.search_by( *label );
            if( dogma )
            {
                const Axiom &axiom = **dogma;
                
                switch( (uuid=axiom.uuid) )
                {
                        //------------------------------------------------------
                        //
                        // terminals
                        //
                        //------------------------------------------------------
                    case Terminal::UUID:
                        switch( axiom.as<Terminal>().feature )
                        {
                            case Terminal::Standard:
                                break;
                                
                            default: return DefiniteAxiom;
                        }
                        return FlexibleAxiom;
                        
                        
                        //------------------------------------------------------
                        //
                        // operators
                        //
                        //------------------------------------------------------
                    case Operator::UUID:
                        switch( axiom.as<Operator>().attribute )
                        {
                            case Operator::Multiple:
                                break;
                                
                            case Operator::Univocal:
                                return DefiniteAxiom;
                        }
                        return FlexibleAxiom;
                        
                        //------------------------------------------------------
                        //
                        // default: break to NoLexemeAxiom
                        //
                        //------------------------------------------------------
                    default:
                        break;
                }
                return NoLexemeAxiom;
                
            }
            else
            {
                // user defined
                return ExternalAxiom;
            }
        }
        
        
        
        
    }
    
}
