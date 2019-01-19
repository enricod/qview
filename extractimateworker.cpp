#include "extractimageworker.h"
#include <QImage>

ExtractImageWorker::ExtractImageWorker(QObject *parent) : QObject(parent)
{

}

void ExtractImageWorker::setCurDir(QDir dir)
{
    curDir = dir;
}

void ExtractImageWorker::process()
{
    // allocate resources using new here
    qDebug("processing %s", qUtf8Printable(curDir.absolutePath()));
    emit finished(images);
}

