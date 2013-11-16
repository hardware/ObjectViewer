#include "mainwindow.h"
#include "window.h"

#include "../objects/object3d.h"
#include "../objects/camera.h"
#include "../objects/scene.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_openglArea(new Window),
      m_scene(0),
      m_object3D(0),
      m_camera(0)
{
    resize(1366, 768);
    setCentralWidget(QWidget::createWindowContainer(m_openglArea.data()));

    m_scene    = m_openglArea->getScene();
    m_object3D = m_scene->getObject();
    m_camera   = m_scene->getCamera();

    initializeMenuBar();
    initializeParamsArea();
}

MainWindow::~MainWindow() {}

void MainWindow::initializeMenuBar()
{
    QAction *exitAction = new QAction("&Exit", this);
    exitAction->setShortcut(QKeySequence("Ctrl+Q"));

    QAction *fullscreenAction = new QAction("&Fullscreen", this);
    fullscreenAction->setCheckable(true);
    fullscreenAction->setShortcut(QKeySequence(Qt::Key_F11));

    QMenu *fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(exitAction);

    QMenu *windowMenu = menuBar()->addMenu("&Window");
    windowMenu->addAction(fullscreenAction);

    QObject::connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    QObject::connect(fullscreenAction, SIGNAL(triggered(bool)), this, SLOT(setFullScreen(bool)));
}

