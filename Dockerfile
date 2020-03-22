FROM ainr/who-drank-my-beer
RUN cd /home/WhoDrankMyBeer && \
	git pull origin master && \
	cd WhoDrankMyBeerBot/build && \
	rm -rf * && \
	cmake .. && \
	make && \
	./WhoDrankMyBeer
