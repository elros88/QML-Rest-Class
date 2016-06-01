REST

Clase para comunicarse con servicios Restful para aplicaciones Qt y QML.
Permite hacer request HTTP simples, enviando objetos o arreglos JSON:

 - get
 - put
 - post

Esta desarrollada en Qt5. Para utilizarla copie el directorio de esta clase en la raiz de su proyecto y en el archivo .pro del mismo agregue la siguiente linea:

include("rest/rest.pri")

Luego salvbe el archivo .pro y utilice la clase incluyendo la libreria de la siguiente forma:

#include "rest.h"

