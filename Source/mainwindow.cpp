#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->openGLWidget, SIGNAL(sendAngles()), this, SLOT(receiveRotation()));
    connect(ui->openGLWidget, SIGNAL(sendZoom(int)), this, SLOT(receiveZoom(int)));
    connect(ui->openGLWidget, SIGNAL(sendCameraT()), this, SLOT(receiveCameraT()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile(QString path)
{
    QFile inputFile(path);
    if (!inputFile.open(QIODevice::ReadOnly))
    {
        //File Error
    }
    else
    {
        ui->openGLWidget->vertex.clear();
        ui->openGLWidget->face.clear();
        int tempCount = 0;
        GLfloat xMin, xMax, yMin, yMax, zMin, zMax;
        while (!inputFile.atEnd())
        {
            QRegExp regEx("(\\ |\\t)");
            QString line = inputFile.readLine();
            QStringList list = line.split(regEx);
            if(list[0] == "v")
            {
                if(tempCount++ == 0)
                {
                    xMin = xMax = list[1].toFloat();
                    yMin = yMax = list[2].toFloat();
                    zMin = zMax = list[3].toFloat();
                }
                else
                {
                    if(list[1].toFloat() < xMin)
                        xMin = list[1].toFloat();
                    if(list[1].toFloat() > xMax)
                        xMax = list[1].toFloat();
                    if(list[2].toFloat() < yMin)
                        yMin = list[2].toFloat();
                    if(list[2].toFloat() > yMax)
                        yMax = list[2].toFloat();
                    if(list[3].toFloat() < zMin)
                        zMin = list[3].toFloat();
                    if(list[3].toFloat() > zMax)
                        zMax = list[3].toFloat();
                }
                QVector<GLfloat> points;
                points.push_back(list[1].toFloat());
                points.push_back(list[2].toFloat());
                points.push_back(list[3].toFloat());
                ui->openGLWidget->vertex.push_back(points);

            }
            if(list[0] == "f")
            {
                QVector<int> points;
                points.push_back(list[1].toInt());
                points.push_back(list[2].toInt());
                points.push_back(list[3].toInt());
                ui->openGLWidget->face.push_back(points);
            }
        }
        inputFile.close();
        on_resetButton_clicked();
        ui->openGLWidget->centerX = (xMin + xMax) / 2.0f;
        ui->openGLWidget->centerY = (yMin + yMax) / 2.0f;
        ui->openGLWidget->centerZ = (zMin + zMax) / 2.0f;
        ui->openGLWidget->objRad = qMax(qMax(xMax - xMin, yMax - yMin), zMax - zMin) / 2.0f;
        ui->openGLWidget->updateGL();
    }
}

void MainWindow::on_sliderMx_sliderMoved(int position)
{
    ui->openGLWidget->Mx = position;
    ui->editMx->setText(QString::number(position));
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderMx_valueChanged(int value)
{
    on_sliderMx_sliderMoved(value);
}

void MainWindow::on_sliderMy_sliderMoved(int position)
{
    ui->openGLWidget->My = position;
    ui->editMy->setText(QString::number(position));
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderMy_valueChanged(int value)
{
    on_sliderMy_sliderMoved(value);
}

void MainWindow::on_sliderMz_sliderMoved(int position)
{
    ui->openGLWidget->Mz = position;
    ui->editMz->setText(QString::number(position));
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderMz_valueChanged(int value)
{
    on_sliderMz_sliderMoved(value);
}

void MainWindow::on_fillModeToggle_clicked()
{
    if(ui->openGLWidget->fillMode == true)
        ui->openGLWidget->fillMode = false;
    else
        ui->openGLWidget->fillMode = true;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderRotate_sliderMoved(int position)
{
    if(ui->radioX->isChecked())
    {
        ui->openGLWidget->mAngleX = ui->openGLWidget->setAngle(position);
        ui->editRx->setText(QString::number(position));
    }
    else if(ui->radioY->isChecked())
    {
        ui->openGLWidget->mAngleY = ui->openGLWidget->setAngle(position);
        ui->editRy->setText(QString::number(position));
    }
    else if(ui->radioZ->isChecked())
    {
        ui->openGLWidget->mAngleZ = ui->openGLWidget->setAngle(position);
        ui->editRz->setText(QString::number(position));
    }
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderRotate_valueChanged(int value)
{
    on_sliderRotate_sliderMoved(value);
}

void MainWindow::on_radioX_clicked()
{
    ui->sliderRotate->setValue(ui->openGLWidget->mAngleX);
}

void MainWindow::on_radioY_clicked()
{
    ui->sliderRotate->setValue(ui->openGLWidget->mAngleY);
}

void MainWindow::on_radioZ_clicked()
{
    ui->sliderRotate->setValue(ui->openGLWidget->mAngleZ);
}

void MainWindow::on_resetButton_clicked()
{
    ui->openGLWidget->setDefaults();
    ui->sliderMx->setValue(0);
    ui->sliderMy->setValue(0);
    ui->sliderMz->setValue(0);
    ui->radioX->setChecked(true);
    ui->radioX->setChecked(false);
    ui->radioX->setChecked(false);
    ui->sliderRotate->setValue(0);
    ui->nearSpinBox->setValue(0.1);
    ui->farSpinBox->setValue(1000.0);
    ui->sliderZoom->setValue(100.0);
    ui->editRy->setText("0");
    ui->editRz->setText("0");
    ui->editCx->setText("0");
    ui->editCy->setText("0");
    ui->editCz->setText("0");
    ui->mouseModel->setChecked(true);
    ui->mouseCamera->setChecked(false);
    ui->sliderCx->setValue(0);
    ui->sliderCy->setValue(0);
    ui->sliderCz->setValue(0);
    ui->spinBoxR->setValue(1.0);
    ui->spinBoxG->setValue(1.0);
    ui->spinBoxB->setValue(1.0);
    ui->vLength->setValue(0.5);
    ui->hLength->setValue(0.5);
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderZoom_sliderMoved(int position)
{
    if(position == 0.0)
        position = 1.0;
    ui->openGLWidget->zoomFactor = 100.0 / position;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderZoom_valueChanged(int value)
{
    on_sliderZoom_sliderMoved(value);
}

void MainWindow::receiveRotation()
{
    if(ui->radioX->isChecked())
        ui->sliderRotate->setValue(ui->openGLWidget->mAngleX);
    else if(ui->radioY->isChecked())
        ui->sliderRotate->setValue(ui->openGLWidget->mAngleY);
    else if(ui->radioZ->isChecked())
        ui->sliderRotate->setValue(ui->openGLWidget->mAngleZ);
    ui->editRx->setText(QString::number(ui->openGLWidget->mAngleX));
    ui->editRy->setText(QString::number(ui->openGLWidget->mAngleY));
    ui->editRz->setText(QString::number(ui->openGLWidget->mAngleZ));
}

void MainWindow::receiveZoom(int X)
{
    ui->sliderZoom->setValue(ui->sliderZoom->value() + X);
}

void MainWindow::on_farSpinBox_valueChanged(double arg1)
{
    ui->openGLWidget->farClipping = arg1;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_nearSpinBox_valueChanged(double arg1)
{
    ui->openGLWidget->nearClipping = arg1;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_openFileButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath(), tr("Object Files (*.obj)"));
    if(!path.isEmpty() && !path.isNull())
    {
        openFile(path);
    }
}

void MainWindow::on_sliderCx_sliderMoved(int position)
{
    ui->openGLWidget->Cx = position;
    ui->editCx->setText(QString::number(position));
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderCx_valueChanged(int value)
{
    on_sliderCx_sliderMoved(value);
}

void MainWindow::on_sliderCy_sliderMoved(int position)
{
    ui->openGLWidget->Cy = position;
    ui->editCy->setText(QString::number(position));
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderCy_valueChanged(int value)
{
    on_sliderCy_sliderMoved(value);
}

void MainWindow::on_sliderCz_sliderMoved(int position)
{
    ui->openGLWidget->Cz = position;
    ui->editCz->setText(QString::number(position));
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderCz_valueChanged(int value)
{
    on_sliderCz_sliderMoved(value);
}

void MainWindow::on_mouseModel_clicked()
{
    ui->openGLWidget->mouseModel = true;
}

void MainWindow::on_mouseCamera_clicked()
{
    ui->openGLWidget->mouseModel = false;
}

void MainWindow::receiveCameraT()
{
    ui->sliderCx->setValue(ui->openGLWidget->Cx);
    ui->sliderCy->setValue(ui->openGLWidget->Cy);
    ui->sliderCz->setValue(ui->openGLWidget->Cz);
}

void MainWindow::on_spinBoxR_valueChanged(double arg1)
{
    ui->openGLWidget->colorR = arg1;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_spinBoxG_valueChanged(double arg1)
{
    ui->openGLWidget->colorG = arg1;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_spinBoxB_valueChanged(double arg1)
{
    ui->openGLWidget->colorB = arg1;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_vLength_valueChanged(double arg1)
{
    ui->openGLWidget->vLength = arg1;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_hLength_valueChanged(double arg1)
{
    ui->openGLWidget->hLength = arg1;
    ui->openGLWidget->updateGL();
}