void MainWindow::initializeParamsArea()
{
    // ############ DOCK RIGHT ############

    m_dockParamsArea = new QDockWidget("PARAMETERS", this);
    addDockWidget(Qt::RightDockWidgetArea, m_dockParamsArea);

    m_params = new QWidget;
    m_dockParamsArea->setWidget(m_params);

    fpsCounter = new QLCDNumber(2);
    fpsCounter->setSegmentStyle(QLCDNumber::Flat);
    fpsCounter->setFrameStyle(QFrame::Box | QFrame::Sunken);
    fpsCounter->display(0.0);

    QTabWidget* tab = new QTabWidget;

    QWidget* objectTab  = new QWidget;
    QWidget* optionsTab = new QWidget;

    tab->addTab(optionsTab, "Options");
    tab->addTab(objectTab, "Object");

    QVBoxLayout* paramsLayout = new QVBoxLayout;
    paramsLayout->addWidget(fpsCounter);
    paramsLayout->addWidget(tab);
    paramsLayout->addStretch();

    m_params->setLayout(paramsLayout);

    // ############ OPTION TAB ############

    QCheckBox* animate = new QCheckBox("Animate");

    // ############ OPTION TAB - RENDERING MODE GROUPBOX ############

    QRadioButton* fill      = new QRadioButton("fill");
    QRadioButton* wireframe = new QRadioButton("wireframe");
    QRadioButton* points    = new QRadioButton("points");

    fill->setChecked(true);

    QVBoxLayout* renderingModeLayout = new QVBoxLayout;
    renderingModeLayout->addWidget(fill);
    renderingModeLayout->addWidget(wireframe);
    renderingModeLayout->addWidget(points);

    QGroupBox* renderingModeGroupBox = new QGroupBox("Rendering mode");
    renderingModeGroupBox->setLayout(renderingModeLayout);

    // ############ OPTION TAB - PROJECTION TYPE GROUPBOX ############

    QRadioButton* perspective = new QRadioButton("perspective");
    QRadioButton* orthographic = new QRadioButton("orthographic");

    perspective->setChecked(true);

    QVBoxLayout* projectionTypeLayout = new QVBoxLayout;
    projectionTypeLayout->addWidget(perspective);
    projectionTypeLayout->addWidget(orthographic);

    QGroupBox* projectionTypeGroupBox = new QGroupBox("Projection Type");
    projectionTypeGroupBox->setLayout(projectionTypeLayout);

    // ############ OPTION TAB - VIEW GROUPBOX ############

    fovLabel         = new QLabel("FOV :");
    nearPlaneLabel   = new QLabel("Near plane :");
    farPlaneLabel    = new QLabel("Far plane :");
    leftLabel        = new QLabel("Left :");
    rightLabel       = new QLabel("Right :");
    bottomLabel      = new QLabel("Bottom :");
    topLabel         = new QLabel("Top :");

    fovValue         = new QDoubleSpinBox;
    nearPlaneValue   = new QDoubleSpinBox;
    farPlaneValue    = new QDoubleSpinBox;
    leftValue        = new QDoubleSpinBox;
    rightValue       = new QDoubleSpinBox;
    bottomValue      = new QDoubleSpinBox;
    topValue         = new QDoubleSpinBox;

    fovValue->setRange(25.0, 130.0);
    fovValue->setValue(75.0);

    nearPlaneValue->setMinimum(0.1);
    nearPlaneValue->setSingleStep(0.1);
    nearPlaneValue->setValue(0.1);

    farPlaneValue->setRange(1.0, 1024.0);
    farPlaneValue->setValue(1024.0);

    leftLabel->hide();
    rightLabel->hide();
    bottomLabel->hide();
    topLabel->hide();

    leftValue->hide();
    rightValue->hide();
    bottomValue->hide();
    topValue->hide();

    leftValue->setRange(-50.0, 0.0);
    leftValue->setSingleStep(0.5);
    leftValue->setValue(-0.5);

    rightValue->setRange(0.0, 50.0);
    rightValue->setSingleStep(0.5);
    rightValue->setValue(0.5);

    bottomValue->setRange(-50.0, 0.0);
    bottomValue->setSingleStep(0.5);
    bottomValue->setValue(-0.5);

    topValue->setRange(0.0, 50.0);
    topValue->setSingleStep(0.5);
    topValue->setValue(0.5);

    QGridLayout* viewLayout = new QGridLayout;
    viewLayout->addWidget(fovLabel, 0, 0);
    viewLayout->addWidget(fovValue, 0, 1);

    viewLayout->addWidget(nearPlaneLabel, 1, 0);
    viewLayout->addWidget(nearPlaneValue, 1, 1);

    viewLayout->addWidget(farPlaneLabel, 2, 0);
    viewLayout->addWidget(farPlaneValue, 2, 1);

    viewLayout->addWidget(leftLabel, 3, 0);
    viewLayout->addWidget(leftValue, 3, 1);

    viewLayout->addWidget(rightLabel, 4, 0);
    viewLayout->addWidget(rightValue, 4, 1);

    viewLayout->addWidget(bottomLabel, 5, 0);
    viewLayout->addWidget(bottomValue, 5, 1);

    viewLayout->addWidget(topLabel, 6, 0);
    viewLayout->addWidget(topValue, 6, 1);

    QGroupBox* viewGroupBox = new QGroupBox("VIEW");
    viewGroupBox->setLayout(viewLayout);

    // ############ OPTION TAB - CAMERA GROUPBOX ############

    QPushButton* resetCamera = new QPushButton("Reset camera");

    QRadioButton* firstPerson = new QRadioButton("First person");
    QRadioButton* thirdPerson = new QRadioButton("Third person");

    firstPerson->setChecked(true);

    QFrame* hLineCamera1 = new QFrame;
    QFrame* hLineCamera2 = new QFrame;

    hLineCamera1->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    hLineCamera2->setFrameStyle(QFrame::HLine | QFrame::Sunken);

    QLabel* cameraSpeedLabel       = new QLabel("Speed (m/s) : ");
    QLabel* cameraSensitivityLabel = new QLabel("Sensitivity : ");

    QDoubleSpinBox* cameraSpeedValue       = new QDoubleSpinBox;
    QDoubleSpinBox* cameraSensitivityValue = new QDoubleSpinBox;

    cameraSpeedValue->setRange(1.0, 200.0);
    cameraSpeedValue->setValue(200.0);
    cameraSpeedValue->setMaximumSize(60, 20);

    cameraSensitivityValue->setValue(0.2);
    cameraSensitivityValue->setSingleStep(0.05);
    cameraSensitivityValue->setMaximumSize(60, 20);

    QHBoxLayout* cameraSpeedLayout = new QHBoxLayout;
    cameraSpeedLayout->addWidget(cameraSpeedLabel);
    cameraSpeedLayout->addWidget(cameraSpeedValue);

    QHBoxLayout* cameraSensitivityLayout = new QHBoxLayout;
    cameraSensitivityLayout->addWidget(cameraSensitivityLabel);
    cameraSensitivityLayout->addWidget(cameraSensitivityValue);

    QVBoxLayout* cameraLayout = new QVBoxLayout;
    cameraLayout->addWidget(firstPerson);
    cameraLayout->addWidget(thirdPerson);
    cameraLayout->addWidget(hLineCamera1);
    cameraLayout->addLayout(cameraSpeedLayout);
    cameraLayout->addLayout(cameraSensitivityLayout);
    cameraLayout->addWidget(hLineCamera2);
    cameraLayout->addWidget(resetCamera);

    QGroupBox* cameraGroupBox = new QGroupBox("CAMERA");
    cameraGroupBox->setLayout(cameraLayout);

    //############ OBJECT TAB - TRANSLATION GROUPBOX ############

    QSlider* translationX = new QSlider(Qt::Horizontal);
    QSlider* translationY = new QSlider(Qt::Horizontal);
    QSlider* translationZ = new QSlider(Qt::Horizontal);

    QLabel* translationXLabel = new QLabel("X");
    QLabel* translationYLabel = new QLabel("Y");
    QLabel* translationZLabel = new QLabel("Z");

    translationX->setRange(-150, 150);
    translationX->setValue(0);
    translationY->setRange(-150, 150);
    translationY->setValue(0);
    translationZ->setRange(-150, 150);
    translationZ->setValue(0);

    QHBoxLayout* translationXLayout = new QHBoxLayout;
    translationXLayout->addWidget(translationXLabel);
    translationXLayout->addWidget(translationX);

    QHBoxLayout* translationYLayout = new QHBoxLayout;
    translationYLayout->addWidget(translationYLabel);
    translationYLayout->addWidget(translationY);

    QHBoxLayout* translationZLayout = new QHBoxLayout;
    translationZLayout->addWidget(translationZLabel);
    translationZLayout->addWidget(translationZ);

    QVBoxLayout* translationLayout = new QVBoxLayout;
    translationLayout->addLayout(translationXLayout);
    translationLayout->addLayout(translationYLayout);
    translationLayout->addLayout(translationZLayout);

    QGroupBox* translationGroupBox = new QGroupBox("TRANSLATION");
    translationGroupBox->setLayout(translationLayout);

    // ############ OBJECT TAB - ROTATION GROUPBOX ############

    QDial* rotationX = new QDial;
    QDial* rotationY = new QDial;
    QDial* rotationZ = new QDial;

    QLabel* rotationXLabel = new QLabel("X");
    QLabel* rotationYLabel = new QLabel("Y");
    QLabel* rotationZLabel = new QLabel("Z");

    rotationX->setRange(0, 360);
    rotationX->setValue(0);
    rotationY->setRange(0, 360);
    rotationY->setValue(0);
    rotationZ->setRange(0, 360);
    rotationZ->setValue(0);

    rotationXLabel->setAlignment(Qt::AlignCenter);
    rotationYLabel->setAlignment(Qt::AlignCenter);
    rotationZLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout* rotationLayout = new QVBoxLayout;
    rotationLayout->addWidget(rotationXLabel);
    rotationLayout->addWidget(rotationX);
    rotationLayout->addWidget(rotationYLabel);
    rotationLayout->addWidget(rotationY);
    rotationLayout->addWidget(rotationZLabel);
    rotationLayout->addWidget(rotationZ);

    QGroupBox* rotationGroupBox = new QGroupBox("ROTATION");
    rotationGroupBox->setLayout(rotationLayout);

    QVBoxLayout* optionsTabLayout = new QVBoxLayout;
    optionsTabLayout->addWidget(renderingModeGroupBox);
    optionsTabLayout->addWidget(projectionTypeGroupBox);
    optionsTabLayout->addWidget(viewGroupBox);
    optionsTabLayout->addWidget(cameraGroupBox);
    optionsTabLayout->addWidget(animate, 0, Qt::AlignHCenter);
    optionsTabLayout->addStretch();

    optionsTab->setLayout(optionsTabLayout);

    QVBoxLayout* objectTabLayout = new QVBoxLayout;
    objectTabLayout->addWidget(translationGroupBox);
    objectTabLayout->addWidget(rotationGroupBox);
    objectTabLayout->addStretch();

    objectTab->setLayout(objectTabLayout);

    // ############ DOCK LEFT ############

    m_dockMatrixArea = new QDockWidget("MODEL VIEW PROJECTION MATRIX", this);
    addDockWidget(Qt::BottomDockWidgetArea, m_dockMatrixArea);

    m_mvpMatrix = new QWidget;
    m_dockMatrixArea->setWidget(m_mvpMatrix);

    modelMatrix00 = new QLabel("0.00");
    modelMatrix01 = new QLabel("0.00");
    modelMatrix02 = new QLabel("0.00");
    modelMatrix03 = new QLabel("0.00");

    modelMatrix10 = new QLabel("0.00");
    modelMatrix11 = new QLabel("0.00");
    modelMatrix12 = new QLabel("0.00");
    modelMatrix13 = new QLabel("0.00");

    modelMatrix20 = new QLabel("0.00");
    modelMatrix21 = new QLabel("0.00");
    modelMatrix22 = new QLabel("0.00");
    modelMatrix23 = new QLabel("0.00");

    modelMatrix30 = new QLabel("0.00");
    modelMatrix31 = new QLabel("0.00");
    modelMatrix32 = new QLabel("0.00");
    modelMatrix33 = new QLabel("0.00");

    viewMatrix00 = new QLabel("0.00");
    viewMatrix01 = new QLabel("0.00");
    viewMatrix02 = new QLabel("0.00");
    viewMatrix03 = new QLabel("0.00");

    viewMatrix10 = new QLabel("0.00");
    viewMatrix11 = new QLabel("0.00");
    viewMatrix12 = new QLabel("0.00");
    viewMatrix13 = new QLabel("0.00");

    viewMatrix20 = new QLabel("0.00");
    viewMatrix21 = new QLabel("0.00");
    viewMatrix22 = new QLabel("0.00");
    viewMatrix23 = new QLabel("0.00");

    viewMatrix30 = new QLabel("0.00");
    viewMatrix31 = new QLabel("0.00");
    viewMatrix32 = new QLabel("0.00");
    viewMatrix33 = new QLabel("0.00");

    projectionMatrix00 = new QLabel("0.00");
    projectionMatrix01 = new QLabel("0.00");
    projectionMatrix02 = new QLabel("0.00");
    projectionMatrix03 = new QLabel("0.00");

    projectionMatrix10 = new QLabel("0.00");
    projectionMatrix11 = new QLabel("0.00");
    projectionMatrix12 = new QLabel("0.00");
    projectionMatrix13 = new QLabel("0.00");

    projectionMatrix20 = new QLabel("0.00");
    projectionMatrix21 = new QLabel("0.00");
    projectionMatrix22 = new QLabel("0.00");
    projectionMatrix23 = new QLabel("0.00");

    projectionMatrix30 = new QLabel("0.00");
    projectionMatrix31 = new QLabel("0.00");
    projectionMatrix32 = new QLabel("0.00");
    projectionMatrix33 = new QLabel("0.00");

    QGridLayout* modelMatrixLayout      = new QGridLayout;
    QGridLayout* viewMatrixLayout       = new QGridLayout;
    QGridLayout* projectionMatrixLayout = new QGridLayout;

    modelMatrixLayout->setSpacing(15);
    viewMatrixLayout->setSpacing(15);
    projectionMatrixLayout->setSpacing(15);

    modelMatrixLayout->addWidget(modelMatrix00, 0, 0, Qt::AlignHCenter);
    modelMatrixLayout->addWidget(modelMatrix01, 0, 1, Qt::AlignHCenter);
    modelMatrixLayout->addWidget(modelMatrix02, 0, 2, Qt::AlignHCenter);
    modelMatrixLayout->addWidget(modelMatrix03, 0, 3, Qt::AlignHCenter);

    modelMatrixLayout->addWidget(modelMatrix10, 1, 0, Qt::AlignHCenter);
    modelMatrixLayout->addWidget(modelMatrix11, 1, 1, Qt::AlignHCenter);
    modelMatrixLayout->addWidget(modelMatrix12, 1, 2, Qt::AlignHCenter);
    modelMatrixLayout->addWidget(modelMatrix13, 1, 3, Qt::AlignHCenter);

    modelMatrixLayout->addWidget(modelMatrix20, 2, 0, Qt::AlignHCenter);
    modelMatrixLayout->addWidget(modelMatrix21, 2, 1, Qt::AlignHCenter);
    modelMatrixLayout->addWidget(modelMatrix22, 2, 2, Qt::AlignHCenter);
    modelMatrixLayout->addWidget(modelMatrix23, 2, 3, Qt::AlignHCenter);

    modelMatrixLayout->addWidget(modelMatrix30, 3, 0, Qt::AlignHCenter);
    modelMatrixLayout->addWidget(modelMatrix31, 3, 1, Qt::AlignHCenter);
    modelMatrixLayout->addWidget(modelMatrix32, 3, 2, Qt::AlignHCenter);
    modelMatrixLayout->addWidget(modelMatrix33, 3, 3, Qt::AlignHCenter);

    viewMatrixLayout->addWidget(viewMatrix00, 0, 0, Qt::AlignHCenter);
    viewMatrixLayout->addWidget(viewMatrix01, 0, 1, Qt::AlignHCenter);
    viewMatrixLayout->addWidget(viewMatrix02, 0, 2, Qt::AlignHCenter);
    viewMatrixLayout->addWidget(viewMatrix03, 0, 3, Qt::AlignHCenter);

    viewMatrixLayout->addWidget(viewMatrix10, 1, 0, Qt::AlignHCenter);
    viewMatrixLayout->addWidget(viewMatrix11, 1, 1, Qt::AlignHCenter);
    viewMatrixLayout->addWidget(viewMatrix12, 1, 2, Qt::AlignHCenter);
    viewMatrixLayout->addWidget(viewMatrix13, 1, 3, Qt::AlignHCenter);

    viewMatrixLayout->addWidget(viewMatrix20, 2, 0, Qt::AlignHCenter);
    viewMatrixLayout->addWidget(viewMatrix21, 2, 1, Qt::AlignHCenter);
    viewMatrixLayout->addWidget(viewMatrix22, 2, 2, Qt::AlignHCenter);
    viewMatrixLayout->addWidget(viewMatrix23, 2, 3, Qt::AlignHCenter);

    viewMatrixLayout->addWidget(viewMatrix30, 3, 0, Qt::AlignHCenter);
    viewMatrixLayout->addWidget(viewMatrix31, 3, 1, Qt::AlignHCenter);
    viewMatrixLayout->addWidget(viewMatrix32, 3, 2, Qt::AlignHCenter);
    viewMatrixLayout->addWidget(viewMatrix33, 3, 3, Qt::AlignHCenter);

    projectionMatrixLayout->addWidget(projectionMatrix00, 0, 0, Qt::AlignHCenter);
    projectionMatrixLayout->addWidget(projectionMatrix01, 0, 1, Qt::AlignHCenter);
    projectionMatrixLayout->addWidget(projectionMatrix02, 0, 2, Qt::AlignHCenter);
    projectionMatrixLayout->addWidget(projectionMatrix03, 0, 3, Qt::AlignHCenter);

    projectionMatrixLayout->addWidget(projectionMatrix10, 1, 0, Qt::AlignHCenter);
    projectionMatrixLayout->addWidget(projectionMatrix11, 1, 1, Qt::AlignHCenter);
    projectionMatrixLayout->addWidget(projectionMatrix12, 1, 2, Qt::AlignHCenter);
    projectionMatrixLayout->addWidget(projectionMatrix13, 1, 3, Qt::AlignHCenter);

    projectionMatrixLayout->addWidget(projectionMatrix20, 2, 0, Qt::AlignHCenter);
    projectionMatrixLayout->addWidget(projectionMatrix21, 2, 1, Qt::AlignHCenter);
    projectionMatrixLayout->addWidget(projectionMatrix22, 2, 2, Qt::AlignHCenter);
    projectionMatrixLayout->addWidget(projectionMatrix23, 2, 3, Qt::AlignHCenter);

    projectionMatrixLayout->addWidget(projectionMatrix30, 3, 0, Qt::AlignHCenter);
    projectionMatrixLayout->addWidget(projectionMatrix31, 3, 1, Qt::AlignHCenter);
    projectionMatrixLayout->addWidget(projectionMatrix32, 3, 2, Qt::AlignHCenter);
    projectionMatrixLayout->addWidget(projectionMatrix33, 3, 3, Qt::AlignHCenter);

    QGroupBox* modelMatrixGroupBox      = new QGroupBox("MODEL");
    QGroupBox* viewMatrixGroupBox       = new QGroupBox("VIEW");
    QGroupBox* projectionMatrixGroupBox = new QGroupBox("PROJECTION");

    modelMatrixGroupBox->setLayout(modelMatrixLayout);
    viewMatrixGroupBox->setLayout(viewMatrixLayout);
    projectionMatrixGroupBox->setLayout(projectionMatrixLayout);

    modelMatrixGroupBox->setAlignment(Qt::AlignHCenter);
    viewMatrixGroupBox->setAlignment(Qt::AlignHCenter);
    projectionMatrixGroupBox->setAlignment(Qt::AlignHCenter);

    modelMatrixGroupBox->setMinimumWidth(250);
    modelMatrixGroupBox->setMaximumWidth(250);
    viewMatrixGroupBox->setMinimumWidth(250);
    viewMatrixGroupBox->setMaximumWidth(250);
    projectionMatrixGroupBox->setMinimumWidth(250);
    projectionMatrixGroupBox->setMaximumWidth(250);

    QLabel* operator1 = new QLabel("x");
    QLabel* operator2 = new QLabel("x");

    operator1->setMargin(30);
    operator2->setMargin(30);
    operator1->setAlignment(Qt::AlignCenter);
    operator2->setAlignment(Qt::AlignCenter);
    operator1->setFont(QFont("Arial", 14));
    operator2->setFont(QFont("Arial", 14));

    QHBoxLayout* matrixLayout = new QHBoxLayout;
    matrixLayout->addStretch();
    matrixLayout->addWidget(modelMatrixGroupBox);
    matrixLayout->addWidget(operator1);
    matrixLayout->addWidget(viewMatrixGroupBox);
    matrixLayout->addWidget(operator2);
    matrixLayout->addWidget(projectionMatrixGroupBox);
    matrixLayout->addStretch();
    matrixLayout->addStrut(102);

    m_mvpMatrix->setLayout(matrixLayout);

    // ############ SIGNALS/SLOTS ############

    QObject::connect(animate, SIGNAL(stateChanged(int)), m_openglArea.data(), SLOT(checkAnimate(int)));

    // Rendering mode
    QObject::connect(fill,      SIGNAL(toggled(bool)), m_scene, SLOT(toggleFill(bool)));
    QObject::connect(wireframe, SIGNAL(toggled(bool)), m_scene, SLOT(toggleWireframe(bool)));
    QObject::connect(points,    SIGNAL(toggled(bool)), m_scene, SLOT(togglePoints(bool)));

    // Projection type
    QObject::connect(perspective, SIGNAL(toggled(bool)), this, SLOT(setViewProperties(bool)));

    // View
    QObject::connect(fovValue, SIGNAL(valueChanged(double)), this, SLOT(updateFieldOfView(double)));
    QObject::connect(nearPlaneValue, SIGNAL(valueChanged(double)), this, SLOT(updateNearPlane(double)));
    QObject::connect(farPlaneValue, SIGNAL(valueChanged(double)), this, SLOT(updateFarPlane(double)));
    QObject::connect(leftValue, SIGNAL(valueChanged(double)), this, SLOT(updateLeft(double)));
    QObject::connect(rightValue, SIGNAL(valueChanged(double)), this, SLOT(updateRight(double)));
    QObject::connect(bottomValue, SIGNAL(valueChanged(double)), this, SLOT(updateBottom(double)));
    QObject::connect(topValue, SIGNAL(valueChanged(double)), this, SLOT(updateTop(double)));

    // Camera
    QObject::connect(cameraSpeedValue, SIGNAL(valueChanged(double)), m_openglArea.data(), SLOT(setCameraSpeed(double)));
    QObject::connect(cameraSensitivityValue, SIGNAL(valueChanged(double)), m_openglArea.data(), SLOT(setCameraSensitivity(double)));
    QObject::connect(resetCamera, SIGNAL(clicked()), m_camera, SLOT(resetCamera()));

    // Object
    QObject::connect(translationX, SIGNAL(valueChanged(int)), m_object3D, SLOT(setObjectXPosition(int)));
    QObject::connect(translationY, SIGNAL(valueChanged(int)), m_object3D, SLOT(setObjectYPosition(int)));
    QObject::connect(translationZ, SIGNAL(valueChanged(int)), m_object3D, SLOT(setObjectZPosition(int)));

    QObject::connect(rotationX, SIGNAL(valueChanged(int)), m_object3D, SLOT(setObjectXRotation(int)));
    QObject::connect(rotationY, SIGNAL(valueChanged(int)), m_object3D, SLOT(setObjectYRotation(int)));
    QObject::connect(rotationZ, SIGNAL(valueChanged(int)), m_object3D, SLOT(setObjectZRotation(int)));

    // Update framerate
    QObject::connect(m_openglArea.data(), SIGNAL(updateFramerate()), this, SLOT(setFramerate()));

    // Update MVP matrix
    QObject::connect(m_scene, SIGNAL(renderCycleDone()), this, SLOT(updateMatrix()));
}

