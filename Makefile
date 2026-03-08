CXX = clang++
# Added Homebrew include path
CXXFLAGS = -O3 -std=c++17 -I./NTL/include -I/opt/homebrew/include
# Added Homebrew library path
LDFLAGS = -L./NTL/src -L/opt/homebrew/lib -lntl -lgmp

TARGET = phase1
SRC = phase1.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)
