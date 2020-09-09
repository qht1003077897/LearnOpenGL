﻿#include "widget.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QDebug>
#include <QTimer>
#include <iostream>
#include <QDateTime>
float Widget::value = 0.0f;
Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    this->resize(400,400);
    timer.setInterval(20);
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
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x,-aPos.y,aPos.z, 1.0);\n"
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
        "   //FragColor = vec4(ourColor,1.0);\n"
        "   //FragColor = texture(ourTexture, TexCoord)*vec4(1.0,0.5,0.5,1.0);\n"
        "   FragColor = mix(texture(texture1, TexCoord)*vec4(0.5,1.0,0.5,1.0), texture(texture2, TexCoord)*vec4(0.5,1.0,0.5,1.0), mixvalue);"
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

    const float m_vertexs[]  = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };

    const unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2, // first Triangle
        0, 2, 3   // second Triangle
    };

    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(m_vertexs),m_vertexs,GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 0);
    QImage images("C:\\Users\\HiWin10\\Pictures\\scene00201.png");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, images.width(), images.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, images.bits());


    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    QImage images2("C:\\Users\\HiWin10\\Pictures\\3.png");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, images2.width(), images2.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, images2.bits());

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0); // 手动设置
    glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1); // 手动设置
    glGenerateMipmap(GL_TEXTURE_2D);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER,0);//取消VAO和VBO绑定
    glBindVertexArray(0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Widget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //绘制
    glUseProgram(shaderProgram);

    // 更新uniform颜色
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "mixvalue");
    glUniform1f(vertexColorLocation, value);

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   // glBindVertexArray(0); // no need to unbind it every time
}

void Widget::resizeGL(int w, int h){
    glViewport(0, 0, w, h);
}

void Widget::timeOut()
{
    if(value >= 1.0f)
    {
        value = 0.0f;
    }
    value += 0.01f;
    //纹理叠加变化
    update();
    //三角形变色
    //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    //glUniform4f(vertexColorLocation, 0.0f, value + 0.5f, 0.0f, 1.0f);
}
