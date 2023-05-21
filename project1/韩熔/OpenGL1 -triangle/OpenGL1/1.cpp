#include <glad/glad.h>  // OpenGL����ָ����ؿ�
#include <GLFW/glfw3.h>  // GLFW��

#include <iostream>  // ��׼�����������

// ���ڴ�С�仯�Ļص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// ��������ĺ���
void processInput(GLFWwindow* window);

// ���ڵĿ�Ⱥ͸߶�
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// ������ɫ������
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// ��һ��Ƭ����ɫ�����룬�����ɫ
const char* fragmentShader1Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

// �ڶ���Ƭ����ɫ�����룬�����ɫ
const char* fragmentShader2Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";

int main()
{
    // ��ʼ��GLFW��
    glfwInit();

    // ����OpenGL�汾Ϊ3.3������ʹ�ú���ģʽ(Core Profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // Macϵͳ��Ҫ�Ķ�������
#endif

    // ��������
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // ����ǰ���ڵ�����������Ϊ��������
    glfwMakeContextCurrent(window);

    // ���ô��ڴ�С�仯�Ļص�����
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ��������OpenGL����ָ��
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ����������ɫ�����󲢱���
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // ������һ��Ƭ����ɫ�����󲢱���
    unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderOrange, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShaderOrange);
    // �����ڶ���Ƭ����ɫ�����󲢱���
    unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderYellow, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragmentShaderYellow);

    // ������һ����ɫ���������������ɫ��
    unsigned int shaderProgramOrange = glCreateProgram();
    glAttachShader(shaderProgramOrange, vertexShader);
    glAttachShader(shaderProgramOrange, fragmentShaderOrange);
    glLinkProgram(shaderProgramOrange);

    // �����ڶ�����ɫ���������������ɫ��
    unsigned int shaderProgramYellow = glCreateProgram();
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, fragmentShaderYellow);
    glLinkProgram(shaderProgramYellow);

    // �ͷŲ�����Ҫ����ɫ������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaderOrange);
    glDeleteShader(fragmentShaderYellow);

    // ���õ�һ�������εĶ�������
    float firstTriangle[] = {
        -0.9f, -0.5f, 0.0f,  // ���½�
        -0.0f, -0.5f, 0.0f,  // ���½�
        -0.45f, 0.5f, 0.0f   // ����
    };

    // ���õڶ��������εĶ�������
    float secondTriangle[] = {
        0.0f, -0.5f, 0.0f,  // ���½�
        0.9f, -0.5f, 0.0f,  // ���½�
        0.45f, 0.5f, 0.0f   // ����
    };

    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);  // �������������������
    glGenBuffers(2, VBOs);  // �����������㻺�����

    // ���õ�һ�������εĶ�������
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ���õڶ��������εĶ�������
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {
        // ��������
        processInput(window);

        // ��Ⱦָ��
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ʹ�õ�һ����ɫ������
        glUseProgram(shaderProgramOrange);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);  // ���Ƶ�һ��������

        // ʹ�õڶ�����ɫ�����򲢻��Ƶڶ���������
        glUseProgram(shaderProgramYellow);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);  // ���Ƶڶ���������

        // ��������������ȡ�û�����
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ����������Դ
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgramOrange);
    glDeleteProgram(shaderProgramYellow);

    // ��ֹGLFW�⣬�ͷ��ڴ�
    glfwTerminate();
    return 0;
}

// �����������룬��ѯ�û��Ƿ���/�ͷ���ذ������ƶ�����
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// ���ڴ�С�仯�Ļص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);  // ���ݴ��ڴ�С�����ӿڴ�С
}
