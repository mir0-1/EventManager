all: remote

OUTPUT_NAME=wifilib
COMPILER=g++
CONFIG=`pkg-config --cflags --libs glib-2.0 libnm`
BUILD=$(COMPILER) $^ $(CONFIG) -o $(OUTPUT_NAME)

local: EventManager.cpp main.cpp
	make clean
	$(BUILD)
	
remote: EventManager.cpp main.cpp
	make clean
	git pull
	$(BUILD)

clean:
	rm -f $(OUTPUT_NAME)