#include "widget.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QDebug>
#include <QTimer>
#include <iostream>
#include <QDateTime>
#include <QDir>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


float Widget::value = 0.0f;
float Widget::scalevalue = 0.0f;
float Widget::rotatevalue = 0.0f;
float Widget::translatevalue = -1.0f;
Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    this->resize(800,800);
    timer.setInterval(16);
    connect(&timer,&QTimer::timeout,this,&Widget::timeOut);
    timer.start();
}

Widget::~Widget()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
}

void Widget::initializeGL()
{
    // 为当前环境初始化OpenGL函数
    initializeOpenGLFunctions();
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "layout (location = 2) in vec2 aTexCoord;\n"
        "out vec3 ourColor;\n"
        "out vec2 TexCoord;\n"
        //"uniform mat4 transform;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = projection * view * model * vec4(aPos.x,aPos.y,aPos.z, 1.0);\n"
            "ourColor = aColor;\n"
            "TexCoord=aTexCoord;"
        "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 ourColor;\n"
        "in vec2 TexCoord;\n"
        "//uniform sampler2D ourTexture;\n"
        "uniform sampler2D texture1;\n"
        "uniform sampler2D texture2;\n"
        "uniform float mixvalue;\n"
        "void main()\n"
        "{\n"
        "   //FragColor = vec4(ourColor,1.0);\n"//正常颜色
        "   //FragColor = texture(ourTexture, TexCoord)*vec4(1.0,0.5,0.5,1.0);\n"//纹理叠加色彩
        "   FragColor = mix(texture(texture1, TexCoord)*vec4(1.0,1.0,1.0,1.0), texture(texture2, TexCoord)*vec4(1.0,1.0,1.0,1.0), mixvalue);"
        "}\n\0";

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // 顶点位置

//    const float m_vertexs[]  = {
//        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
//             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
//             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
//            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
//            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
//    };

    //正方体顶点
    const float m_vertexs[]  = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
 };

//    const unsigned int indices[] = {  // note that we start from 0!
//        0, 1, 2, // first Triangle
//        0, 2, 3   // second Triangle
//    };

    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(m_vertexs),m_vertexs,GL_STATIC_DRAW);

//    glGenBuffers(1, &EBO);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 0);
    QString path1 = ":/new/prefix1/3.png";
    QImage images(path1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, images.width(), images.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, images.bits());


    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    QImage images2(":/new/prefix1/4.png");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, images2.width(), images2.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, images2.bits());

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0); // 手动设置
    glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1); // 手动设置
    glGenerateMipmap(GL_TEXTURE_2D);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(6 * sizeof(float)));
//    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER,0);//取消VAO和VBO绑定
    glBindVertexArray(0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}




void Widget::paintGL()
{
    //定义10个世界坐标
    static glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f),
      glm::vec3( 2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f,  2.0f, -2.5f),
      glm::vec3( 1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //绘制
    glUseProgram(shaderProgram);

    // 更新uniform颜色或者mix纹理混合
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "mixvalue");
    glUniform1f(vertexColorLocation, value);

    //glm::mat4 trans = glm::mat4(1.0f);//创建单位矩阵;
    //trans = glm::translate(trans, glm::vec3(translatevalue, translatevalue, 0.0f));
    //trans = glm::rotate(trans, rotatevalue, glm::vec3(1.0, 0.0, 0.0));
    //trans = glm::scale(trans, glm::vec3(scalevalue,scalevalue, scalevalue));
    // 更新uniform 变化矩阵
    //unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    // make sure to initialize matrix to identity matrix first
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    //model = glm::rotate(model, rotatevalue, glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));
    projection = glm::perspective(glm::radians(45.0f), this->width()/(float)this->height(), 0.1f, 100.0f);

    //int modelLoc = glGetUniformLocation(shaderProgram, "model");
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE,  glm::value_ptr(view));

    //利用观察矩阵也可以实现在世界坐标中平移出多个物体（和下面的for循环一样）
//    glDrawArrays(GL_TRIANGLES, 0, 36);
//    glm::mat4 view2          = glm::mat4(1.0f);
//    view2 = glm::translate(view2, glm::vec3(1.0f, 0.0f, -4.0f));
//    int viewLoc2 = glGetUniformLocation(shaderProgram, "view");
//    glUniformMatrix4fv(viewLoc2, 1, GL_FALSE,  glm::value_ptr(view2));
//    glDrawArrays(GL_TRIANGLES, 0, 36);
//    glm::mat4 view3          = glm::mat4(1.0f);
//    view3 = glm::translate(view3, glm::vec3(0.0f, 1.0f, -4.0f));
//    int viewLoc3 = glGetUniformLocation(shaderProgram, "view");
//    glUniformMatrix4fv(viewLoc3, 1, GL_FALSE,  glm::value_ptr(view3));
//    glDrawArrays(GL_TRIANGLES, 0, 36);

    int proLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(proLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glBindVertexArray(VAO);
    for(unsigned int i = 0; i < 10; i++)
    {
      glm::mat4 model         = glm::mat4(1.0f);
      int modelLoc = glGetUniformLocation(shaderProgram, "model");
      model = glm::translate(model, cubePositions[i]);
      //float angle = 20.0f * i;
      if(i % 3 == 0)
      {
        model = glm::rotate(model, rotatevalue, glm::vec3(1.0f, 0.3f,0.5f));
      }
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   // glBindVertexArray(0); // no need to unbind it every time
}

void Widget::resizeGL(int w, int h){
    glViewport(0, 0, w, h);
}

void Widget::timeOut()
{
    value += 0.0067777777f;
    rotatevalue += 0.042f; // 3秒从左下角到右上角共150帧，转2Π（弧度制），每帧的角度 = 2Π / 150
    scalevalue += 0.00677777f;//3秒从左下角到右上角共150帧，从0缩放到1  每帧的缩放值 = 1 / 150
    translatevalue += 0.01333333f;//3秒从左下角到右上角共150帧，从-1平移到1  每帧的缩放值 = 2 / 150

    if(translatevalue >= 1.0f)
    {
        translatevalue = -1.0f;
        value = 0.0f;
        rotatevalue = 0.0f;
        scalevalue = 0.0f;
    }

    //纹理叠加变化
    update();
    //三角形变色
    //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    //glUniform4f(vertexColorLocation, 0.0f, value + 0.5f, 0.0f, 1.0f);
}
