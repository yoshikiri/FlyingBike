#include "Result.h"

Result::Result(GLFWwindow *window): State(window){}

State *Result::update(){
  return this;
}