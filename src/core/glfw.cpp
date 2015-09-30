/*
 * glfw.cpp
 *
 *  Created on: 16.08.2013
 *      Author: scn
 */


#include "core/glfw.h"
#include "core/error/error.h"


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


namespace core {

  void glfw::init() {
    if (!glfwInit()) {
      throw core::error::error("glfwInit() failed");
    }
  }


  void glfw::shutdown() {
    glfwTerminate();
  }

}

