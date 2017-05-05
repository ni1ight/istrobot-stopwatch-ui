#ifndef __REST_CLIENT_
#define __REST_CLIENT_

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QString>
#include <QObject>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>

#define STATUS_START 0
#define STATUS_OK 1
#define STATUS_FAIL 2
#define API "99243319c28a0e0e9ef6"

class RestClient {
private:
	QNetworkAccessManager *manager;
	QNetworkRequest request;
	QString url;
	const QString configFile = QString("url-config.ini");
public:
	RestClient();
	int sendData(int status, QString time);
};

#endif
