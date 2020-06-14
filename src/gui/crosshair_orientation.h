//
// Created by alex on 09/03/2020.
//

#ifndef MRTRIX3_CROSSHAIR_ORIENTATION_H
#define MRTRIX3_CROSSHAIR_ORIENTATION_H

#include "types.h"

#include "gui/opengl/gl.h"
#include "gui/opengl/shader.h"

namespace MR
{
  namespace GUI
  {


    class ModelViewProjection;


    class CrosshairOrientation
    { NOMEMALIGN
    public:
      CrosshairOrientation() { }
      void render (const Eigen::Vector3f& focus,
                   const ModelViewProjection& MVP) const;
    protected:
      mutable GL::VertexBuffer VB;
      mutable GL::VertexArrayObject VAO;
      mutable GL::Shader::Program program;
      mutable GL::Shader::Program program2;
      mutable GL::Shader::Program program3;
      mutable GL::Shader::Program program4;
    };




  }
}


#endif // MRTRIX3_CROSSHAIR_ORIENTATION_H
