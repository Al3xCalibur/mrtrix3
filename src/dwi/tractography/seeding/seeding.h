/*
    Copyright 2011 Brain Research Institute, Melbourne, Australia

    Written by Robert E. Smith, 2012.

    This file is part of MRtrix.

    MRtrix is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    MRtrix is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MRtrix.  If not, see <http://www.gnu.org/licenses/>.

 */

#ifndef __dwi_tractography_seeding_seeding_h__
#define __dwi_tractography_seeding_seeding_h__


#include "dwi/tractography/seeding/basic.h"
#include "dwi/tractography/seeding/dynamic.h"
#include "dwi/tractography/seeding/gmwmi.h"
#include "dwi/tractography/seeding/list.h"



namespace MR
{

  namespace App { class OptionGroup; }

  namespace DWI
  {
    namespace Tractography
    {

      class Properties;

      namespace Seeding
      {


        extern const App::OptionGroup SeedOption;
        void load_tracking_seeds (Properties&);



      }
    }
  }
}

#endif

