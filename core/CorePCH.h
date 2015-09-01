// glad (OpenGL binding)
#include <glad/glad.h>

// C++ Standard Library
#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <utility>

// Lua
#include <lua.hpp>

// GLFW
#include <GLFW/glfw3.h>

// Internal libraries
#include "IntegralTypes.h"
#include "MathUtil.h"
#include "Vector.h"
#include "Macro.h"

#pragma warning(disable:4996) // Error: This function or variable may be unsafe.
#pragma warning(disable:4100) // unreferenced formal parameter
