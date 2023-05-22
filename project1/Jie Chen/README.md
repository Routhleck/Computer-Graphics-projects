# Computer-Graphics-projects



北京交通大学 计算机图形学 OpenGL实验

本次实验在成功配置了OpenGL相关第三方库之后，学习了窗口的创建以及初始三角形的渲染后，进行了矩形的创建，主要流程如下：

1. 初始化GLFW并配置OpenGL上下文
2. GLAD加载所有必要的OpenGL函数指针
3. 使用`glCreateShader`创建顶点着色器和片段着色器对象。然后，使用`glShaderSource`和`glCompileShader`附加和编译着色器源代码。如果存在编译错误，将错误信息打印到控制台。然后，使用`glCreateProgram`创建着色器程序对象，并使用`glAttachShader`将顶点着色器和片段着色器附加到程序上。
4. 使用`glCreateShader`创建顶点着色器和片段着色器对象。然后，使用`glShaderSource`和`glCompileShader`附加和编译着色器源代码。如果存在编译错误，将错误信息打印到控制台。然后，使用`glCreateProgram`创建着色器程序对象，并使用`glAttachShader`将顶点着色器和片段着色器附加到程序上。
5. 使用顶点和索引数据定义顶点数据。使用`glGenBuffers`和`glGenVertexArrays`生成顶点缓冲对象（VBO）、元素缓冲对象（EBO）和顶点数组对象（VAO）。然后，使用`glBufferData`将顶点和索引数据绑定到它们各自的缓冲区。使用`glVertexAttribPointer`设置顶点属性，并使用`glEnableVertexAttribArray`启用它们。
6. 进入主渲染循环。循环内部处理用户输入，清除屏幕，并使用`glDrawElements`渲染矩形。

