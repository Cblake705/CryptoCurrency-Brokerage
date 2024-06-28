# Compiler settings
CXX=g++
CXXFLAGS=-std=c++11 -Wall -g3 -c

# Object files
OBJS = main.o producer.o consumer.o report.o

# Program name
PROGRAM = tradecrypto

# Rules
# The first target is executed if you just type 'make'
$(PROGRAM) : $(OBJS)
	$(CXX) -o $(PROGRAM) $^ -lpthread

# Individual source files
main.o : main.cpp tradecrypto.h producer.h consumer.h report.h
	$(CXX) $(CXXFLAGS) main.cpp

producer.o : producer.cpp producer.h tradecrypto.h report.h
	$(CXX) $(CXXFLAGS) producer.cpp

consumer.o : consumer.cpp consumer.h tradecrypto.h report.h
	$(CXX) $(CXXFLAGS) consumer.cpp

report.o : report.cpp report.h tradecrypto.h
	$(CXX) $(CXXFLAGS) report.cpp

# Clean up
clean :
	rm -f *.o $(PROGRAM)
