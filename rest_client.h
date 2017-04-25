#ifndef __REST_CLIENT_
#define __REST_CLIENT_

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QString>
#include <QObject>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>

#define STATUS_OK 0
#define STATUS_FAIL 1

class RestClient {
private:
	QNetworkAccessManager *manager;
	QNetworkRequest request;
	QString url;
	const QString configFile = QString("url-config.ini");
public:
	RestClient();
	int sendData(int status, long time);
};

#endif
