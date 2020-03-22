FROM ainr/housekeeperbot
RUN apt update -y && \
	apt upgrade -y && \
	cd /home && \
	git clone https://github.com/a-khakimov/WhoDrankMyBeer.git && \
	cd WhoDrankMyBeer/ && \
	git submodule update --init --recursive && \
	cd WhoDrankMyBeerBot && \
	mkdir build && \
	cd build && \
	cmake .. && \
	make && \
	./WhoDrankMyBeer
