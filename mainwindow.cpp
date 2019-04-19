#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
     mSettings = new QSettings("data/Settings.ini", QSettings::IniFormat);

     // Default Settings
    if (mSettings->value("LastOpenPath").isNull())
        mSettings->setValue("LastOpenPath", QCoreApplication::applicationDirPath());

    if (mSettings->value("LastSavePath").isNull())
        mSettings->setValue("LastSavePath", QCoreApplication::applicationDirPath());

    if (mSettings->value("SpriteTreeVisibility").isNull())
        mSettings->setValue("SpriteTreeVisibility", true);

    if (mSettings->value("ExistingDataVisibility").isNull())
        mSettings->setValue("ExistingDataVisibility", false);

    // Setup Ui
    ui->setupUi(this);
    ui->mainToolBar->setMovable(false);

    // Setup Widgets
    mEditorWidget = new EditorWidget();
    this->setCentralWidget(mEditorWidget);

    QString basePath = QCoreApplication::applicationDirPath();

    // Setup Actions + Menus
    mOpenFile = new QAction();
    mOpenFile->setText("Open File");
    mOpenFile->setIcon(QIcon(basePath + "/data/icons/open.png"));
    ui->mainToolBar->addAction(mOpenFile);
    connect(mOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));

    ui->mainToolBar->addSeparator();

    mSave = new QAction();
    mSave->setText("Save");
    mSave->setIcon(QIcon(basePath + "/data/icons/save.png"));
    mSave->setDisabled(true);
    ui->mainToolBar->addAction(mSave);
    connect(mSave, SIGNAL(triggered()), this, SLOT(saveFile()));

    mSaveAs = new QAction();
    mSaveAs->setText("Save As");
    mSaveAs->setIcon(QIcon(basePath + "/data/icons/saveAs.png"));
    mSaveAs->setDisabled(true);
    ui->mainToolBar->addAction(mSaveAs);
    connect(mSaveAs, SIGNAL(triggered()), this, SLOT(saveAsFile()));

    ui->mainToolBar->addSeparator();

    mShowExistingData = new QAction();
    mShowExistingData->setText("Show Existing Data");
    mShowExistingData->setDisabled(true);
    mShowExistingData->setCheckable(true);
    mShowExistingData->setChecked(mSettings->value("ExistingDataVisibility").value<bool>());
    ui->mainToolBar->addAction(mShowExistingData);
    mEditorWidget->toggleExistingData(mShowExistingData->isChecked());
    connect(mShowExistingData, SIGNAL(toggled(bool)), this, SLOT(toggleExistingVisibility(bool)));

    mShowSpriteTree = new QAction();
    mShowSpriteTree->setText("Show Sprite Tree");
    mShowSpriteTree->setDisabled(true);
    mShowSpriteTree->setCheckable(true);
    mShowSpriteTree->setChecked(mSettings->value("SpriteTreeVisibility").value<bool>());
    ui->mainToolBar->addAction(mShowSpriteTree);
    mEditorWidget->toggleSpriteTree(mShowSpriteTree->isChecked());
    connect(mShowSpriteTree, SIGNAL(toggled(bool)), this, SLOT(toggleSpriteTree(bool)));

    this->setWindowTitle("SpriteHelper");
    this->setWindowIcon(QIcon(basePath + "/data/icons/goomba.png"));

    this->resize(1150, 760);
}

MainWindow::~MainWindow()
{
    delete mEditorWidget;

    delete mOpenFile;
    delete mSave;
    delete mSaveAs;
    delete mShowSpriteTree;
    delete mShowExistingData;

    delete mSettings;

    delete ui;
}

void MainWindow::openFile()
{
    QString previousPath = mSettings->value("LastOpenPath").value<QString>();

    QString openPath = QFileDialog::getOpenFileName(this, "SpriteHelper - Open", previousPath, "NSMBU Spritedata (*.xml)");

    if (openPath.isEmpty() || openPath.isNull())
        return;

    mSettings->setValue("LastOpenPath", openPath);

    QFile file(openPath);
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::information(this, "SpriteHelper", "Could not open " + openPath + "!", QMessageBox::Ok);
        return;
    }

    QDomDocument domDoc;
    domDoc.setContent(&file);
    mEditorWidget->loadXML(domDoc);
    file.close();

    mSave->setEnabled(true);
    mSaveAs->setEnabled(true);
    mShowExistingData->setEnabled(true);
    mShowSpriteTree->setEnabled(true);

    this->setWindowTitle(QString("SpriteHelper - Editing: %1").arg(openPath));
}

void MainWindow::saveFile()
{
    QString savePath = mSettings->value("LastOpenPath").value<QString>();

    mEditorWidget->saveXML(savePath);
    ui->statusBar->showMessage(QString("Saved to: %1").arg(savePath));
}

void MainWindow::saveAsFile()
{
    QString previousPath = mSettings->value("LastSavePath").value<QString>();

    QString savePath = QFileDialog::getSaveFileName(this, "SpriteHelper - Save", previousPath, "NSMBU Spritedata (*.xml)");

    if (savePath.isEmpty() || savePath.isNull())
        return;

    mSettings->setValue("LastSavePath", savePath);
    mEditorWidget->saveXML(savePath);
    ui->statusBar->showMessage(QString("Saved to: %1").arg(savePath));
}

void MainWindow::toggleExistingVisibility(bool toggle)
{
    mEditorWidget->toggleExistingData(toggle);
    mSettings->setValue("ExistingDataVisibility", toggle);
}

void MainWindow::toggleSpriteTree(bool toggle)
{
    mEditorWidget->toggleSpriteTree(toggle);
    mSettings->setValue("SpriteTreeVisibility", toggle);
}
