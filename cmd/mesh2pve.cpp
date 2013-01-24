/*
    Copyright 2011 Brain Research Institute, Melbourne, Australia

    Written by Robert E. Smith, 2011.

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


#include "app.h"

#include "image/header.h"

#include "mesh/mesh.h"




MRTRIX_APPLICATION

using namespace MR;
using namespace App;



void usage ()
{

	AUTHOR = "Robert E. Smith (r.smith@brain.org.au)";

  DESCRIPTION
  + "convert a mesh surface to a partial volume estimation image.";

  ARGUMENTS
  + Argument ("source",   "the mesh file (currently only .vtk files are supported)").type_file()
  + Argument ("template", "the template image").type_image_in()
  + Argument ("output",   "the output image").type_text();

};



void run ()
{

  // Read in the mesh data
  Mesh::Mesh mesh (argument[0]);

  // Get the template image
  Image::Header template_image (argument[1]);

  // Convert the mesh to voxel coordinates to make processing easier
  mesh.transform_fsl_to_image (template_image);

  // Create the output image
  mesh.output_pve_image (template_image, argument[2]);

}
