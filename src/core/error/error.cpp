/*
 * error.cpp
 *
 *  Created on: 03.10.2013
 *      Author: scn
 */

#include "core/error/error.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>


namespace core {

  namespace error {

    error::error(std::string const& what) {
      log_error(what);
    }


    void error::log_error(std::string const& what) {
      time_t now;
      time(&now);
      auto ts = gmtime(&now);
      char time_buffer[20];
      strftime(time_buffer, 20, "%Y %m", ts);
      int year, month;
      std::istringstream (time_buffer) >> year >> month;
      strftime(time_buffer, 20, "-%d %H:%M:%S", ts);

      std::ostringstream oss;
      oss << year << '-';
      if (month < 10)
        oss << '0';
      oss << month << time_buffer << "# " << what << std::endl;

      std::ofstream error_log("logs/error.log", std::ios::out | std::ios::app);
      if (error_log.is_open()) {
        error_log << oss.str();
        error_log.flush();
        error_log.close();
      }

  #ifdef DEBUG
      std::cerr << oss.str() << std::flush;
  #endif
    }

  }

}

