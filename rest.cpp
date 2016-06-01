#include "rest.h"

Rest::Rest(QObject *parent) : QObject(parent)
{
    connectionManager = new  QNetworkAccessManager(this);

    replyStatus = false;
}

Rest::~Rest()
{

}

//***** INITIALIZATE REQUEST PARAMETERS AND URL
void Rest::init(QString url, QString token)
{
     replyStatus = false;

     request.setUrl(QUrl(url));
     request.setHeader(request.ContentTypeHeader,"application/json");

     if(!token.isEmpty())
     {
         QString auth = "Bearer " + token;
         request.setRawHeader(QByteArray("Authorization"), QByteArray(auth.toStdString().c_str()));
     }

}


//***** POST-REQUEST OPERATIONS *****
//       (in order of calling)
void Rest::replyFinished()
{ 
    if(sender())
    {
        qDebug() << "Finalizo";
        reply = qobject_cast<QNetworkReply*>(sender());
        setReplyStatus(true);
    }
    else
    {
        qDebug() << "No hay sender";
    }
}

bool Rest::validateReply()
{
    qDebug()<<"Error del Endpoint: "<<reply->error();

    if(reply->error() == QNetworkReply::NoError)
    {
        replyString = (QString)reply->readAll();
        replyJson = QJsonDocument::fromJson(replyString.toUtf8());
        qDebug()<<"Respuesta del Endpoint: "<<replyString.toStdString().c_str();
        qDebug()<<"JSON: "<<replyJson.toJson();
        if(!replyJson.isArray())
        {
            jsonObject = replyJson.object();
            reply->deleteLater();
            return true;
        }
        if(replyJson.isArray())
        {
            jsonArray = replyJson.array();
            reply->deleteLater();
            return true;
        }
    }
    else
    {
        replyString = reply->readAll();
        replyJson = QJsonDocument::fromJson(replyString.toUtf8());
        error = replyJson.object();
        QByteArray errorBody = QJsonDocument::fromJson(replyString.toUtf8()).toJson();
//        qDebug()<<"Error en Respuesta: "<<replyString.toStdString().c_str();
//        qDebug()<<"Error json: "<<replyJson.toJson();
//        qDebug()<<errorBody.toStdString().c_str();

        reply->deleteLater();

    }
    return false;
}

void Rest::setReplyStatus(bool status)
{
    if(replyStatus != status)
    {
        replyStatus = status;
        emit replyStatusChanged();
        replyStatus = !status;
    }

}

bool Rest::getReplyStatus()
{
    return replyStatus;
}

QString Rest::getError()
{
    return replyString;
}

QJsonArray Rest::getJsonArray() {

    return jsonArray;
}

QJsonObject Rest::getJsonObject()
{
    return jsonObject;
}

QString Rest::getReplyString() const
{
    return replyString;
}

void Rest::setReplyString(const QString &value)
{
    replyString = value;
}

int Rest::getCounter() const
{
    return counter;
}

void Rest::setCounter(int value)
{
    counter = value;
    emit counterChanged();
}

//***** END POST-REQUEST OPERATION *****

//***** REQUEST METHODS: POST, GET, PUT *****
void Rest::post(QByteArray data)
{
    reply = connectionManager->post(request, data);
    connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
}

void Rest::get()
{
    reply = connectionManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
}

void Rest::put()
{
    QByteArray data;
    data.append("{}");
    reply = connectionManager->put(request, data);
    connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
}

//***** END REQUEST METHODS *****
