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

namespace MR {
namespace GUI {

namespace {
class OrientationLabel {
  MEMALIGN(OrientationLabel)
public:
  OrientationLabel() {}
  OrientationLabel(const Eigen::Vector3f &direction, const char textlabel)
      : dir(direction), label(1, textlabel) {}
  Eigen::Vector3f dir;
  std::string label;
  bool operator<(const OrientationLabel &R) const {
    return dir.squaredNorm() < R.dir.squaredNorm();
  }
};
} // namespace

void Projection::draw_orientation_labels() const {
  vector<OrientationLabel> labels;
  labels.push_back(OrientationLabel(
      model_to_screen_direction(Eigen::Vector3f{-1.0, 0.0, 0.0}), 'L')); // red
  labels.push_back(OrientationLabel(
      model_to_screen_direction(Eigen::Vector3f{1.0, 0.0, 0.0}), 'R')); // red
  labels.push_back(OrientationLabel(
      model_to_screen_direction(Eigen::Vector3f{0.0, -1.0, 0.0}),
      'P')); // green
  labels.push_back(OrientationLabel(
      model_to_screen_direction(Eigen::Vector3f{0.0, 1.0, 0.0}), 'A')); // green
  labels.push_back(OrientationLabel(
      model_to_screen_direction(Eigen::Vector3f{0.0, 0.0, -1.0}), 'I')); // blue
  labels.push_back(OrientationLabel(
      model_to_screen_direction(Eigen::Vector3f{0.0, 0.0, 1.0}), 'S')); // blue

  // change color text

  std::sort (labels.begin(), labels.end());
  for (size_t i = 0; i < labels.size(); ++i) {
    std::string label = labels[i].label;
    if(label == "L" || label == "R") {
      setup_render_text(0.651, 0.3216, 0.6039);
    } else if (label == "P" || label == "A"){
      setup_render_text(0.9569, 0.902, 0.0);
    } else {
      setup_render_text(0.4314, 0.7804, 0.851);
    }
    float pos[] = { labels[i].dir[0], labels[i].dir[1]};
    float dist = std::min (height()/(10*abs (pos[0])), height()/(10*abs (pos[1]))) * 1.2;
    int x = std::round (width() /1.1 + pos[0]*dist);
    int y = std::round (height() - width()/11 + pos[1]*dist);
    render_text_inset (x, y, std::string (labels[i].label));
    done_render_text();
  }

}

} // namespace GUI
} // namespace MR
