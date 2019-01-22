#ifndef EXTRACTIMAGEWORKER_H
#define EXTRACTIMAGEWORKER_H

#include "libraw/libraw.h"
#include <QObject>
#include <QStringList>
#include <QImage>
#include <QDir>
#include <QStringListModel>

class ExtractImageWorker : public QObject
{
    Q_OBJECT

public:
    explicit ExtractImageWorker(QObject *parent = nullptr);

    void setImage(QString image);

public slots:
    void process();


signals:
    void finished(QImage *qimage);

    void error(QString err);

private:

    QImage* createThumb(libraw_processed_image_t *img);

    QString image;

};

#endif // EXTRACTIMAGEWORKER_H
