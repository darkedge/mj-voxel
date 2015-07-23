// glad (OpenGL binding)
#include <glad/glad.h>

// C++ Standard Library
#include <cassert>
#include <cstdlib>
#include <functional>
#include <utility>
#include <memory>

// Lua
#include <lua.hpp>

// Mono
#pragma warning(push)
#pragma warning(disable:4510) // default constructor could not be generated
#pragma warning(disable:4512) // assignment operator could not be generated
#pragma warning(disable:4610) // struct can never be instantiated - user defined constructor required
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#pragma warning(pop)

// GLFW
#include <GLFW/glfw3.h>

// Internal libraries
#include "IntegralTypes.h"
#include "MathUtil.h"
#include "Vector.h"
#include "Macro.h"

#pragma warning(disable:4996) // Error: This function or variable may be unsafe.
#pragma warning(disable:4100) // unreferenced formal parameter
