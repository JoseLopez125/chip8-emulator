# 1. Compiler settings - Can change to clang++ if you prefer
CXX = g++

# 2. Compile flags - These control how your code runs
# -Wall -Wextra: Turn on all warnings (helps you catch bugs!)
# -std=c++17: Use Modern C++ features
CXXFLAGS = -Wall -Wextra -std=c++17

# 3. SDL2 flags - This automatically finds the SDL libraries on Fedora
LDFLAGS = $(shell sdl2-config --cflags --libs)

# 4. Source files - List all your .cpp files here
# Example: main.cpp chip8.cpp
SRC = main.cpp

# 5. Output name - What you want your program to be called
TARGET = chip8_emu

# --- The Rules (Don't touch these unless you know what you're doing) ---

# The default rule: "make" runs this
all: $(TARGET)

# The recipe to build the final executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# A clean-up rule: "make clean" deletes the executable
clean:
	rm -f $(TARGET)
