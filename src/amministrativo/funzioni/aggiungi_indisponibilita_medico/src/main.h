# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -I../../../../lib/con2db -I../../../../lib/con2redis/src -I../../../../utils/src

# Linker flags
LDFLAGS = -L../../../../lib/con2db -L../../../../lib/con2redis/src -lpq -lhiredis

# Target executable
TARGET = main

# Source files
SRCS = main.cpp richiesta_prenotazione.cpp prenotazione_accettata.cpp prenotazione_rifiutata.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(OBJS)
    $(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile source files into object files
%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
    rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean