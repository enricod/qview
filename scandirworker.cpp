#include "scandirworker.h"
#include <QImage>

ScanDirWorker::ScanDirWorker(QObject *parent) : QObject(parent)
{

}

void ScanDirWorker::setCurDir(QDir dir)
{
    curDir = dir;
}

void ScanDirWorker::process()
{
    // allocate resources using new here
    qDebug("processing %s", qUtf8Printable(curDir.absolutePath()));
    emit finished(images);
}

