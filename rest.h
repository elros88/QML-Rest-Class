#ifndef REST_HPP
#define REST_HPP

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QUrl>
#include <QHttpMultiPart>
#include <QFile>
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QByteArray>
#include <QDebug>


class Rest : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool estatusRespuesta READ getEstatusRespuesta WRITE setEstatusRespuesta NOTIFY cambioEstatusRespuesta)
    Q_PROPERTY(QString respuesta READ getRespuesta WRITE setRespuesta NOTIFY cambioRespuesta)
    Q_PROPERTY(int contador READ getContador WRITE setContador NOTIFY cambioContador)

private:

    //**** ATTRIBUTES *******
    QEventLoop eventos;
    QNetworkAccessManager* manejadorConexion;
    QNetworkRequest peticion;
    QNetworkReply* respuestaBruta;
    QString respuesta;
    QString llave;
    QUrl url;
    QUrlQuery valorPeticion;
    QJsonObject error;
    QJsonDocument documentoJson;
    QJsonObject objectoJson;
    QJsonArray arregloJson;
    QHttpMultiPart* multiparte;
    int contador;

    bool estatusRespuesta;
    //***** END ATTRIBUTES *****


public:

    //***** METHODS ********

    explicit Rest(QObject *parent = 0);

    Q_INVOKABLE void iniciar(QString url);
    Q_INVOKABLE void iniciarCabecera(QString campo, QString valor);
    Q_INVOKABLE void post (QByteArray data);
    Q_INVOKABLE void get ();
    Q_INVOKABLE void put ();
    Q_INVOKABLE bool validarRespuesta();
    Q_INVOKABLE void agregarAUrl(QString valor);
    //***** GETERS
    Q_INVOKABLE QString getMensajeError();
    Q_INVOKABLE QJsonObject getObjetoJson();
    Q_INVOKABLE QJsonArray getCadenaJson();

    bool getEstatusRespuesta();

    //***** END GETTERS

    //***** SETERS
    Q_INVOKABLE void setEstatusRespuesta(bool estatus);
    //***** END SETTERS

    ~Rest();
    //***** END METHODS ***********


    QString getRespuesta() const;
    void setRespuesta(const QString &value);

    int getContador() const;
    void setContador(int value);


    Q_INVOKABLE QUrl getUrl() const;
    Q_INVOKABLE void setUrl(const QString &value);

    Q_INVOKABLE QJsonObject getError() const;
    void setError(const QJsonObject &value);

    Q_INVOKABLE void post();
    Q_INVOKABLE void postArchivo();

    Q_INVOKABLE void iniciarCabeceraString(QString campo, QString valor);
    Q_INVOKABLE bool crearCabeceraMultiparte(QString url);


signals:

    //void sslErrors(QNetworkReply*,QList<QSslError>);
    void cambioEstatusRespuesta();
    void cambioRespuesta();
    void cambioContador();

public slots:
   void respuestaRecibida();

};

#endif // REST_H
