

#include "y/jargon/pattern/all.hpp"
#include "y/ios/istream.hpp"
#include "y/exception.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        static const char fn[] = "Jargon::Pattern::Load";
        
        
        Pattern * LoadLogical( Logical *p, ios::istream &fp)
        {
            assert(p);
            auto_ptr<Pattern> guard(p);
            size_t count = 0;
            if( ! fp.query_upack(count) ) throw exception("LoadLogical(missing #operands)");
            while(count-- > 0)
            {
                p->push_back( Pattern::Load(fp) );
            }
            return guard.yield();
        }
        
        Pattern * Pattern:: Load(ios::istream &fp)
        {
            
            uint32_t id = 0;
            if( !fp.query_nbo(id) ) throw exception("%s(missing UUID)",fn);
            
            switch(id)
            {
                    //----------------------------------------------------------
                    // basic
                    //----------------------------------------------------------

                case Any1  ::UUID: return Any1::Create();
                    
                case Single::UUID: {
                    uint8_t code = 0;
                    if(!fp.query_nbo(code))
                        throw exception("%s(missing Single.code)",fn);
                    return Single::Create(code);
                }
                    
                case Excluded::UUID: {
                    uint8_t code = 0;
                    if(!fp.query_nbo(code))
                        throw exception("%s(missing Excluded.code)",fn);
                    return Excluded::Create(code);
                }
                    
                case Range::UUID: {
                    uint8_t lower=0, upper=0;
                    if(!fp.query_nbo(lower)) throw exception("%s(missing Range.lower)",fn);
                    if(!fp.query_nbo(upper)) throw exception("%s(missing Range.upper)",fn);
                    return Range::Create(lower,upper);
                }
                    
                    //----------------------------------------------------------
                    // logic
                    //----------------------------------------------------------
                case AND:: UUID: return LoadLogical(AND:: Create(),fp);
                case OR::  UUID: return LoadLogical(OR::  Create(),fp );
                case NONE::UUID: return LoadLogical(NONE::Create(),fp );

                    //----------------------------------------------------------
                    // joker
                    //----------------------------------------------------------
                case Optional :: UUID : return Optional::Create( Load(fp) );
                case Repeating:: UUID0: return Repeating::ZeroOrMore( Load(fp) );
                case Repeating:: UUID1: return Repeating::OneOrMore( Load(fp) );
                case Repeating:: UUID: {
                    size_t nmin =0; if(!fp.query_upack(nmin)) throw exception("%s(missing Repeating::minimalCount",fn);
                    return Repeating::Create( Load(fp), nmin);
                }
                    
                case Counting:: UUID: {
                    size_t nmin =0; if(!fp.query_upack(nmin)) throw exception("%s(missing Counting::minimalCount",fn);
                    size_t nmax =0; if(!fp.query_upack(nmax)) throw exception("%s(missing Counting::maximalCount",fn);
                    if(nmin>nmax) throw exception("%s(invalid min/max for Counting",fn);
                    return Counting::Create(Load(fp),nmin,nmax);
                }
                    
                default:
                    break;
                    
            }
            throw exception("%s(unknown <%s>)",fn,fourcc_(id));
            
        }
        
    }
    
}
