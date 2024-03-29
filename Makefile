CC=g++
CFLAGS=-c -Wall -Werror -Wpedantic
LDFLAGS=-static
SOURCES=main.cpp card.cpp deck.cpp hand.cpp player.cpp dealer.cpp ui.cpp pcg.cpp blackjack.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=blackjack
TEST_SOURCES=tests.cpp card.cpp deck.cpp hand.cpp player.cpp dealer.cpp ui.cpp pcg.cpp
TEST_OBJECTS=$(TEST_SOURCES:.cpp=.o)
TEST_EXECUTABLE=tests

ifeq ($(OS),Windows_NT)
    RM = del /Q /F
    EXECUTABLE := $(EXECUTABLE).exe
    TEST_EXECUTABLE := $(TEST_EXECUTABLE).exe
else
    RM = rm -f
endif

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) *.o $(EXECUTABLE) $(TEST_EXECUTABLE)

# Add new rules for tests
test: $(TEST_SOURCES) $(TEST_EXECUTABLE)

$(TEST_EXECUTABLE): $(TEST_OBJECTS)
	$(CC) $(LDFLAGS) $(TEST_OBJECTS) -o $@

run_tests: test
	./$(TEST_EXECUTABLE)
