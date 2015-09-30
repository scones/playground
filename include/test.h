/*
 * test.h
 *
 *  Created on: 29.09.2015
 *      Author: scones
 */

#ifndef INCLUDE_TEST_H_
#define INCLUDE_TEST_H_


class test {

  public:


  test() = default;
  virtual ~test() {};

  virtual void init() = 0;
  virtual void shutdown() = 0;

  virtual void render() = 0;

  protected:


};


#endif /* INCLUDE_TEST_H_ */

