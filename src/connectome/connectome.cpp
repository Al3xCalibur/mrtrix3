/* Copyright (c) 2008-2017 the MRtrix3 contributors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * MRtrix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * For more details, see http://www.mrtrix.org/.
 */




#include "connectome/connectome.h"


namespace MR {
  namespace Connectome {



    using namespace App;
    const OptionGroup MatrixOutputOptions = OptionGroup ("Options for outputting connectome matrices")
        + Option ("symmetric", "Make matrices symmetric on output")
        + Option ("zero_diagonal", "Set matrix diagonal to zero on output");



  }
}