void MainWindow::setViewProperties(bool state)
{
    if(state)
    {
        m_camera->setProjectionType(Camera::PerspectiveProjection);
        m_scene->resize(m_openglArea->width(), m_openglArea->height());

        fovLabel->show();
        fovValue->show();

        leftLabel->hide();
        rightLabel->hide();
        bottomLabel->hide();
        topLabel->hide();

        leftValue->hide();
        rightValue->hide();
        bottomValue->hide();
        topValue->hide();
    }
    else
    {
        m_camera->setProjectionType(Camera::OrthogonalProjection);
        m_scene->resize(m_openglArea->width(), m_openglArea->height());

        fovLabel->hide();
        fovValue->hide();

        leftLabel->show();
        rightLabel->show();
        bottomLabel->show();
        topLabel->show();

        leftValue->show();
        rightValue->show();
        bottomValue->show();
        topValue->show();
    }
}

void MainWindow::setFullScreen(bool state)
{
    (state) ? showFullScreen() : showNormal();
}

void MainWindow::setFramerate()
{
    static double currentTime = 0;
    static double lastTime    = 0;
    static double average     = 0;

    static int count = 0;

    lastTime = currentTime;

    QTime time;
    currentTime = time.currentTime().msec();

    if(currentTime > lastTime)
    {
         average += 1000.0 / (currentTime - lastTime);
         count++;
    }

    if(count == 30)
    {
        fpsCounter->display(average/count);

        count   = 0;
        average = 0;
    }
}

