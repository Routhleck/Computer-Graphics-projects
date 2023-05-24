代码主要结构与流程为:

1. 初始化和配置OpenGL上下文。
2. 创建窗口并设置窗口回调函数。
3. 加载OpenGL函数指针。
4. 定义三角形的顶点坐标和着色器代码。
5. 创建和编译顶点着色器和片段着色器。
6. 创建着色器程序并链接顶点着色器和片段着色器。
7. 创建和绑定顶点缓冲对象（VBO）和顶点数组对象（VAO）。
8. 设置顶点属性指针。
9. 在渲染循环中，处理用户输入、清空颜色缓冲区、使用着色器程序绘制三角形。
10. 清理和释放相关资源。

这段代码通过使用OpenGL库实现了创建窗口和绘制三角形的功能， 最终产生的三角形的的三点坐标由定义的定点坐标决定

```C++
float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
};
```

三角形颜色由 `FragColor` 的值决定，它表示每个片段的颜色。在这里，颜色被设置为 `(1.0f, 0.5f, 0.2f, 1.0f)`，表示一个橙色

```C++
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";
```

窗口的背景颜色由`glClearColor` 函数决定，参数 `(0.2f, 0.3f, 0.3f, 1.0f)` 表示背景颜色为一种深灰色。然后，`glClear` 函数将颜色缓冲区清空为指定的背景颜色。

```C++
// 渲染循环
while (!glfwWindowShouldClose(window)) {
    // 输入处理
    processInput(window);

    // 清空颜色缓冲区并设置背景颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 绘制三角形
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // 交换缓冲区和处理事件
    glfwSwapBuffers(window);
    glfwPollEvents();
}
```

其次第二个其实是两个三角形组成的矩形，使用了元素缓冲对象EBO

```C++
// 两个三角形的顶点
float vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
};

unsigned int indices[] = {
        // 注意索引从0开始! 
        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        // 这样可以由下标代表顶点组合成矩形
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
};
```

可以根据`indices`来分别绘制

最后绑定了按键切换绘制线框和绘制填充，使用按键`T`可以切换

```c++
bool ifLine = true;
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		if (!ifLine) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			ifLine = !ifLine;
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			ifLine = !ifLine;
		}
	}
}
```

