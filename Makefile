CXXFLAGS := -std=c++14 -ggdb -O0

SOURCES := $(wildcard *.cpp)
OBJS := $(SOURCES:%.cpp=%.o)

TARGET := client

.PHONY : all
all : $(TARGET)

$(TARGET) : client.cpp
	 $(CXX) $(CXXFLAGS) $^ -o $@

.PHONY : clean
clean :
	rm -rf $(TARGET) $(OBJS)
