.. _tck2connectome:

tck2connectome
===================

Synopsis
--------

Generate a connectome matrix from a streamlines file and a node parcellation image

Usage
--------

::

    tck2connectome [ options ]  tracks_in nodes_in connectome_out

-  *tracks_in*: the input track file
-  *nodes_in*: the input node parcellation image
-  *connectome_out*: the output .csv file containing edge weights

Options
-------

Structural connectome streamline assignment option
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

-  **-assignment_end_voxels** use a simple voxel lookup value at each streamline endpoint

-  **-assignment_radial_search radius** perform a radial search from each streamline endpoint to locate the nearest node.Argument is the maximum radius in mm; if no node is found within this radius, the streamline endpoint is not assigned to any node. Default search distance is 2mm.

-  **-assignment_reverse_search max_dist** traverse from each streamline endpoint inwards along the streamline, in search of the last node traversed by the streamline. Argument is the maximum traversal length in mm (set to 0 to allow search to continue to the streamline midpoint).

-  **-assignment_forward_search max_dist** project the streamline forwards from the endpoint in search of a parcellation node voxel. Argument is the maximum traversal length in mm.

-  **-assignment_all_voxels** assign the streamline to all nodes it intersects along its length (note that this means a streamline may be assigned to more than two nodes, or indeed none at all)

Structural connectome metric options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

-  **-scale_length** scale each contribution to the connectome edge by the length of the streamline

-  **-scale_invlength** scale each contribution to the connectome edge by the inverse of the streamline length

-  **-scale_invnodevol** scale each contribution to the connectome edge by the inverse of the two node volumes

-  **-scale_file path** scale each contribution to the connectome edge according to the values in a vector file

Options for outputting connectome matrices
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

-  **-symmetric** Make matrices symmetric on output

-  **-zero_diagonal** Set matrix diagonal to zero on output

Other options for tck2connectome
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

-  **-stat_edge statistic** statistic for combining the values from all streamlines in an edge into a single scale value for that edge (options are: sum,mean,min,max; default=sum)

-  **-tck_weights_in path** specify a text scalar file containing the streamline weights

-  **-keep_unassigned** By default, the program discards the information regarding those streamlines that are not successfully assigned to a node pair. Set this option to keep these values (will be the first row/column in the output matrix)

-  **-out_assignments path** output the node assignments of each streamline to a file; this can be used subsequently e.g. by the command connectome2tck

-  **-vector** output a vector representing connectivities from a given seed point to target nodes, rather than a matrix of node-node connectivities

Standard options
^^^^^^^^^^^^^^^^

-  **-info** display information messages.

-  **-quiet** do not display information messages or progress status. Alternatively, this can be achieved by setting the MRTRIX_QUIET environment variable to a non-empty string.

-  **-debug** display debugging messages.

-  **-force** force overwrite of output files. Caution: Using the same file as input and output might cause unexpected behaviour.

-  **-nthreads number** use this number of threads in multi-threaded applications (set to 0 to disable multi-threading).

-  **-help** display this information page and exit.

-  **-version** display version information and exit.

References
^^^^^^^^^^

If using the default streamline-parcel assignment mechanism (or -assignment_radial_search option): Smith, R. E.; Tournier, J.-D.; Calamante, F. & Connelly, A. The effects of SIFT on the reproducibility and biological accuracy of the structural connectome. NeuroImage, 2015, 104, 253-265

If using -scale_invlength or -scale_invnodevol options: Hagmann, P.; Cammoun, L.; Gigandet, X.; Meuli, R.; Honey, C.; Wedeen, V. & Sporns, O. Mapping the Structural Core of Human Cerebral Cortex. PLoS Biology 6(7), e159

--------------



**Author:** Robert E. Smith (robert.smith@florey.edu.au)

**Copyright:** Copyright (c) 2008-2018 the MRtrix3 contributors.

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, you can obtain one at http://mozilla.org/MPL/2.0/

MRtrix3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

For more details, see http://www.mrtrix.org/


