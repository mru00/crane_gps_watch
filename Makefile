SHELL=/bin/sh -x -c

CXXFLAGS += -std=c++11 -Wall -Werror `xml2-config --cflags`
LDFLAGS += `xml2-config --libs` 


crane_gps_watch_client: crane_gps_watch_client.o Watch.o DataTypes.o TcxWriter.o
	$(LINK.cpp) -o $@ $^ $(LDFLAGS)



