/*
 * main.cpp
 *
 *  Created on: 28.09.2015
 *      Author: scones
 */

#include "app.h"


#include <cinttypes>
#include <iostream>
#include <stdexcept>


extern app* pplayground;


int main(std::int32_t argc, char** argv) {

  try {
    pplayground = new app(argc, argv);
    pplayground->run();
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}

