#include "rest.h"

Rest::Rest(QObject *parent) : QObject(parent)
{
    manejadorConexion = new  QNetworkAccessManager(this);

    estatusRespuesta = false;
    llave = "1234567890";
}

Rest::~Rest()
{

}

//***** INITIALIZATE REQUEST PARAMETERS AND URL
void Rest::iniciar(QString url)
{
     estatusRespuesta = false;

     peticion.setUrl(QUrl(url));
     peticion.setHeader(peticion.ContentTypeHeader,"application/json");
     peticion.setRawHeader(QByteArray("X-BAASBOX-APPCODE"), QByteArray(llave.toStdString().c_str()));
}

void Rest::iniciarCabecera(QString campo, QString valor)
{
    peticion.setRawHeader(QByteArray(campo.toStdString().c_str()), QByteArray(valor.toStdString().c_str()));
}

void Rest::iniciarCabeceraString(QString campo, QString valor)
{
    peticion.setRawHeader(QByteArray(campo.toStdString().c_str()), valor.toStdString().c_str());
}



//***** POST-REQUEST OPERATIONS *****
//       (in order of calling)
void Rest::respuestaRecibida()
{ 
    if(sender())
    {
        respuestaBruta = qobject_cast<QNetworkReply*>(sender());
        setEstatusRespuesta(true);
    }
}

bool Rest::validarRespuesta()
{
    qDebug()<<"Error del Endpoint: "<<respuestaBruta->error();

    QList<QByteArray> headerList = respuestaBruta->rawHeaderList();
    foreach(QByteArray head, headerList) {
        qDebug() << head << ":" << respuestaBruta->rawHeader(head);
    }
    if(respuestaBruta->error() == QNetworkReply::NoError)
    {
        respuesta = (QString)respuestaBruta->readAll();
        qDebug()<<respuesta;
        documentoJson = QJsonDocument::fromJson(respuesta.toUtf8());
//        qDebug()<<"Respuesta del Endpoint: "<<replyString.toStdString().c_str();
//        qDebug()<<"JSON: "<<replyJson.toJson();
        if(!documentoJson.isArray())
        {
            qDebug() << "OBJETO";
            objectoJson = documentoJson.object();
            respuestaBruta->deleteLater();
            return true;
        }
        if(documentoJson.isArray())
        {
            qDebug() << "ARREGLO";
            arregloJson = documentoJson.array();
            respuestaBruta->deleteLater();
            return true;
        }
    }
    else
    {
        respuesta = respuestaBruta->readAll();
        documentoJson = QJsonDocument::fromJson(respuesta.toUtf8());
        error = documentoJson.object();
        QByteArray errorBody = QJsonDocument::fromJson(respuesta.toUtf8()).toJson();
//        qDebug()<<"Error en Respuesta: "<<replyString.toStdString().c_str();
//        qDebug()<<"Error json: "<<replyJson.toJson();
//        qDebug()<<errorBody.toStdString().c_str();

        respuestaBruta->deleteLater();

    }
    return false;
}

void Rest::agregarAUrl(QString valor)
{
    //valorPeticion.addQueryItem(valor);

}

void Rest::setEstatusRespuesta(bool status)
{
    if(estatusRespuesta != status)
    {
        estatusRespuesta = status;
        emit cambioEstatusRespuesta();
        estatusRespuesta = !status;
    }

}

bool Rest::getEstatusRespuesta()
{
    return estatusRespuesta;
}

QString Rest::getMensajeError()
{
    return respuesta;
}

QJsonArray Rest::getCadenaJson() {

    return arregloJson;
}

QJsonObject Rest::getObjetoJson()
{
    return objectoJson;
}

QString Rest::getRespuesta() const
{
    return respuesta;
}

void Rest::setRespuesta(const QString &value)
{
    respuesta = value;
}

int Rest::getContador() const
{
    return contador;
}

void Rest::setContador(int value)
{
    contador = value;
    emit cambioContador();
}

QUrl Rest::getUrl() const
{
    return url;
}

void Rest::setUrl(const QString &value)
{
    url.setUrl(value);
}

QJsonObject Rest::getError() const
{
    return error;
}

void Rest::setError(const QJsonObject &value)
{
    error = value;
}

//***** END POST-REQUEST OPERATION *****

//***** REQUEST METHODS: POST, GET, PUT *****
void Rest::post(QByteArray data)
{
    respuestaBruta = manejadorConexion->post(peticion, data);
    qDebug()<<"Enviando Post";

    connect(respuestaBruta, SIGNAL(finished()), this, SLOT(respuestaRecibida()));
}

void Rest::post()
{
    QByteArray data;

    QJsonObject json;
    json[""] = "";
    QJsonDocument documento(json);
    data.append(documento.toJson(QJsonDocument::Compact));
    qDebug()<<"Enviando Post";
    respuestaBruta = manejadorConexion->post(peticion, data);
//    qDebug()<<data.toStdString();
    connect(respuestaBruta, SIGNAL(finished()), this, SLOT(respuestaRecibida()));
}

void Rest::postArchivo()
{
    respuestaBruta = manejadorConexion->post(peticion, multiparte);
    multiparte->setParent(respuestaBruta);
    qDebug()<<"Enviando Post";

    connect(respuestaBruta, SIGNAL(finished()), this, SLOT(respuestaRecibida()));
    //multiparte->deleteLater();
}

void Rest::get()
{
    respuestaBruta = manejadorConexion->get(peticion);
    connect(respuestaBruta, SIGNAL(finished()), this, SLOT(respuestaRecibida()));
}

void Rest::put()
{
    QByteArray data;
    data.append("{}");
    respuestaBruta = manejadorConexion->put(peticion, data);
    connect(respuestaBruta, SIGNAL(finished()), this, SLOT(respuestaRecibida()));
}

//***** END REQUEST METHODS *****


bool Rest::crearCabeceraMultiparte(QString url)
{
    multiparte = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart parteArchivo;

    //parteArchivo.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    parteArchivo.setHeader(QNetworkRequest::ContentDispositionHeader,
                      QVariant("form-data; name=\"file\""));

    QFile* imagen = new QFile(url);

    if(imagen->open(QIODevice::ReadOnly))
    {
        parteArchivo.setBodyDevice(imagen);
        imagen->setParent(multiparte);
        multiparte->append(parteArchivo);
        return true;
    }
    else
    {
        qDebug()<< imagen->errorString();
    }

    return false;
}
