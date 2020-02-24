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


#include "gui/projection.h"

namespace MR
{
  namespace GUI
  {

    namespace
    {
      class OrientationLabel
      { MEMALIGN(OrientationLabel)
        public:
          OrientationLabel () { }
          OrientationLabel (const Eigen::Vector3f& direction, const char textlabel) :
            dir (direction), label (1, textlabel) { }
          Eigen::Vector3f dir;
          std::string label;
          bool operator< (const OrientationLabel& R) const {
            return dir.squaredNorm() < R.dir.squaredNorm();
          }
      };
    }



    void Projection::draw_orientation_labels () const
    {
      vector<OrientationLabel> labels;
      labels.push_back (OrientationLabel (model_to_screen_direction (Eigen::Vector3f {-1.0,  0.0,  0.0}), 'L'));// red
      labels.push_back (OrientationLabel (model_to_screen_direction (Eigen::Vector3f { 1.0,  0.0,  0.0}), 'R'));// red
      labels.push_back (OrientationLabel (model_to_screen_direction (Eigen::Vector3f { 0.0, -1.0,  0.0}), 'P'));// green
      labels.push_back (OrientationLabel (model_to_screen_direction (Eigen::Vector3f { 0.0,  1.0,  0.0}), 'A'));// green
      labels.push_back (OrientationLabel (model_to_screen_direction (Eigen::Vector3f { 0.0,  0.0, -1.0}), 'I'));// blue
      labels.push_back (OrientationLabel (model_to_screen_direction (Eigen::Vector3f { 0.0,  0.0,  1.0}), 'S'));// blue

      // change color text

      std::sort (labels.begin(), labels.end());
      int coordLines [6][2];
      for (size_t i = 0; i < labels.size(); ++i) {
        if(i == 0 || i == 1) {
          setup_render_text(1.0, 0.0, 0.0);
        } else if (i==2 || i == 3){
          setup_render_text(0.0, 1.0, 0.0);
        } else {
          setup_render_text(0.0, 0.0, 1.0);
        }
        float pos[] = { labels[i].dir[0], labels[i].dir[1]};
        float dist = std::min (width()/(10*abs (pos[0])), height()/(10*abs (pos[1]))) / 2.0;
        int x = std::round (width() /1.1 + pos[0]*dist);
        int y = std::round (height() /1.1 + pos[1]*dist);
        coordLines[i][0] = x;
        coordLines[i][1] = y;
        render_text_inset (x, y, std::string (labels[i].label));
        done_render_text();
      }
      for (int i = 0; i<5; i+=2) {
            glBegin(GL_LINE);
//                glVertex2f(0,0);
//                glVertex2f(width(), height());
                glVertex2f(coordLines[i][0], coordLines[i][1]);
                glVertex2f(coordLines[i+1][0], coordLines[i+1][1]);
            glEnd();
      }
    }






  }
}

