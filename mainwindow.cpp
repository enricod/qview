#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>

#include <QImage>
#include <QPixmap>
#include "scandirworker.h"
#include "extractimageworker.h"
#include <QStringListModel>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //curDir = QDir("/home/enricodonelli/Pictures/2018/10/04");
    curDir = QDir("/data2/Pictures/2019/01");
    ui->setupUi(this);
    createActions();

    imagesListModel = new QStringListModel(this);

    connect(ui->imagesListView, SIGNAL(activated(QModelIndex)),
            this, SLOT(onImageItemClicked(QModelIndex)));
}

MainWindow::~MainWindow()
{
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

void MainWindow::extractThumbs()
{
    qInfo("%s", qUtf8Printable(curDir.absolutePath()));

    // Avvia thread per lettura delle immagini nella directory
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

}

void MainWindow::onImageItemClicked(QModelIndex item)
{
    selectImage(item.row());
}

/**
 * @brief MainWindow::imagesList
 * @param images immagini lette nella directory selezionata
 */
void MainWindow::imagesList(QStringList images)
{
    qInfo("Immagini caricate %d", images.length());

    // Populate our model
    imagesListModel->setStringList(images);

    // Glue model and view together
    ui->imagesListView->setModel(imagesListModel);
}

/**
 * @brief MainWindow::selectImage
 * @param imageIndex l'immagine da selezionare e da caricare
 */
void MainWindow::selectImage(int imageIndex)
{
    qInfo("Immagini selezionata %d", imageIndex);

    QThread* thread = new QThread;
    ExtractImageWorker* worker = new ExtractImageWorker();
    QString img = imagesListModel->stringList().value(imageIndex);
    worker->setImage(img);
    worker->moveToThread(thread);
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished(QImage*)), this, SLOT(imageLoaded(QImage*)));
    connect(worker, SIGNAL(finished(QImage*)), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished(QImage*)), worker, SLOT(deleteLater()));
    thread->start();
}


void MainWindow::imageLoaded(QImage* img)
{
    qInfo("immagine caricata");
    ui->mainImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->mainImageLabel->setScaledContents(true);
    ui->mainImageLabel->setPixmap( QPixmap::fromImage(*img));
    //scaleFactor = 1.0;

    ui->scrollArea->setVisible(true);
}


//int MainWindow::processImage( QString imgName){
//    int ret;

//    qInfo("processing %s", qUtf8Printable( imgName ));

//    LibRaw iProcessor;
//    iProcessor.open_file(qUtf8Printable(imgName));

//    // The metadata are accessible through data fields of the class
//    qInfo("Image size: %d x %d",iProcessor.imgdata.sizes.width,iProcessor.imgdata.sizes.height);

//    // Let us unpack the image
//    iProcessor.unpack();

//    // Convert from imgdata.rawdata to imgdata.image:
//    // iProcessor.raw2image();
//    if ( (ret = iProcessor.unpack_thumb()) != LIBRAW_SUCCESS )
//    {

//    } else
//    {
//        libraw_processed_image_t *thumb = iProcessor.dcraw_make_mem_thumb(&ret);
//        if (thumb) {
//            qInfo("salvo miniatura %s", qUtf8Printable( imgName));
//            QImage *imgThumb =  createThumb(thumb) ; //, imgName);

//            //QPixmap::fromImage(imgThumb)
//            ui->mainImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
//            ui->mainImageLabel->setScaledContents(true);
//            ui->mainImageLabel->setPixmap( QPixmap::fromImage(*imgThumb));
//            //scaleFactor = 1.0;

//            ui->scrollArea->setVisible(true);

//            LibRaw::dcraw_clear_mem(thumb);
//        } else {
//            fprintf(stderr, "Cannot unpack thumbnail of %s to memory buffer: %s\n", qUtf8Printable(imgName), libraw_strerror(ret));
//        }

//    }



//    // And let us print its dump; the data are accessible through data fields of the class
//    //        for(int i = 0;i < iProcessor.imgdata.sizes.iwidth *  iProcessor.imgdata.sizes.iheight; i++)
//    //           printf("i=%d R=%d G=%d B=%d G2=%d\n",
//    //                        i,
//    //                        iProcessor.imgdata.image[i][0],
//    //                        iProcessor.imgdata.image[i][1],
//    //                        iProcessor.imgdata.image[i][2],
//    //                        iProcessor.imgdata.image[i][3]
//    //                );

//    // Finally, let us free the image processor for work with the next image
//    iProcessor.recycle();
//    return 0;
//}

void MainWindow::on_startProcessingBtn_clicked()
{
    extractThumbs();
}
