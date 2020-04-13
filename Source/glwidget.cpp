#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    setDefaults();
}

void GLWidget::setDefaults()
{
    Mx = My = Mz = 0.0;
    mAngleX = mAngleY = mAngleZ = 0.0;
    nearClipping = 1.0f;
    farClipping = 1000.0f;
    fillMode = true;
    zoomFactor = 1.0;
    Cx = Cy = Cz = 0.0;
    mouseModel = true;
    colorR = colorG = colorB = 1.0;
    vLength = hLength = 0.5;
}

void GLWidget::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearDepth(1.0f);
    //Anti-Aliasing
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-vLength * zoomFactor, vLength * zoomFactor, -hLength * zoomFactor, hLength * zoomFactor, nearClipping, farClipping);;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //Camera Positioning
    gluLookAt(Cx, Cy, (3 * objRad) + Cz, 0, 0, 0, 0, 1, 0);
    //Polygon Mode
    toggleFillMode();
    //Model Transformations (Reverse Order)
    glTranslatef(Mx, My, Mz);
    glRotatef(mAngleX, 1.0, 0.0, 0.0);
    glRotatef(mAngleY, 0.0, 1.0, 0.0);
    glRotatef(mAngleZ, 0.0, 0.0, 1.0);
    glTranslatef(-centerX, -centerY, -centerZ);
    //Color
    glColor3f(colorR, colorG, colorB);
    //Model Creation
    glBegin(GL_TRIANGLES);
        for(int i = 0; i < face.length(); i++)
        {
            glVertex3f(vertex[face[i][0] - 1][0], vertex[face[i][0] - 1][1], vertex[face[i][0] - 1][2]);
            glVertex3f(vertex[face[i][1] - 1][0], vertex[face[i][1] - 1][1], vertex[face[i][1] - 1][2]);
            glVertex3f(vertex[face[i][2] - 1][0], vertex[face[i][2] - 1][1], vertex[face[i][2] - 1][2]);
        }
    glEnd();
}

void GLWidget::updateGL()
{
    update();
}

void GLWidget::toggleFillMode()
{
    if(fillMode == true)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPosition = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dX = event->x() - lastPosition.x();
    int dY = event->y() - lastPosition.y();
    if(event->buttons() & Qt::LeftButton)
    {
        if(mouseModel)
        {
            mAngleX = setAngle(mAngleX + 1 * dY);
            mAngleY = setAngle(mAngleY + 1 * dX);
        }
        else
        {
            Cx = Cx + dX;
            Cy = Cy + dY;
        }
    }
    else if(event->buttons() & Qt::RightButton)
    {
        if(mouseModel)
        {
            mAngleX = setAngle(mAngleX + 1 * dY);
            mAngleZ = setAngle(mAngleZ - 1 * dX);
        }
        else
        {
            Cx = Cx  + dX;
            Cz = Cz + dY;
        }
    }
    lastPosition = event->pos();
    emit sendAngles();
    emit sendCameraT();
    updateGL();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
     emit sendZoom(event->delta() / 25);
}

GLfloat GLWidget::setAngle(GLfloat angle)
{
    if(angle > 360)
        return angle - 360;
    else if (angle < -360)
        return angle + 360;
    else
        return angle;
}
