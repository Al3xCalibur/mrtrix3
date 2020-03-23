//
// Created by alex on 09/03/2020.
//

#include "gui/crosshair_orientation.h"
#include "gui/projection.h"

namespace MR
{
  namespace GUI
  {



    void CrosshairOrientation::render (const Eigen::Vector3f& focus,
                            const ModelViewProjection& MVP) const
    {
      if (!VB || !VAO) {
        VB.gen();
        VAO.gen();

        VB.bind (gl::ARRAY_BUFFER);
        VAO.bind();

        gl::EnableVertexAttribArray (0);
        gl::VertexAttribPointer (0, 2, gl::FLOAT, gl::FALSE_, 0, nullptr);
      } else {
        VB.bind (gl::ARRAY_BUFFER);
        VAO.bind();
      }

      if (!program) {
        GL::Shader::Vertex vertex_shader (
            "layout(location=0) in vec2 pos;\n"
            "// orientation\n"
            "void main () {\n"
            "  gl_Position = vec4 (pos, 0.0, 1.0);\n"
            "}\n");
        GL::Shader::Fragment fragment_shader (
            "out vec4 color;\n"
            "void main () {\n"
            "  color = vec4 (0.5, 0.0, 0.5, 1.0);\n"
            "}\n");
        program.attach (vertex_shader);
        program.attach (fragment_shader);
        program.link();
      }

      int cubeSize = 24;
//      Eigen::Vector3f F = MVP.model_to_screen (focus);
      Eigen::Vector3f cube[] = {
          Eigen::Vector3f{0.0,0.5,0.0},
          Eigen::Vector3f{1.0,0.5,0.0},

          Eigen::Vector3f{1.0,0.5,0.0},
          Eigen::Vector3f{1.0,0.5,1.0},

          Eigen::Vector3f{1.0,0.5,1.0},
          Eigen::Vector3f{0.0,0.5,1.0},

          Eigen::Vector3f{0.0,0.5,1.0},
          Eigen::Vector3f{0.0,0.5,0.0},


          Eigen::Vector3f{0.0,0.0,0.5},
          Eigen::Vector3f{0.0,1.0,0.5},

          Eigen::Vector3f{0.0,0.0,0.5},
          Eigen::Vector3f{1.0,0.0,0.5},

          Eigen::Vector3f{1.0,0.0,0.5},
          Eigen::Vector3f{1.0,1.0,0.5},

          Eigen::Vector3f{0.0,1.0,0.5},
          Eigen::Vector3f{1.0,1.0,0.5},


          Eigen::Vector3f{0.5,0.0,0.0},
          Eigen::Vector3f{0.5,1.0,0.0},

          Eigen::Vector3f{0.5,0.0,0.0},
          Eigen::Vector3f{0.5,0.0,1.0},

          Eigen::Vector3f{0.5,1.0,0.0},
          Eigen::Vector3f{0.5,1.0,1.0},

          Eigen::Vector3f{0.5,0.0,1.0},
          Eigen::Vector3f{0.5,1.0,1.0},

      };

      GLfloat data[cubeSize*2];
      float scale = 25;
      for(int i = 0; i < cubeSize; i++){
        Eigen::Vector3f before = Eigen::Vector3f{
          cube[i][0]-0.5,
          cube[i][1]-0.5,
          cube[i][2]-0.5
        };
        Eigen::Vector3f F = MVP.model_to_screen_direction(before);
        F[0] = 2.0f * F[0] * scale / MVP.width() - 1.0f;
        F[1] = 2.0f * F[1] * scale / MVP.height() - 1.0f;

        F[0] = F[0] + 1.0 + 0.825;
        F[1] = F[1] + 1.0 + 0.825;

        data[2*i] = F[0];
        data[2*i+1] = F[1];
      }

//      Eigen::Vector3f F1 = MVP.model_to_screen(Eigen::Vector3f{1.0,0.0,0.0});

//      float dist = std::min (MVP.width()/(10), MVP.height()/(10)) / 2.0;
//      float dist = std::min (MVP.width()/(10*abs (F[0])), MVP.height()/(10*abs (F[1]))) / 2.0;
//      F[0] = std::round (MVP.width() /1.1 + F[0]*dist);
//      F[1] = std::round (MVP.height() /1.1 + F[1]*dist);
//
//      float dist1 = std::min (MVP.width()/(10*abs (F1[0])), MVP.height()/(10*abs (F1[1]))) / 2.0;
//      F1[0] = std::round (MVP.width() /1.1 + F1[0]*dist1);
//      F1[1] = std::round (MVP.height() /1.1 + F1[1]*dist1);
//      F[0] = std::round (F[0] - MVP.x_position()) - 0.5f;
//      F[1] = std::round (F[1] - MVP.y_position()) + 0.5f;

//      GLfloat data [] = {
//          F[0], F[1],
//          F1[0], F1[1],
//          -F[0], F[1],
//          -F1[0], -F1[1]
//      };
//      GLfloat data [] = {
//          F[0], -1.0f,
//          F[0], 1.0f,
//          -1.0f, F[1],
//          1.0f, F[1]
//      };
//      DEBUG("data{"+str(data[0])+", "+str(data[1])+", "+str(data[2])+", "+str(data[3])+", "+str(data[4])+", "+str(data[5])+", "+str(data[6])+", "+str(data[7]));
      gl::BufferData (gl::ARRAY_BUFFER, sizeof(data), data, gl::STATIC_DRAW);

      gl::DepthMask (gl::TRUE_);
      gl::Disable (gl::BLEND);
      gl::LineWidth (50.0);

      program.start();
      gl::DrawArrays (gl::LINES, 0, cubeSize*2);
      program.stop();
    }



  }
}


