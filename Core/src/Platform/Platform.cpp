#include "Platform/Platform.h"
#include "GLFW/glfw3.h"
#include <stdexcept>

namespace Core::Platform {
	void Init() {
		#if defined (DM_PLATFORM_WINDOWS)
					// Some Code
			if (!glfwInit()) {
				throw std::runtime_error("Failed to initialize GLFW");
			}
		#elif defined (DM_PLATFORM_MACOS)
					// Some Code
			if (!glfwInit()) {
				throw std::runtime_error("Failed to initialize GLFW");
			}
		#elif defined (DM_PLATFORM_LINUX)
					// Some Code
			if (!glfwInit()) {
				throw std::runtime_error("Failed to initialize GLFW");
			}
		#endif
	}

	void Shutdown() {
		#if defined (DM_PLATFORM_WINDOWS)
				// Some Code
			glfwTerminate();
		#elif defined (DM_PLATFORM_MACOS)
				// Some Code
			glfwTerminate();
		#elif defined (DM_PLATFORM_LINUX)
				// Some Code
			glfwTerminate();
		#endif
	}

	bool IsMacOS() {
		#if defined (DM_PLATFORM_MACOS)
			return true;
		#else
			return false;
		#endif
	}
}