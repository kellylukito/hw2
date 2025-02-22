CXX=g++
CXXFLAGS=-g -Wall -std=c++11
# Uncomment for parser DEBUG
#DEFS=-DDEBUG

# Add new object files
OBJS=amazon.o user.o db_parser.o product.o product_parser.o util.o book.o clothing.o movie.o mydatastore.o

all: amazon test_amazon

amazon: $(OBJS)
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ $(OBJS)

# Main program object files
amazon.o: amazon.cpp db_parser.h datastore.h product_parser.h mydatastore.h
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c amazon.cpp
user.o: user.cpp user.h
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c user.cpp
db_parser.o: db_parser.cpp db_parser.h product.h product_parser.h user.h datastore.h
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c db_parser.cpp
product.o: product.cpp product.h
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c product.cpp
product_parser.o: product_parser.cpp product_parser.h product.h book.h clothing.h movie.h
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c product_parser.cpp
util.o: util.cpp util.h
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c util.cpp

# New object files
book.o: book.cpp book.h product.h util.h
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c book.cpp
clothing.o: clothing.cpp clothing.h product.h util.h
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c clothing.cpp
movie.o: movie.cpp movie.h product.h util.h
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c movie.cpp
mydatastore.o: mydatastore.cpp mydatastore.h datastore.h product.h user.h
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c mydatastore.cpp

# Test program
test_amazon: test_amazon.o $(filter-out amazon.o, $(OBJS))
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ $^

test_amazon.o: test_amazon.cpp book.h clothing.h movie.h util.h mydatastore.h
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c test_amazon.cpp

clean:
	rm -f *.o amazon test_amazon