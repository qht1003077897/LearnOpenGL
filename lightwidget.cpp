#include "lightwidget.h"

LightWidget::LightWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setMouseTracking(true);
    this->resize(800,600);
}

LightWidget::~LightWidget()
{
}

void LightWidget::initializeGL()
{
    // 为当前环境初始化OpenGL函数
    initializeOpenGLFunctions();
    const char *vertexShader =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = projection * view * model * vec4(aPos.x,aPos.y,aPos.z, 1.0);\n"
        "}\0";

    const char *fragmentShaderBox =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "uniform vec3 objectColor;\n"
            "uniform vec3 lightColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(lightColor * objectColor,1.0);\n"
            "}\n\0";
    const char *fragmentShaderLight =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0);\n"
        "}\n\0";
    int success;
    char infoLog[512];
    int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShader, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        qDebug() << "ERROR::vertexShader::VERTEX::COMPILATION_FAILED\n" << infoLog;
    };
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderBox, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        qDebug() << "ERROR::fragmentShader::VERTEX::COMPILATION_FAILED\n" << infoLog;
    };

    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        qDebug()  << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog ;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragmentShader);

    //正方体顶点
    const float m_vertexs[]  = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
 };
    glGenVertexArrays(1,&VAOBOX);
    glBindVertexArray(VAOBOX);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(m_vertexs),m_vertexs,GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);


    int vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader2, 1, &vertexShader, NULL);
    glCompileShader(vertexShader2);
    glGetShaderiv(vertexShader2, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader2, 512, NULL, infoLog);
        qDebug() << "ERROR::vertexShader2::VERTEX::COMPILATION_FAILED\n" << infoLog;
    };
    // fragment shader
    int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderLight, NULL);
    glCompileShader(fragmentShader2);
    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
        qDebug() << "ERROR::fragmentShader2::VERTEX::COMPILATION_FAILED\n" << infoLog;
    };

    // link shaders
    shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader2);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
        qDebug()  << "ERROR::SHADER::PROGRAM2::LINKING_FAILED\n" << infoLog ;
    }

    glDeleteShader(vertexShader2);
    glDeleteShader(fragmentShader2);

    glGenVertexArrays(1,&VAOLIGHT);
    glBindVertexArray(VAOLIGHT);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);


//    glBindBuffer(GL_ARRAY_BUFFER,0);//取消VAO和VBO绑定
//    glBindVertexArray(0);
}

void LightWidget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //绘制

    glUseProgram(shaderProgram);

    int objectColor = glGetUniformLocation(shaderProgram, "objectColor");
    glUniform3f(objectColor, 1.0f, 0.5f,0.31f);
    int lightColor = glGetUniformLocation(shaderProgram, "lightColor");
    glUniform3f(lightColor, 1.0f, 1.0f,1.0f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(25.0f), glm::vec3(1.0, 0.3, 0.2));
    model = glm::scale(model, glm::vec3(0.5f,0.5f, 0.5f));
    int m = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(m, 1, GL_FALSE, glm::value_ptr(model));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    int v = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(v, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
    int pro = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(pro, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAOBOX);
    glDrawArrays(GL_TRIANGLES, 0, 36);



    glUseProgram(shaderProgram2);

    glm::mat4 view2 = glm::mat4(1.0f);
    view2 = glm::translate(view2, glm::vec3(0.0f, 0.0f, -3.0f));
    int v2 = glGetUniformLocation(shaderProgram2, "view");
    glUniformMatrix4fv(v2, 1, GL_FALSE, glm::value_ptr(view2));

    glm::mat4 projection2 = glm::mat4(1.0f);
    projection2 = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
    int pro2 = glGetUniformLocation(shaderProgram2, "projection");
    glUniformMatrix4fv(pro2, 1, GL_FALSE, glm::value_ptr(projection2));

    glm::mat4 modellight         = glm::mat4(1.0f);
    glm::vec3 lightPos         = glm::vec3(1.0f);
    lightPos = glm::vec3(0.8f, 0.8f, 0.5f);
    modellight = glm::rotate(modellight, glm::radians(25.0f), glm::vec3(1.0, 0.3, 0.2));
    modellight = glm::translate(modellight, lightPos);
    modellight = glm::scale(modellight, glm::vec3(0.2f));
    int m2 = glGetUniformLocation(shaderProgram2, "model");
    glUniformMatrix4fv(m2, 1, GL_FALSE, glm::value_ptr(modellight));
    glBindVertexArray(VAOLIGHT);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   // glBindVertexArray(0); // no need to unbind it every time
}

void LightWidget::resizeGL(int w, int h){
    glViewport(0, 0, w, h);
}

