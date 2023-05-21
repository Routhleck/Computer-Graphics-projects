#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#define _USE_MATH_DEFINES // 声明使用数学常量

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main()
{
    // 初始化GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口对象
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Circle", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 将窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);

    // 设置窗口大小变化时的回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 编译顶点着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // 编译片段着色器
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // 创建着色器程序并链接顶点和片段着色器
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 释放着色器资源
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 创建圆形顶点数据
    float radius = 0.5f;
    int numSegments = 360;
    //有横纵坐标所以要×2
    float* circleVertices = new float[numSegments * 2];

    for (int i = 0; i < numSegments; ++i)
    {
        // 计算当前顶点在圆上的角度 i/360 * 2pi
        float theta = static_cast<float>(i) * (2.0f * static_cast<float>(3.14159265358979323846) / static_cast<float>(numSegments));
        // 计算当前顶点的 x 和 y 坐标  轴坐标系
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        // 将顶点坐标存入数组
        circleVertices[i * 2] = x;
        circleVertices[i * 2 + 1] = y;
    }

    // 生成顶点数组对象和顶点缓冲对象
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 绑定顶点数组对象和顶点缓冲对象
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 将圆形顶点数据传输到顶点缓冲对象
    glBufferData(GL_ARRAY_BUFFER, numSegments * 2 * sizeof(float), circleVertices, GL_STATIC_DRAW);
    // 设置顶点属性指针
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 释放圆形顶点数据的内存
    delete[] circleVertices;

    while (!glfwWindowShouldClose(window))
    {
        // 处理输入
        processInput(window);

        // 清空颜色缓冲区
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 使用着色器程序
        glUseProgram(shaderProgram);
        // 绑定顶点数组对象
        glBindVertexArray(VAO);
        // 绘制圆形
        glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments);

        // 交换前后缓冲区
        glfwSwapBuffers(window);
        // 处理事件
        glfwPollEvents();
    }

    // 释放顶点数组对象和顶点缓冲对象
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // 释放着色器程序
    glDeleteProgram(shaderProgram);

    // 终止GLFW，清理资源
    glfwTerminate();
    return 0;
}

// 窗口大小变化回调函数 调用framebuffer_size_callback函数来设置视口的大小，确保它与窗口的新大小匹配。
//当视口的大小发生变化时，OpenGL会自动调整渲染的区域，以适应新的视口大小。这意味着绘制的图形将会自动进行缩放和拉伸，以填充整个视口。因此，圆的形状也会相应地进行缩放和拉伸，以适应新的窗口大小。
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 设置视口大小
    glViewport(0, 0, width, height);
}

// 处理输入函数
void processInput(GLFWwindow* window)
{
    // 按下ESC键时关闭窗口
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

