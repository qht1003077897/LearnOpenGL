#include "widget.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QDebug>
#include <QTimer>
#include <iostream>
#include <QDateTime>
#include <QDir>


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
        "uniform float mixvalue;\n"
        "void main()\n"
        "{\n"
        "   FragColor = texture(texture1, TexCoord);"
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
    float floorVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.51f,  5.0f,  1.0f, 0.0f,
        -5.0f, -0.51f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.51f, -5.0f,  0.0f, 1.0f,

         5.0f, -0.51f,  5.0f,  1.0f, 0.0f,
        -5.0f, -0.51f, -5.0f,  0.0f, 1.0f,
         5.0f, -0.51f, -5.0f,  1.0f, 1.0f
    };

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 0);
    QString path1 = ":/new/prefix1/3.png";
    QImage images(path1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, images.width(), images.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, images.bits());
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0); // 手动设置
    glGenerateMipmap(GL_TEXTURE_2D);


    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(m_vertexs),m_vertexs,GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER,0);//取消VAO和VBO绑定
    glBindVertexArray(0);



    //-------------------------------------------地板
    glGenTextures(1, &texture1);
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
    glGenerateMipmap(GL_TEXTURE_2D);

    glGenVertexArrays(1,&VAO2);
    glBindVertexArray(VAO2);
    glGenBuffers(1,&VBO2);
    glBindBuffer(GL_ARRAY_BUFFER,VBO2);
    glBufferData(GL_ARRAY_BUFFER,sizeof(floorVertices),floorVertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}





void Widget::paintGL()
{
    int64_t currentFrame = QDateTime::currentMSecsSinceEpoch();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //绘制
    glUseProgram(shaderProgram);


    int viewLoc = glGetUniformLocation(shaderProgram, "view");

    //摄像机绕y轴旋转/场景绕y轴旋转
    glm::mat4 viewlook         = glm::mat4(1.0f);
    viewlook = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    viewlook = glm::translate(viewlook, glm::vec3(0.0f, 0.0f, -4.0f));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE,  glm::value_ptr(viewlook));

    glm::mat4 projection    = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), this->width()/(float)this->height(), 0.1f, 100.0f);
    int proLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(proLoc, 1, GL_FALSE, glm::value_ptr(projection));

    int modelLoc = glGetUniformLocation(shaderProgram, "model");

    glBindTexture(GL_TEXTURE_2D, texture1);
    glBindVertexArray(VAO2);
    model = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);


    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);


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
    update();
}

void Widget::keyPressEvent( QKeyEvent *k )
{
    float cameraSpeed = 0.001f * deltaTime;
     qDebug() << "cameraSpeed:" << cameraSpeed;
    if(k->key() == Qt::Key_W)
    {
        qDebug("Key_W");
        cameraPos += cameraSpeed * cameraFront;
    }
    else if(k->key() == Qt::Key_S)
    {
        qDebug("Key_S");
        cameraPos -= cameraSpeed * cameraFront;
    }
    else if(k->key() == Qt::Key_D)
    {
        qDebug("Key_A");
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    else if(k->key() == Qt::Key_A)
    {
         qDebug("Key_D");
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

     update();
}


void Widget::mouseMoveEvent(QMouseEvent *e)
{
    int xpos = e->pos().x();
    int ypos = e->pos().y();
    if(firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw   += xoffset;
        pitch += yoffset;
        qDebug() << "yaw:" << yaw;
        qDebug() << "pitch:" << pitch;
        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front        = glm::vec3(1.0f);
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
        update();
}
