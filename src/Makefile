OBJS=line.o wm-vvo.o line_group.o station.o collector.o
BIN=wm-vvo
CXXFLAGS+= -g
LDFLAGS+=-lcurl -lboost_regex

all: $(BIN)

$(BIN): $(OBJS)
	g++ $(LDFLAGS) -o $(BIN) $(OBJS)

clean:
	rm -rf *.o
