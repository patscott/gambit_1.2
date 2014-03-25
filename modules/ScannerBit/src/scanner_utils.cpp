//  GAMBIT: Global and Modular BSM Inference Tool
//  *********************************************
/// \file
///  Utility functions definitions for ScannerBit
///
///  *********************************************
///
///  Authors
///  =======
///
///  (add name and date if you modify)
///
///  \author Pat Scott 
///          (patscott@physics.mcgill.ca)
///  \date 2014 Mar
///
///  *********************************************

#include "scanner_utils.hpp"

namespace Gambit
{

        namespace Scanner
        { 

                /// Scanner errors
                error& scan_error()
                {
                        static error local("A problem has been raised by ScannerBit.","scan_error", LogTags::scan);
                        return local;
                }

                /// Scanner warnings
                warning& scan_warning()
                {
                        static warning local("A problem has been raised by ScannerBit.","scan_warning", LogTags::scan);
                        return local;
                }

        }

}
