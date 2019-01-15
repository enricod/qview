#ifndef SCANDIRWORKER_H
#define SCANDIRWORKER_H

#include <QObject>
#include <QStringList>
#include <QImage>
#include <QDir>
#include <QStringListModel>

class ScanDirWorker : public QObject
{
    Q_OBJECT

public:
    explicit ScanDirWorker(QObject *parent = nullptr);

    void setCurDir(QDir curDir);

public slots:
    void process();


signals:
    void finished(QStringList images);

    void error(QString err);

private:
    QStringList images;

    QDir curDir;



};

#endif // SCANDIRWORKER_H
