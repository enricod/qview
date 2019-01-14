#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QThread>

#include "libraw/libraw.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void extractThumbs();

    void on_startProcessingBtn_clicked();

    void imagesList(QStringList images);

private:
    Ui::MainWindow *ui;

    void createActions();

    int processImage(  QString file);

    QImage *createThumb(libraw_processed_image_t *img); //, const QString imgName);

    //void extractThumbs();

    // current directory
    QDir curDir;
};

#endif // MAINWINDOW_H
