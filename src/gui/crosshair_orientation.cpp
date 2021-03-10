//
// Created by alex on 09/03/2020.
//

#include "gui/crosshair_orientation.h"
#include "gui/projection.h"

#include "gui/pugiconfig.hpp"
#include "gui/pugixml.hpp"

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <string>

// The following instruction is *very* ugly, but Michel Simatic could 
// not find another way to have build script compile pugixml.cpp and 
// then link the object file
#include "gui/pugixml.cpp"

namespace MR
{
  namespace GUI
  {
    
    // Definitions of transformations, compute_normalize_vector and init_transformations are
    // exactly the same as the ones in tractogram.cpp. It is probably possible to factorize,
    // but Michel Simatic did not have to search for that.
    std::vector<std::vector<std::vector<double>>> transformations;

    void compute_normalized_vector(double& a, double& b, double& c, pugi::xml_node childOV) {
      std::array<double,3> v{ childOV.attribute("x").as_double(),
			      childOV.attribute("y").as_double(),
			      childOV.attribute("z").as_double() };
      double norm = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
      a = v[0] / norm;
      b = v[1] / norm;
      c = v[2] / norm;
    }

    void init_transformations() {
      static bool init_done = false;
      if (init_done) {
	return;
      }
      init_done = true;

      pugi::xml_document doc;
      pugi::xml_parse_result result = doc.load_file(getenv("MRTRIX_XML"));

      if (result.status != pugi::status_ok) {
	if (!getenv("MRTRIX_XML")) {
	  std::cerr << "ERROR : \"MRTRIX_XML\" environnement variable is not defined." << std::endl;
	} else {
	  std::cerr << "ERROR : Coud not open XML file named \"" << getenv("MRTRIX_XML") << "\" (according to \"MRTRIX_XML\" environnement variable)" << std::endl;
	}
	exit(1);
      }

      for (auto child : doc.child("Transformations").children()) {
	if (strcmp(child.name(), "Rotation") == 0) {
	  double a, b, c;
	  compute_normalized_vector(a, b, c, child.child("OrientationVector_of_axis"));

	  double phi = child.child("Angle").attribute("degrees").as_double() * M_PI / 180.0;

	  // We initialize rotation matrix according to http://www.pierreaudibert.fr/tra/Rotation3D.pdf
	  // NB : We do not use std::array<std::array<double,3>, 3>, because it does not initialize properly on Visual Studio
	  std::vector<std::vector<double>> m{
					     {(1 - cos(phi)) * a * a + cos(phi),     (1 - cos(phi)) * a * b - sin(phi) * c, (1 - cos(phi)) * a * c + sin(phi) * b},
					     {(1 - cos(phi)) * b * a + sin(phi) * c, (1 - cos(phi)) * b * b + cos(phi),     (1 - cos(phi)) * b * c - sin(phi) * a},
					     {(1 - cos(phi)) * c * a - sin(phi) * b, (1 - cos(phi)) * c * b + sin(phi) * a, (1 - cos(phi)) * c * c + cos(phi)}
	  };

	  transformations.push_back(std::move(m));
	}
	else if (strcmp(child.name(), "CentralSymmetry") == 0) {
	  // We initialize symmetry matrix according to computations we have done
	  std::vector<std::vector<double>> m{
					     {-1.0,  0.0,  0.0},
					     { 0.0, -1.0,  0.0},
					     { 0.0,  0.0, -1.0}
	  };

	  transformations.push_back(std::move(m));
	}
	else if (strcmp(child.name(), "AxisSymmetry") == 0) {
	  double a, b, c;
	  compute_normalized_vector(a, b, c, child.child("OrientationVector_of_axis"));

	  // We initialize symmetry matrix :
	  //   - Let P be the matrix that changes referential so that z axis becomes axis of the symmetry.
	  //     (see  http://www.pierreaudibert.fr/tra/Rotation3D.pdf for values inside P)
	  //   - In this referential, symmetry matrix is S = Matrix([[-1,0,0],[0,-1,0],[0,0,1]])
	  //   - Thus m = P*S*P.T (where P.T is transposition of P)
	  std::vector<std::vector<double>> m{
					     {2.0 * a * a - 1.0, 2.0 * a * b,       2.0 * a * c},
					     {2.0 * a * b,       2.0 * b * b - 1.0, 2.0 * b * c},
					     {2.0 * a * c,       2.0 * b * c,       2.0 * c * c - 1.0}
	  };

	  transformations.push_back(std::move(m));
	}
	else if (strcmp(child.name(), "PlaneSymmetry") == 0) {
	  double a, b, c;
	  compute_normalized_vector(a, b, c, child.child("OrientationVector_of_axis_normalToPlane"));

	  // We initialize symmetry matrix :
	  //   - Let P be the matrix that changes referential so that z axis becomes axis normal to plane of the symmetry.
	  //     (see  http://www.pierreaudibert.fr/tra/Rotation3D.pdf for values inside P)
	  //   - In this referential, symmetry matrix is S = Matrix([[1,0,0],[0,1,0],[0,0,-1]])
	  //   - Thus m = P*S*P.T (where P.T is transposition of P)
	  std::vector<std::vector<double>> m{
					     {-2.0 * a * a + 1.0, -2.0 * a * b,       -2.0 * a * c},
					     {-2.0 * a * b,       -2.0 * b * b + 1.0, -2.0 * b * c},
					     {-2.0 * a * c,       -2.0 * b * c,       -2.0 * c * c + 1.0}
	  };

	  transformations.push_back(std::move(m));
	}
      }
    }

