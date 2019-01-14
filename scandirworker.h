#ifndef SCANDIRWORKER_H
#define SCANDIRWORKER_H

#include <QObject>
#include <QStringList>
#include <QImage>

class ScanDirWorker : public QObject
{
    Q_OBJECT

public:
    explicit ScanDirWorker(QObject *parent = nullptr);

public slots:
    void process(QString dir);

    void extractImage(QString imageFile);

signals:
    void finished(QStringList images);

    void imageExtracted();

    void error(QString err);

private:
    QStringList images;
};

#endif // SCANDIRWORKER_H
