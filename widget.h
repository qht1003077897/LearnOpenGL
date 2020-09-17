#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QVector>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <qevent.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class QOpenGLTexture;

class Widget : public QOpenGLWidget,protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
protected:
    void initializeGL() override;
    void paintGL()override;
    void resizeGL(int w, int h)override;
    void keyPressEvent(QKeyEvent *k)override;
    void mouseMoveEvent(QMouseEvent *e)override;
private:
    float pitch = 0.0f,yaw = -90.0f;
    bool firstMouse = true;
    float lastX = 400, lastY = 300;//鼠标上一帧的坐标位置，初始化为中央
    int64_t deltaTime = 0; // 当前帧与上一帧的时间差
    int64_t lastFrame = 0; // 上一帧的时间
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    static float value;
    static float scalevalue;
    static float rotatevalue;
    static float translatevalue;
    QTimer timer;
    GLuint VBO, VAO, VBO2, VAO2,EBO,shaderProgram,texture,texture1;
public slots:
    void timeOut();
};

#endif // WIDGET_H