void MainWindow::updateMatrix()
{
    static int count = 0;

    count++;

    if(count == 15) // Mise à jour des matrices toutes les 1/4 de secondes
    {
        const float* modelMatrixData = m_object3D->modelMatrix().data();
        const float* viewMatrixData  = m_scene->getCamera()->viewMatrix().data();
        const float* projectionMatrixData = m_scene->getCamera()->projectionMatrix().data();

        modelMatrix00->setNum(modelMatrixData[0]);
        modelMatrix01->setNum(modelMatrixData[4]);
        modelMatrix02->setNum(modelMatrixData[8]);
        modelMatrix03->setNum(modelMatrixData[12]);

        modelMatrix10->setNum(modelMatrixData[1]);
        modelMatrix11->setNum(modelMatrixData[5]);
        modelMatrix12->setNum(modelMatrixData[9]);
        modelMatrix13->setNum(modelMatrixData[13]);

        modelMatrix20->setNum(modelMatrixData[2]);
        modelMatrix21->setNum(modelMatrixData[6]);
        modelMatrix22->setNum(modelMatrixData[10]);
        modelMatrix23->setNum(modelMatrixData[14]);

        modelMatrix30->setNum(modelMatrixData[3]);
        modelMatrix31->setNum(modelMatrixData[7]);
        modelMatrix32->setNum(modelMatrixData[11]);
        modelMatrix33->setNum(modelMatrixData[15]);

        viewMatrix00->setNum(viewMatrixData[0]);
        viewMatrix01->setNum(viewMatrixData[4]);
        viewMatrix02->setNum(viewMatrixData[8]);
        viewMatrix03->setNum(viewMatrixData[12]);

        viewMatrix10->setNum(viewMatrixData[1]);
        viewMatrix11->setNum(viewMatrixData[5]);
        viewMatrix12->setNum(viewMatrixData[9]);
        viewMatrix13->setNum(viewMatrixData[13]);

        viewMatrix20->setNum(viewMatrixData[2]);
        viewMatrix21->setNum(viewMatrixData[6]);
        viewMatrix22->setNum(viewMatrixData[10]);
        viewMatrix23->setNum(viewMatrixData[14]);

        viewMatrix30->setNum(viewMatrixData[3]);
        viewMatrix31->setNum(viewMatrixData[7]);
        viewMatrix32->setNum(viewMatrixData[11]);
        viewMatrix33->setNum(viewMatrixData[15]);

        projectionMatrix00->setNum(projectionMatrixData[0]);
        projectionMatrix01->setNum(projectionMatrixData[4]);
        projectionMatrix02->setNum(projectionMatrixData[8]);
        projectionMatrix03->setNum(projectionMatrixData[12]);

        projectionMatrix10->setNum(projectionMatrixData[1]);
        projectionMatrix11->setNum(projectionMatrixData[5]);
        projectionMatrix12->setNum(projectionMatrixData[9]);
        projectionMatrix13->setNum(projectionMatrixData[13]);

        projectionMatrix20->setNum(projectionMatrixData[2]);
        projectionMatrix21->setNum(projectionMatrixData[6]);
        projectionMatrix22->setNum(projectionMatrixData[10]);
        projectionMatrix23->setNum(projectionMatrixData[14]);

        projectionMatrix30->setNum(projectionMatrixData[3]);
        projectionMatrix31->setNum(projectionMatrixData[7]);
        projectionMatrix32->setNum(projectionMatrixData[11]);
        projectionMatrix33->setNum(projectionMatrixData[15]);

        count = 0;
    }
}

void MainWindow::updateFieldOfView(double fov)
{
    m_camera->setFieldOfView(fov);
}

void MainWindow::updateNearPlane(double nearPlane)
{
    m_camera->setNearPlane(nearPlane);
}

void MainWindow::updateFarPlane(double farPlane)
{
    m_camera->setFarPlane(farPlane);
}

void MainWindow::updateLeft(double left)
{
    m_camera->setLeft(left);
}

void MainWindow::updateRight(double right)
{
    m_camera->setRight(right);
}

void MainWindow::updateBottom(double bottom)
{
    m_camera->setBottom(bottom);
}

void MainWindow::updateTop(double top)
{
    m_camera->setTop(top);
}
