#pragma once

// glfw3
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// glm
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>
#include "../glm/gtc/constants.hpp"
#include "../glm/gtc/matrix_transform.hpp"

// Include
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>

// Containers
#include <array>
#include <set>
#include <unordered_set>
#include <vector>

// Define
#define TITLE "scop"
#define WIDTH 1600
#define HEIGHT 900
#define TWO_PI glm::two_pi<float>()
