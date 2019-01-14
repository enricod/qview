#include "scandirworker.h"
#include <QImage>

ScanDirWorker::ScanDirWorker(QObject *parent) : QObject(parent)
{

}

void ScanDirWorker::process(QString dir)
{
    // allocate resources using new here
    qDebug("processing %s", dir);
    emit finished(images);
}

void ScanDirWorker::extractImage(QString imageFile) {
    qDebug("processing %s", imageFile);
    emit imageExtracted();
}
