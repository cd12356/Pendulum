#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
using namespace std;

// Settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;
double lastTime = glfwGetTime();

//PHYSICS CONSTANTS

const float g = 9.81f;
const float L = 0.6f;
const float pi = 3.14159265f;
const float rho = 1.225;
const float m = 0.05f;
const float C_d = 0.47f;

//PENDULUM INITIAL CONDIOTIONS

float theta = 0.523f;
float angular_velocity = 0.0f;
float angular_acceleration = 0.0f;

//FUNCTION TO HELP WITH THE DRAWING OF THE CIRCLE

void drawBob(float cx, float cy, float r) {

    glColor3f(0.2f, 0.6f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);

    for (int i = 0; i <= 30; i++) {
        float angle = 2.0f * pi * i / 30.0f;
        glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
    }
    glEnd();
}


int main() {
    // 1. Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 2. Create Window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Project", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 3. Main Loop
    while (!glfwWindowShouldClose(window)) {
        // (Esc to close)
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);


        // TIME DELTA CALCULATION

        double currentTime = glfwGetTime();
        float dt = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        if (dt > 0.05f) dt = 0.05f;
        

        if (angular_velocity >= 0) angular_acceleration = (-g / L) * sin(theta) - 0.5f * (rho/m) * pi * pow(0.05f, 2.0f) * C_d * L * pow(angular_velocity, 2.0f);
        if (angular_velocity < 0) angular_acceleration = (-g / L) * sin(theta) + 0.5f * (rho/m) * pi * pow(0.05f, 2.0f) * C_d * L * pow(angular_velocity, 2.0f);
        
        angular_velocity = angular_velocity + angular_acceleration * dt;
        theta = theta + angular_velocity * dt;

        // POLAR TO CARTESIAN CONVERSION

        float x_0 = 0.0f;
        float y_0 = 0.0f;

        float x = x_0 + L * sin(theta);
        float y = y_0 - L * cos(theta);

        // Rendering
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set background to dark grey
        glClear(GL_COLOR_BUFFER_BIT);


        // DRAWINGS

        glBegin(GL_LINES);
            glVertex2f(x_0, y_0);
            glVertex2f(x, y);
        glEnd();
        
        drawBob(x, y, 0.05f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 4. Cleanup
    glfwTerminate();
    return 0;
}
