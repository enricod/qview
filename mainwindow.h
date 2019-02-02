#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QThread>

#include <QStringListModel>
#include <QListWidgetItem>

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

    void imageLoaded(QImage* img);

    void onImageItemClicked(QModelIndex item);

private:
    Ui::MainWindow *ui;

    void createActions();

    void selectImage(int imageIndex);

    int processImage(QString file);

    QImage *createThumb(libraw_processed_image_t *img); //, const QString imgName);

    // current directory
    QDir curDir;

    // indice dell'immagine corrente
    int imgIndex;

    QImage *currentImg;

    QStringListModel *imagesListModel;


};

#endif // MAINWINDOW_H
