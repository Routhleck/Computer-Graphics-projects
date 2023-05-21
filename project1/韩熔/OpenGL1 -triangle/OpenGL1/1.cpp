#include <glad/glad.h>  // OpenGL函数指针加载库
#include <GLFW/glfw3.h>  // GLFW库

#include <iostream>  // 标准输入输出流库

// 窗口大小变化的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// 处理输入的函数
void processInput(GLFWwindow* window);

// 窗口的宽度和高度
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 顶点着色器代码
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// 第一个片段着色器代码，输出橙色
const char* fragmentShader1Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

// 第二个片段着色器代码，输出黄色
const char* fragmentShader2Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";

int main()
{
    // 初始化GLFW库
    glfwInit();

    // 配置OpenGL版本为3.3，并且使用核心模式(Core Profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // Mac系统需要的额外配置
#endif

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 将当前窗口的上下文设置为主上下文
    glfwMakeContextCurrent(window);

    // 设置窗口大小变化的回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 加载所有OpenGL函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 创建顶点着色器对象并编译
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // 创建第一个片段着色器对象并编译
    unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderOrange, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShaderOrange);
    // 创建第二个片段着色器对象并编译
    unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderYellow, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragmentShaderYellow);

    // 创建第一个着色器程序对象并链接着色器
    unsigned int shaderProgramOrange = glCreateProgram();
    glAttachShader(shaderProgramOrange, vertexShader);
    glAttachShader(shaderProgramOrange, fragmentShaderOrange);
    glLinkProgram(shaderProgramOrange);

    // 创建第二个着色器程序对象并链接着色器
    unsigned int shaderProgramYellow = glCreateProgram();
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, fragmentShaderYellow);
    glLinkProgram(shaderProgramYellow);

    // 释放不再需要的着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaderOrange);
    glDeleteShader(fragmentShaderYellow);

    // 设置第一个三角形的顶点数据
    float firstTriangle[] = {
        -0.9f, -0.5f, 0.0f,  // 左下角
        -0.0f, -0.5f, 0.0f,  // 右下角
        -0.45f, 0.5f, 0.0f   // 顶部
    };

    // 设置第二个三角形的顶点数据
    float secondTriangle[] = {
        0.0f, -0.5f, 0.0f,  // 左下角
        0.9f, -0.5f, 0.0f,  // 右下角
        0.45f, 0.5f, 0.0f   // 顶部
    };

    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);  // 生成两个顶点数组对象
    glGenBuffers(2, VBOs);  // 生成两个顶点缓冲对象

    // 设置第一个三角形的顶点属性
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 设置第二个三角形的顶点属性
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 处理输入
        processInput(window);

        // 渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 使用第一个着色器程序并
        glUseProgram(shaderProgramOrange);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);  // 绘制第一个三角形

        // 使用第二个着色器程序并绘制第二个三角形
        glUseProgram(shaderProgramYellow);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);  // 绘制第二个三角形

        // 交换缓冲区并获取用户输入
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 清理分配的资源
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgramOrange);
    glDeleteProgram(shaderProgramYellow);

    // 终止GLFW库，释放内存
    glfwTerminate();
    return 0;
}

// 处理所有输入，查询用户是否按下/释放相关按键或移动鼠标等
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// 窗口大小变化的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);  // 根据窗口大小调整视口大小
}
