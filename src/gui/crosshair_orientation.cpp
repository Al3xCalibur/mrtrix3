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

      if (!program2) {
          GL::Shader::Vertex vertex_shader (
                  "layout(location=0) in vec2 pos;\n"
                  "// orientation\n"
                  "void main () {\n"
                  "  gl_Position = vec4 (pos, 0.0, 1.0);\n"
                  "}\n");
          GL::Shader::Fragment fragment_shader (
                  "out vec4 color;\n"
                  "void main () {\n"
                  "  color = vec4 (0.6484375, 0.309433962, 0.581132075, 1.0);\n"
                  "}\n");
          program2.attach (vertex_shader);
          program2.attach (fragment_shader);
          program2.link();
      }

      if (!program3) {
          GL::Shader::Vertex vertex_shader (
                  "layout(location=0) in vec2 pos;\n"
                  "// orientation\n"
                  "void main () {\n"
                  "  gl_Position = vec4 (pos, 0.0, 1.0);\n"
                  "}\n");
          GL::Shader::Fragment fragment_shader (
                  "out vec4 color;\n"
                  "void main () {\n"
                  "  color = vec4 (0.953125, 0.8984375, 0.0, 1.0);\n"
                  "}\n");
          program3.attach (vertex_shader);
          program3.attach (fragment_shader);
          program3.link();
      }

      if (!program4) {
          GL::Shader::Vertex vertex_shader (
                  "layout(location=0) in vec2 pos;\n"
                  "// orientation\n"
                  "void main () {\n"
                  "  gl_Position = vec4 (pos, 0.0, 1.0);\n"
                  "}\n");
          GL::Shader::Fragment fragment_shader (
                  "out vec4 color;\n"
                  "void main () {\n"
                  "  color = vec4 (0.41509434, 0.750943396, 0.818867925, 1.0);\n"
                  "}\n");
          program4.attach (vertex_shader);
          program4.attach (fragment_shader);
          program4.link();
      }


        int cubeSize = 24;
//      Eigen::Vector3f F = MVP.model_to_screen (focus);
      Eigen::Vector3f carreRouge[] = {
          Eigen::Vector3f{0.0, 0.5, 0.0},
          Eigen::Vector3f{1.0, 0.5, 0.0},

          Eigen::Vector3f{1.0, 0.5, 0.0},
          Eigen::Vector3f{1.0, 0.5, 1.0},

          Eigen::Vector3f{1.0, 0.5, 1.0},
          Eigen::Vector3f{0.0, 0.5, 1.0},

          Eigen::Vector3f{0.0, 0.5, 1.0},
          Eigen::Vector3f{0.0, 0.5, 0.0}
      };

      Eigen::Vector3f carreVert[] = {
          Eigen::Vector3f{0.0, 0.0, 0.5},
          Eigen::Vector3f{0.0, 1.0, 0.5},

          Eigen::Vector3f{0.0, 0.0, 0.5},
          Eigen::Vector3f{1.0, 0.0, 0.5},

          Eigen::Vector3f{1.0, 0.0, 0.5},
          Eigen::Vector3f{1.0, 1.0, 0.5},

          Eigen::Vector3f{0.0, 1.0, 0.5},
          Eigen::Vector3f{1.0, 1.0, 0.5}
      };

      Eigen::Vector3f carreBleu[] = {
          Eigen::Vector3f{0.5,0.0,0.0},
          Eigen::Vector3f{0.5,1.0,0.0},

          Eigen::Vector3f{0.5,0.0,0.0},
          Eigen::Vector3f{0.5,0.0,1.0},

          Eigen::Vector3f{0.5,1.0,0.0},
          Eigen::Vector3f{0.5,1.0,1.0},

          Eigen::Vector3f{0.5,0.0,1.0},
          Eigen::Vector3f{0.5,1.0,1.0}
      };

      GLfloat dataRouge[cubeSize*2/3];
      GLfloat dataVert[cubeSize*2/3];
      GLfloat dataBleu[cubeSize*2/3];
      float scale = 25;
      for(int i = 0; i < cubeSize/3; i++){
        Eigen::Vector3f beforeRouge = Eigen::Vector3f{
          carreRouge[i][0]-0.5,
          carreRouge[i][1]-0.5,
          carreRouge[i][2]-0.5
        };

        Eigen::Vector3f beforeVert = Eigen::Vector3f{
          carreVert[i][0]-0.5,
          carreVert[i][1]-0.5,
          carreVert[i][2]-0.5
        };

        Eigen::Vector3f beforeBleu = Eigen::Vector3f{
          carreBleu[i][0]-0.5,
          carreBleu[i][1]-0.5,
          carreBleu[i][2]-0.5
        };

        Eigen::Vector3f FRouge = MVP.model_to_screen_direction(beforeRouge);
        FRouge[0] = 2.0f * FRouge[0] * scale / MVP.width() - 1.0f;
        FRouge[1] = 2.0f * FRouge[1] * scale / MVP.height() - 1.0f;

        FRouge[0] = FRouge[0] + 1.0 + 0.825;
        FRouge[1] = FRouge[1] + 1.0 + 0.825 - 0.15;

        dataRouge[2*i] = FRouge[0];
        dataRouge[2*i+1] = FRouge[1];


        Eigen::Vector3f FVert = MVP.model_to_screen_direction(beforeVert);
        FVert[0] = 2.0f * FVert[0] * scale / MVP.width() - 1.0f;
        FVert[1] = 2.0f * FVert[1] * scale / MVP.height() - 1.0f;

        FVert[0] = FVert[0] + 1.0 + 0.825;
        FVert[1] = FVert[1] + 1.0 + 0.825 - 0.15;

        dataVert[2*i] = FVert[0];
        dataVert[2*i+1] = FVert[1];


        Eigen::Vector3f FBleu = MVP.model_to_screen_direction(beforeBleu);
        FBleu[0] = 2.0f * FBleu[0] * scale / MVP.width() - 1.0f;
        FBleu[1] = 2.0f * FBleu[1] * scale / MVP.height() - 1.0f;

        FBleu[0] = FBleu[0] + 1.0 + 0.825;
        FBleu[1] = FBleu[1] + 1.0 + 0.825 - 0.15;

        dataBleu[2*i] = FBleu[0];
        dataBleu[2*i+1] = FBleu[1];
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
      gl::BufferData (gl::ARRAY_BUFFER, sizeof(dataRouge), dataRouge, gl::STATIC_DRAW);

      gl::DepthMask (gl::TRUE_);
      gl::Disable (gl::BLEND);
      gl::LineWidth (50.10);

      program2.start();
      gl::DrawArrays (gl::LINES, 0, cubeSize*2/3);
      program2.stop();

      gl::BufferData (gl::ARRAY_BUFFER, sizeof(dataVert), dataVert, gl::STATIC_DRAW);

      gl::DepthMask (gl::TRUE_);
      gl::Disable (gl::BLEND);
      gl::LineWidth (50.10);

      program3.start();
      gl::DrawArrays (gl::LINES, 0, cubeSize*2/3);
      program3.stop();

      gl::BufferData (gl::ARRAY_BUFFER, sizeof(dataBleu), dataBleu, gl::STATIC_DRAW);

      gl::DepthMask (gl::TRUE_);
      gl::Disable (gl::BLEND);
      gl::LineWidth (50.1000);

      program4.start();
      gl::DrawArrays (gl::LINES, 0, cubeSize*2/3);
      program4.stop();

    }



  }
}


