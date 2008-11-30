OBJS=line.o wm-vvo.o line_group.o station.o collector.o
BIN=wm-vvo
CXXFLAGS+= -g -DBOOST_REGEX_MATCH_EXTRA -DBOOST_REGEX_USE_CPP_LOCALE
LDFLAGS+=-lcurl -lboost_regex

all: $(BIN)

$(BIN): $(OBJS)
	g++ $(LDFLAGS) -o $(BIN) $(OBJS)

clean:
	rm -rf *.o
