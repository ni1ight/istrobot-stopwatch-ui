#include "rest_client.h"

RestClient::RestClient() : manager(new QNetworkAccessManager)
{
	QFile file(configFile);
	file.open(QIODevice::ReadOnly);
	QTextStream in(&file);	
	url = in.readLine();
}

int RestClient::sendData(int status, long time){
	QUrl qurl(url);
	QNetworkRequest request(qurl);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	QString json = QString("{\"status\":%1, \"time\": %2}").arg(status).arg(time);
	manager->post(request, json.toUtf8());
	// qDebug() << "received status " << status << " time " << time;
	return 0;
}
