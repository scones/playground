/*
 * fterrorstruct.h
 *
 *  Created on: 11.06.2014
 *      Author: scn
 */

#ifndef FTERRORSTRUCT_H_
#define FTERRORSTRUCT_H_


//namespace freetype {

  extern "C" {
    #include <freetype2/ft2build.h>
    #include <freetype2/freetype.h>
  }

  #undef __FTERRORS_H__
  #define FT_ERRORDEF( e, v, s )  { e, s },
  #define FT_ERROR_START_LIST     {
  #define FT_ERROR_END_LIST       { 0, 0 } };
  const struct {
      int          code;
      const char*  message;
  } FT_Errors[] =
  #include FT_ERRORS_H

//}

#endif /* FTERRORSTRUCT_H_ */

