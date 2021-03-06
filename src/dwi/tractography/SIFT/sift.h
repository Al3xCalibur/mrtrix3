/*
 * Copyright (c) 2008-2018 the MRtrix3 contributors.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at http://mozilla.org/MPL/2.0/
 *
 * MRtrix3 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * For more details, see http://www.mrtrix.org/
 */


#ifndef __dwi_tractography_sift_sift_h__
#define __dwi_tractography_sift_sift_h__

#include "app.h"

namespace MR {
namespace App { class OptionGroup; }
namespace DWI {
namespace Tractography {
namespace SIFT {


extern const App::OptionGroup SIFTModelOption;
extern const App::OptionGroup SIFTOutputOption;
extern const App::OptionGroup SIFTTermOption;


}
}
}
}

#endif

