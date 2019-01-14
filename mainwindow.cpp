#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "libraw/libraw.h"
#include <QDir>
#include <qiterator.h>
#include <QDirIterator>
#include <QImage>
#include <QPixmap>
#include "scandirworker.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    curDir = QDir("/home/enricodonelli/Pictures/2018/10/04");
    ui->setupUi(this);


    createActions();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
      QToolBar *fileToolBar = addToolBar(tr("File"));
      const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
      QAction *newAct = new QAction(newIcon, tr("&New"), this);
      newAct->setShortcuts(QKeySequence::New);
      newAct->setStatusTip(tr("Create a new file"));
      connect(newAct, &QAction::triggered, this, &MainWindow::extractThumbs);
      fileMenu->addAction(newAct);
      fileToolBar->addAction(newAct);
}

void MainWindow::extractThumbs() {
    qInfo("%s", qUtf8Printable(curDir.absolutePath()));
    //qInfo( curDir.absolutePath().toStdString().c_str());


    QThread* thread = new QThread;
    ScanDirWorker* worker = new ScanDirWorker();
    worker->setCurDir(curDir);
    worker->moveToThread(thread);
//    connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished(QStringList)), this, SLOT(imagesList(QStringList)));
    connect(worker, SIGNAL(finished(QStringList)), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished(QStringList)), worker, SLOT(deleteLater()));
    //connect(thread, SIGNAL(finished(QStringList)), thread, SLOT(deleteLater()));
    thread->start();

    QDirIterator it(curDir.absolutePath(), QStringList() << "*.ARW", QDir::Files, QDirIterator::Subdirectories);
    int i =0;
    while (it.hasNext()) {
        QString imgName = it.next();
        qInfo("found %s", qUtf8Printable( imgName ));
        if (i==0) {
            processImage( imgName);
        }
        i++;
    }

}

void MainWindow::imagesList(QStringList images)
{
    qInfo("Immagini caricate %d", images.length());
}

QImage *MainWindow::createThumb(libraw_processed_image_t *img) { //, const QString imgName) {

    if (!img) {
        return NULL;
    }
    qInfo("salvo miniatura");
    if (img->type == LIBRAW_IMAGE_BITMAP)  {
        char fnt[1024];
        // snprintf(fnt, 1024, "%s.thumb", qUtf8Printable(imgName) );

        return NULL;
        //write_ppm(img, fnt);
    } else if (img->type == LIBRAW_IMAGE_JPEG) {

        QImage *result = new QImage;
        result->loadFromData(img->data, img->data_size, "JPG");

        //QImage result = QImage::loadFromData(img->data, img->data_size, "JPG");
        return result;

//        char fn[1024];
//        snprintf(fn, 1024, "%s.thumb.jpg", qUtf8Printable(imgName));
//        FILE *f = fopen(fn, "wb");
//        if (!f)
//          return;
//        fwrite(img->data, img->data_size, 1, f);
//        qInfo("scrittara %s", fn);
//        fclose(f);
    }
}


int MainWindow::processImage( QString imgName){
    int ret;

    qInfo("processing %s", qUtf8Printable( imgName ));

    // Let us create an image processor
    LibRaw iProcessor;

    // Open the file and read the metadata
    iProcessor.open_file(qUtf8Printable(imgName));

    // The metadata are accessible through data fields of the class
    qInfo("Image size: %d x %d",iProcessor.imgdata.sizes.width,iProcessor.imgdata.sizes.height);

    // Let us unpack the image
    iProcessor.unpack();

    // Convert from imgdata.rawdata to imgdata.image:
    // iProcessor.raw2image();
    if ( (ret = iProcessor.unpack_thumb()) != LIBRAW_SUCCESS ) {

    } else {
        libraw_processed_image_t *thumb = iProcessor.dcraw_make_mem_thumb(&ret);
        if (thumb) {
            qInfo("salvo miniatura %s", qUtf8Printable( imgName));
            QImage *imgThumb =  createThumb(thumb) ; //, imgName);

            //QPixmap::fromImage(imgThumb)
            ui->mainImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
            ui->mainImageLabel->setScaledContents(true);
            ui->mainImageLabel->setPixmap( QPixmap::fromImage(*imgThumb));
            //scaleFactor = 1.0;

            ui->scrollArea->setVisible(true);

            LibRaw::dcraw_clear_mem(thumb);
        } else {
           fprintf(stderr, "Cannot unpack thumbnail of %s to memory buffer: %s\n", qUtf8Printable(imgName), libraw_strerror(ret));
        }

    }



        // And let us print its dump; the data are accessible through data fields of the class
//        for(int i = 0;i < iProcessor.imgdata.sizes.iwidth *  iProcessor.imgdata.sizes.iheight; i++)
//           printf("i=%d R=%d G=%d B=%d G2=%d\n",
//                        i,
//                        iProcessor.imgdata.image[i][0],
//                        iProcessor.imgdata.image[i][1],
//                        iProcessor.imgdata.image[i][2],
//                        iProcessor.imgdata.image[i][3]
//                );

    // Finally, let us free the image processor for work with the next image
    iProcessor.recycle();
    return 0;
}

void MainWindow::on_startProcessingBtn_clicked() {
    extractThumbs();
}
