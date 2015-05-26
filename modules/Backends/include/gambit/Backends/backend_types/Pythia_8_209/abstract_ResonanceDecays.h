#ifndef __abstract_ResonanceDecays_Pythia_8_209_h__
#define __abstract_ResonanceDecays_Pythia_8_209_h__

#include "gambit/Backends/abstractbase.hpp"
#include "forward_decls_abstract_classes.h"
#include "forward_decls_wrapper_classes.h"
#include "wrapper_Info_decl.h"
#include "wrapper_ParticleData_decl.h"
#include "wrapper_Rndm_decl.h"
#include <vector>
#include "wrapper_Event_decl.h"
#include <cstddef>

#ifndef ENUMS_DECLARED
#define ENUMS_DECLARED
#include "enum_decl_copies.h"
#endif

#include "identification.hpp"

// Forward declaration needed by the destructor pattern.
void wrapper_deleter(CAT_3(BACKENDNAME,_,SAFE_VERSION)::Pythia8::ResonanceDecays*);


namespace CAT_3(BACKENDNAME,_,SAFE_VERSION)
{
    
    
    namespace Pythia8
    {
        class Abstract_ResonanceDecays : virtual public AbstractBase
        {
            private:
                // IGNORED: Variable  -- Name: NTRYCHANNEL  -- XML id: _21906
                // IGNORED: Variable  -- Name: NTRYMASSES  -- XML id: _21907
                // IGNORED: Variable  -- Name: MSAFETY  -- XML id: _21908
                // IGNORED: Variable  -- Name: WIDTHCUT  -- XML id: _21909
                // IGNORED: Variable  -- Name: TINY  -- XML id: _21910
                // IGNORED: Variable  -- Name: TINYBWRANGE  -- XML id: _21911
                // IGNORED: Variable  -- Name: WTCORRECTION  -- XML id: _21912
                // IGNORED: Field  -- Name: infoPtr  -- XML id: _21913
                // IGNORED: Field  -- Name: particleDataPtr  -- XML id: _21914
                // IGNORED: Field  -- Name: rndmPtr  -- XML id: _21915
                // IGNORED: Field  -- Name: id0  -- XML id: _21916
                // IGNORED: Field  -- Name: mult  -- XML id: _21917
                // IGNORED: Field  -- Name: m0  -- XML id: _21918
                // IGNORED: Field  -- Name: idProd  -- XML id: _21919
                // IGNORED: Field  -- Name: cols  -- XML id: _21920
                // IGNORED: Field  -- Name: acols  -- XML id: _21921
                // IGNORED: Field  -- Name: mProd  -- XML id: _21922
                // IGNORED: Field  -- Name: pProd  -- XML id: _21923
            public:
    
                virtual void init__BOSS(Pythia8::Abstract_Info*, Pythia8::Abstract_ParticleData*, Pythia8::Abstract_Rndm*) =0;
    
                virtual bool next__BOSS(Pythia8::Abstract_Event&, int) =0;
    
                virtual bool next__BOSS(Pythia8::Abstract_Event&) =0;
            private:
    
                virtual bool pickMasses() =0;
    
                virtual bool pickColours__BOSS(int, Pythia8::Abstract_Event&) =0;
    
                virtual bool pickKinematics() =0;
    
            public:
                virtual void pointerAssign__BOSS(Abstract_ResonanceDecays*) =0;
                virtual Abstract_ResonanceDecays* pointerCopy__BOSS() =0;
    
            private:
                mutable ResonanceDecays* wptr;
    
            public:
                Abstract_ResonanceDecays()
                {
                }
    
                void wrapper__BOSS(ResonanceDecays* wptr_in)
                {
                    wptr = wptr_in;
                    is_wrapped(true);
                    can_delete_wrapper(true);
                }
    
                ResonanceDecays* wrapper__BOSS()
                {
                    return wptr;
                }
    
                virtual ~Abstract_ResonanceDecays()
                {
                    if (can_delete_wrapper())
                    {
                        can_delete_me(false);
                        wrapper_deleter(wptr);
                    }
                }
        };
    }
    
}


#include "gambit/Backends/backend_undefs.hpp"


#endif /* __abstract_ResonanceDecays_Pythia_8_209_h__ */
