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
private:
    static float value;
    static float scalevalue;
    static float rotatevalue;
    static float translatevalue;
    QTimer timer;
    GLuint VBO, VAO, EBO,shaderProgram,texture,texture1;
public slots:
    void timeOut();
};

#endif // WIDGET_H
