OBJS=line.o wm-vvo.o line_group.o station.o collector.o config_parser.o dockapp.o
BIN=wm-vvo
CXXFLAGS+= -DBOOST_REGEX_MATCH_EXTRA
CXXFLAGS+= -O3
#CXXFLAGS+= -g
CXXFLAGS+=`pkg-config --cflags gai`
LDFLAGS+=-L ../wmapp-0.0.4.3 -lcurl -lboost_regex 
LDFLAGS+=-L/usr/lib/lib -lgai -lgdk_pixbuf-2.0 -lm
LDFLAGS+=-O3

all: $(BIN)

$(BIN): $(OBJS)
	g++ $(LDFLAGS) -o $(BIN) $(OBJS)

clean:
	rm -rf *.o

install:
	install -m755 $(BIN) $(DESTDIR)$(BINDIR)



collector.o: collector.cpp \
	collector.h \
	line_group.h \
	station.h \
	line.h \
	config_parser.h
config_parser.o: config_parser.cpp \
	config_parser.h \
	line.h \
	station.h \
	line_group.h
dockapp.o: dockapp.cpp \
	dockapp.h \
	line_group.h \
	station.h \
	line.h \
	config_parser.h \
	collector.h
line.o: line.cpp \
	line.h
line_group.o: line_group.cpp \
	line_group.h \
	station.h \
	line.h
station.o: station.cpp \
	station.h \
	line.h
wm-vvo.o: wm-vvo.cpp \
	line_group.h \
	station.h \
	line.h \
	collector.h \
	config_parser.h \
	dockapp.h
collector.o: collector.h
config_parser.o: config_parser.h
dockapp.o: dockapp.h
line_group.o: line_group.h \
	station.h \
	line.h
line.o: line.h
station.o: station.h \
	line.h



.PHONY: all clean install
