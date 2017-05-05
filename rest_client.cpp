#include "rest_client.h"

RestClient::RestClient() : manager(new QNetworkAccessManager)
{
	QFile file(configFile);
	url = QString();
	if (file.exists()){
		file.open(QIODevice::ReadOnly);
		QTextStream in(&file);	
		url = in.readLine();
	}
}

int RestClient::sendData(int status, QString time){
	if (url.isNull()){
		qDebug() << "[WARN] missing url configuration, url-config.ini file not exists or cannnot read file";
		return -1;
	}
	QUrl qurl(url);
	QNetworkRequest request(qurl);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	QString data;
	if (status == STATUS_START){
		data = QString("api=%1&command=start").arg(API);
	}else{
		QString val;
		if (status == STATUS_OK){
			val = time;
		}
		else{
			val = "00:00.000";
		}
		data = QString("api=%1&command=end&value=%2").arg(API).arg(val);
	}
	manager->post(request, data.toUtf8());
	qDebug() << data;
	return 0;
}
