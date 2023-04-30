#ifndef LVE_LVE_WINDOW_HPP
#define LVE_LVE_WINDOW_HPP

#include <string>
#include <GLFW/glfw3.h>

namespace lve {

class LveWindow{
public:
    LveWindow(int w, int h, std::string name);
    ~LveWindow();

    LveWindow(const LveWindow &) = delete;
    LveWindow& operator=(const LveWindow &) = delete;

    void initWindow();
    bool shouldClose();
    void createWindowSurface(VkInstance instance, VkSurfaceKHR * surface);

private:
    const int width;
    const int height;

    GLFWwindow * window;
    std::string windowName;
};


}

#endif // LVE_LVE_WINDOW_HPP