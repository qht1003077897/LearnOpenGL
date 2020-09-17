#ifndef LIGHTWIDGET_H
#define LIGHTWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QVector>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class QOpenGLTexture;

class LightWidget : public QOpenGLWidget,protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT
public:
    explicit LightWidget(QWidget *parent = 0);
    ~LightWidget();
protected:
    void initializeGL() override;
    void paintGL()override;
    void resizeGL(int w, int h)override;
private:
    GLuint VBO, VAOBOX,VAOLIGHT ,shaderProgram,shaderProgram2;
};


#endif // LIGHTWIDGET_H
