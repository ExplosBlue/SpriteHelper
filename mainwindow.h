#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "editorwidget.h"

#include <QMainWindow>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void resizeEvent(QResizeEvent* event);


private slots:
    void openFile();
    void saveFile();
    void saveAsFile();
    void toggleExistingVisibility(bool);
    void toggleSpriteTree(bool);

private:
    Ui::MainWindow *ui;
    EditorWidget* mEditorWidget;

    QSettings* mSettings;

    QAction* mOpenFile;
    QAction* mSave;
    QAction* mSaveAs;
    QAction* mShowExistingData;
    QAction* mShowSpriteTree;
};

#endif // MAINWINDOW_H
