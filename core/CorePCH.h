// glad (OpenGL binding)
#include <glad/glad.h>

// used for gluErrorString
// requires Glu32.lib
#include <gl/GLU.h>

// C++ Standard Library
#include <cassert>
#include <cstdlib>
#include <utility>

// STL Containers (FIXME: Do not use these)
//#include <fstream>
//#include <vector>

// Mono
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

// GLFW
#include <GLFW/glfw3.h>

// Internal libraries
#include "IntegralTypes.h"
#include "MathUtil.h"
#include "Vector.h"
#include "Macro.h"