    std::vector<double> colorAxis(int axis) {
      static std::map<int, std::vector<double>> cache;
      auto search = cache.find(axis);
      if (search != cache.end()) {
        return search->second;
      }
      // We do the computation for this axis.
      std::cout << "----------- colorAxis(" << axis << ") ----------- " << std::endl;
      std::vector<double> color{0.0, 0.0, 0.0};
      color[axis] = 1;
      std::cout << "Before conversion : " << color[0] << " , " << color[1] << " , " << color[2] << std::endl;

      // Convert RGB to CMYB explained (in French) at https://fr.wikipedia.org/wiki/Quadrichromie#Conversion_de_RVB_vers_CMJN
      double norm = sqrt(color[0] * color[0] + color[1] * color[1] + color[2] * color[2]);
      double a = abs(color[0] / norm);
      double b = abs(color[1] / norm);
      double c = abs(color[2] / norm);

      double minv = 1-a < 1-b ? (1-a < 1-c ? 1-a : 1-c) : (1-b < 1-c ? 1-b : 1-c);

      if (minv == 1.0) {
	color[0] = 0.0;
	color[1] = 0.0;
	color[2] = 0.0;
      } else {
	color[0] = 1 - a - minv;
	color[1] = 1 - b - minv;
	color[2] = 1 - c - minv;
      }
      std::cout << "After RGB-CYMB    : " << color[0] << " , " << color[1] << " , " << color[2] << std::endl;

      // Apply colour transformations
      std::vector<double> color_tmp{color};
      init_transformations();
      for (const auto &ms : transformations) {
	for (unsigned int i=0 ; i<color.size() ; i++) {
	  color[i] -= 0.5;
	}
	for (unsigned int i=0 ; i<color.size() ; i++) {
	  color_tmp[i] = ms[i][0] * color[0] + ms[i][1] * color[1] + ms[i][2]* color[2];
	}
	for (unsigned int i=0 ; i<color.size() ; i++) {
	  color[i] = color_tmp[i];
	  color[i] += 0.5;
	  color[i] = color[i] < 1.0 ? (color[i] > 0.0 ? color[i] : 0.0) : 1.0;
	}
      }
      std::cout << "After transforms  : " << color[0] << " , " << color[1] << " , " << color[2] << std::endl;

      // Convert colors within a printable range thanks to MRTRIX_IMAGE file
      if (!getenv("MRTRIX_IMAGE")) {
	std::cerr << "ERROR : \"MRTRIX_IMAGE\" environnement variable is not defined." << std::endl;
      } 
      std::ifstream infile(getenv("MRTRIX_IMAGE"));

      std::streampos pos = 3*(int(color[0]*255)+int(color[1]*255)*256+int(color[2]*255)*65536 + 18);
      infile.seekg(pos);
      char colors[3];
      infile.read(colors, 3);
      for (unsigned int i=0 ; i<color.size() ; i++) {
	color[i] = ((unsigned char)colors[i]) / 255.0;
      }
      std::cout << "After printable   : " << color[0] << " , " << color[1] << " , " << color[2] << std::endl;
      
      // Cache this color
      cache[axis] = color;

      return color;
    }

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
	  std::vector<double> col = colorAxis(0);
	  std::string source =
	    "out vec4 color;\n"
	    "void main () {\n";
	  source += "  color = vec4 (" + std::to_string(col[0]) + "," + std::to_string(col[1]) + "," + std::to_string(col[2]) + ", 1.0);\n" + 
	    "}\n";
          GL::Shader::Fragment fragment_shader (source);
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
	  std::vector<double> col = colorAxis(1);
	  std::string source =
	    "out vec4 color;\n"
	    "void main () {\n";
	  source += "  color = vec4 (" + std::to_string(col[0]) + "," + std::to_string(col[1]) + "," + std::to_string(col[2]) + ", 1.0);\n" + 
	    "}\n";
          GL::Shader::Fragment fragment_shader (source);
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
	  std::vector<double> col = colorAxis(2);
	  std::string source =
	    "out vec4 color;\n"
	    "void main () {\n";
	  source += "  color = vec4 (" + std::to_string(col[0]) + "," + std::to_string(col[1]) + "," + std::to_string(col[2]) + ", 1.0);\n" + 
	    "}\n";
          GL::Shader::Fragment fragment_shader (source);
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
        DEBUG("scale " + str(scale));
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


