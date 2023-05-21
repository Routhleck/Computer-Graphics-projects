#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#define _USE_MATH_DEFINES // ����ʹ����ѧ����

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
    // ��ʼ��GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // �������ڶ���
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Circle", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // �����ڵ�����������Ϊ��ǰ�̵߳���������
    glfwMakeContextCurrent(window);

    // ���ô��ڴ�С�仯ʱ�Ļص�����
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ��ʼ��GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ���붥����ɫ��
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // ����Ƭ����ɫ��
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // ������ɫ���������Ӷ����Ƭ����ɫ��
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // �ͷ���ɫ����Դ
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ����Բ�ζ�������
    float radius = 0.5f;
    int numSegments = 360;
    //�к�����������Ҫ��2
    float* circleVertices = new float[numSegments * 2];

    for (int i = 0; i < numSegments; ++i)
    {
        // ���㵱ǰ������Բ�ϵĽǶ� i/360 * 2pi
        float theta = static_cast<float>(i) * (2.0f * static_cast<float>(3.14159265358979323846) / static_cast<float>(numSegments));
        // ���㵱ǰ����� x �� y ����  ������ϵ
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        // �����������������
        circleVertices[i * 2] = x;
        circleVertices[i * 2 + 1] = y;
    }

    // ���ɶ����������Ͷ��㻺�����
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // �󶨶����������Ͷ��㻺�����
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // ��Բ�ζ������ݴ��䵽���㻺�����
    glBufferData(GL_ARRAY_BUFFER, numSegments * 2 * sizeof(float), circleVertices, GL_STATIC_DRAW);
    // ���ö�������ָ��
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // �ͷ�Բ�ζ������ݵ��ڴ�
    delete[] circleVertices;

    while (!glfwWindowShouldClose(window))
    {
        // ��������
        processInput(window);

        // �����ɫ������
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ʹ����ɫ������
        glUseProgram(shaderProgram);
        // �󶨶����������
        glBindVertexArray(VAO);
        // ����Բ��
        glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments);

        // ����ǰ�󻺳���
        glfwSwapBuffers(window);
        // �����¼�
        glfwPollEvents();
    }

    // �ͷŶ����������Ͷ��㻺�����
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // �ͷ���ɫ������
    glDeleteProgram(shaderProgram);

    // ��ֹGLFW��������Դ
    glfwTerminate();
    return 0;
}

// ���ڴ�С�仯�ص����� ����framebuffer_size_callback�����������ӿڵĴ�С��ȷ�����봰�ڵ��´�Сƥ�䡣
//���ӿڵĴ�С�����仯ʱ��OpenGL���Զ�������Ⱦ����������Ӧ�µ��ӿڴ�С������ζ�Ż��Ƶ�ͼ�ν����Զ��������ź����죬����������ӿڡ���ˣ�Բ����״Ҳ����Ӧ�ؽ������ź����죬����Ӧ�µĴ��ڴ�С��
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // �����ӿڴ�С
    glViewport(0, 0, width, height);
}

// �������뺯��
void processInput(GLFWwindow* window)
{
    // ����ESC��ʱ�رմ���
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

