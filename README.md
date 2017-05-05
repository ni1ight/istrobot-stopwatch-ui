## BUILD ##

	git clone git@github.com:ni1ight/istrobot-stopwatch-ui.git
	mkdir build
	cd build
	qmake ..
	make
	sudo ./install_unity.sh #For unity desktop, install and create desktop icon

## RUNNING ##

	./StopWatch


## REST API Integration ##

	create url-config.ini file
	put URL, which accept application/x-www-form-urlencoded method with parameters API, command and value.
