## OpenGL实验一

本次实验在完成OpenGL相关第三方库的成功配置基础上，我们完成了窗口的创建和简单三角形的渲染，最终的实验代码主要包含以下步骤：

1. 初始化OpenGL和GLFW库。
2. 创建窗口并设置OpenGL上下文的版本和配置。
3. 加载OpenGL函数指针。
4. 创建和绑定顶点缓冲对象（VBO）和顶点数组对象（VAO），设置顶点属性指针。
5. 创建顶点着色器和片段着色器，并编译、链接成着色器程序。
6. 在渲染循环中，处理输入事件，清空颜色缓冲区，使用着色器程序绘制三角形，交换缓冲区和处理窗口事件。
7. 释放资源，删除VBO、VAO和着色器程序，并终止GLFW库。

通过完成以上步骤，我们可以成功运行程序，创建一个包含简单三角形的窗口，并在窗口中进行渲染。

![result](D:\GitHubRepository\Computer-Graphics-projects\project1\Zengyao Chen\Readme.assets\result.png)