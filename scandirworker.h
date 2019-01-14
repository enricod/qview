#ifndef SCANDIRWORKER_H
#define SCANDIRWORKER_H

#include <QObject>
#include <QStringList>
#include <QImage>
#include <QDir>

class ScanDirWorker : public QObject
{
    Q_OBJECT

public:
    explicit ScanDirWorker(QObject *parent = nullptr);

    void setCurDir(QDir curDir);

public slots:
    void process();

    void extractImage(QString imageFile);

signals:
    void finished(QStringList images);

    void imageExtracted();

    void error(QString err);

private:
    QStringList images;

    QDir curDir;
};

#endif // SCANDIRWORKER_H
