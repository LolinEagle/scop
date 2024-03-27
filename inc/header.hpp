#pragma once

// glfw3
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// glm
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include "../glm/gtc/constants.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtx/hash.hpp"

// Include
#include <chrono>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <png.h>
#include <sstream>
#include <stdexcept>

// Containers
#include <array>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Define
#define TITLE "scop"
#define WIDTH 1600
#define HEIGHT 900
#define WIDTH_HALF 800
#define HEIGHT_HALF 450
#define TWO_PI glm::two_pi<float>()
#define MAX_FRAMES_IN_FLIGHT 2
#define MAX_LIGHTS 16
