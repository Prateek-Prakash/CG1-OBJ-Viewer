#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QtMath>
#include <QMouseEvent>
#include "gl/GLU.h"

#pragma comment(lib, "glu32")

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    bool fillMode;
    bool mouseModel;

    GLdouble Mx, My, Mz;
    GLdouble zoomFactor;

    GLfloat centerX, centerY, centerZ;    
    GLfloat mAngleX, mAngleY, mAngleZ;
    GLfloat nearClipping;
    GLfloat farClipping;
    GLfloat objRad;
    GLfloat Cx, Cy, Cz;
    GLfloat colorR, colorG, colorB;
    GLfloat vLength, hLength;

    QPoint lastPosition;

    QVector<QVector<GLfloat>> vertex;
    QVector<QVector<int>> face;

    GLWidget(QWidget *parent = 0);

    void setDefaults();
    void initializeGL();
    void paintGL();
    void updateGL();
    void toggleFillMode();

    GLfloat setAngle(GLfloat angle);

private slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

signals:
    void sendAngles();
    void sendZoom(int X);
    void sendCameraT();
};

#endif // GLWIDGET_H
