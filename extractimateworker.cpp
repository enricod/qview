#include "extractimageworker.h"


#include <QImage>

ExtractImageWorker::ExtractImageWorker(QObject *parent) : QObject(parent)
{

}

void ExtractImageWorker::setImage(QString img)
{
    image = img;
}

void ExtractImageWorker::process()
{
    int ret;

    qInfo("processing %s", qUtf8Printable( image ));

    LibRaw iProcessor;

    iProcessor.open_file(qUtf8Printable(image));

    // The metadata are accessible through data fields of the class
    qInfo("Image size: %d x %d",iProcessor.imgdata.sizes.width,iProcessor.imgdata.sizes.height);

    iProcessor.unpack();

    // Convert from imgdata.rawdata to imgdata.image:
    // iProcessor.raw2image();
    if ( (ret = iProcessor.unpack_thumb()) != LIBRAW_SUCCESS )
    {

    } else
    {
        //libraw_processed_image_t *thumb = iProcessor.dcraw_make_mem_thumb(&ret);
        // libraw_processed_image_t *thumb = iProcessor.dcraw_make_mem_image(&ret);
        libraw_processed_image_t *thumb = iProcessor.dcraw_make_mem_thumb(&ret);
        if (thumb) {
            QImage *imgThumb =  createThumb(thumb) ; //, imgName);


//            //QPixmap::fromImage(imgThumb)
//            ui->mainImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
//            ui->mainImageLabel->setScaledContents(true);
//            ui->mainImageLabel->setPixmap( QPixmap::fromImage(*imgThumb));
//            //scaleFactor = 1.0;

//            ui->scrollArea->setVisible(true);

            LibRaw::dcraw_clear_mem(thumb);
            iProcessor.recycle();

            emit( finished(imgThumb)) ;
        } else {
            fprintf(stderr, "Cannot unpack thumbnail of %s to memory buffer: %s\n", qUtf8Printable(image), libraw_strerror(ret));
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

}

QImage *ExtractImageWorker::createThumb(libraw_processed_image_t *img)
{

    if (!img) {
        return NULL;
    }
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
