#ifndef REST_HPP
#define REST_HPP

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QByteArray>
#include <QDebug>


class Rest : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool replyStatus READ getReplyStatus WRITE setReplyStatus NOTIFY replyStatusChanged)
    Q_PROPERTY(QString replyString READ getReplyString WRITE setReplyString NOTIFY replyStringChanged)
    Q_PROPERTY(int counter READ getCounter WRITE setCounter NOTIFY counterChanged)

private:

    //**** ATTRIBUTES *******
    QEventLoop events;
    QNetworkAccessManager* connectionManager;
    QNetworkRequest request;
    QNetworkReply* reply;
    QString replyString;
    QJsonObject error;
    QJsonDocument replyJson;
    QJsonObject jsonObject;
    QJsonArray jsonArray;
    int counter;

    bool replyStatus;
    //***** END ATTRIBUTES *****


public:

    //***** METHODS ********

    explicit Rest(QObject *parent = 0);

    Q_INVOKABLE void init(QString url, QString token);
    Q_INVOKABLE void post (QByteArray data);
    Q_INVOKABLE void get ();
    Q_INVOKABLE void put ();
    Q_INVOKABLE bool validateReply();

    //***** GETERS
    Q_INVOKABLE QString getError();
    Q_INVOKABLE QJsonObject getJsonObject();
    Q_INVOKABLE QJsonArray getJsonArray();

    bool getReplyStatus();

    //***** END GETTERS

    //***** SETERS
    Q_INVOKABLE void setReplyStatus(bool status);
    //***** END SETTERS

    ~Rest();
    //***** END METHODS ***********


    QString getReplyString() const;
    void setReplyString(const QString &value);

    int getCounter() const;
    void setCounter(int value);


signals:

    //void sslErrors(QNetworkReply*,QList<QSslError>);
    void replyStatusChanged();
    void replyStringChanged();
    void counterChanged();

public slots:
   void replyFinished();

};

#endif // REST_H
