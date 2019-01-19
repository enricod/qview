#ifndef EXTRACTIMAGEWORKER_H
#define EXTRACTIMAGEWORKER_H

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

#endif // EXTRACTIMAGEWORKER_H
