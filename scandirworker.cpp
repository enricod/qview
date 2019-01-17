#include "scandirworker.h"
#include <QImage>
#include <qiterator.h>
#include <QDirIterator>
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

    QDirIterator it(curDir.absolutePath(), QStringList() << "*.ARW", QDir::Files, QDirIterator::Subdirectories);
    int i =0;
    while (it.hasNext()) {
        QString imgName = it.next();
        // qInfo("found %s", qUtf8Printable( imgName ));

        images << imgName;
        i++;
    }
    emit finished(images);
}

