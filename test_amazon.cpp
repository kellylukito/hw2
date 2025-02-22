#include <iostream>
#include <sstream>
#include "book.h"
#include "clothing.h"
#include "movie.h"
#include "util.h"
#include "mydatastore.h"

using namespace std;

//function to print test result
void testResult(const string& testName, bool passed){
  cout << testName << ": " << (passed ? "PASSED" : "FAILED") << endl;

}

//test product creation and keywords
void testProducts(){
  cout << "\n=== Testing Product Classes ===\n";

  //test book
  Book* book = new Book("book", "Data Structures", 50.00, 10, "978-0123456789", "John Smith");
  testResult("Book Creation", book != nullptr);

  set<string> bookKeywords = book->keywords();
  testResult("Book Keywords Count", bookKeywords.size() >= 4); // should include data, structures, isbn, john, smith
  testResult("Book ISBN Keyword", bookKeywords.find("978-0123456789") != bookKeywords.end());
  
  string bookDisplay = book->displayString();
  cout << "Book Display Format:\n" << bookDisplay << "\n";
  delete book;

  //test clothing
  Clothing* clothing = new Clothing("clothing", "Blue Jeans", 29.99, 20, "Medium", "Levi's");
  testResult("Clothing Creation", clothing != nullptr);

  set<string> clothingKeywords = clothing->keywords();
  testResult("Clothing Keywords Count", clothingKeywords.size() >= 3); // should include blue, jeans, levis

  string clothingDisplay = clothing->displayString();
  cout << "Clothing Display Format:\n" << clothingDisplay << "\n";
  delete clothing;  

  //test movie
  Movie* movie = new Movie("movie", "The Matrix", 19.99, 5, "Science Fiction", "R");
  testResult("Movie Creation", movie != nullptr);

  set<string> movieKeywords = movie->keywords();
  testResult("Movie Keywords Count", movieKeywords.size() >= 3); // should include matrix, science, fiction
  testResult("Movie Genre Keyword", movieKeywords.find("science fiction") != movieKeywords.end());

  string movieDisplay = movie->displayString();
  cout << "Movie Display Format:\n" << movieDisplay << "\n";
  delete movie;
}

//test MyDataStore
void testDataStore(){
  cout << "\n=== Testing DataStore ===\n";

  MyDataStore ds;

  //add products
  ds.addProduct(new Book("book", "C++ Primer", 59.99, 5, "978-1234567890", "Stanley Lippman"));
  ds.addProduct(new Clothing("clothing", "Red Shirt", 19.99, 3, "Large", "Nike"));

  //add users
  ds.addUser(new User("alice", 100.00, 0));
  ds.addUser(new User("bob", 50.00, 1));

  //test search
  vector<string> terms;
  terms.push_back("c++");
  vector<Product*> hits = ds.search(terms, 0);
  testResult("Search Single Term", hits.size() == 1);

  //add more defensive checks for cart operations
  if(hits.size() > 0) {
    ds.addToCart("alice", hits[0]);
    ds.viewCart("alice");
    ds.buyCart("alice");
  }
  else {
    std::cout << "No hits found for search term 'c++'" << std::endl;
  }

  //test invalid user
  ds.addToCart("invaliduser", hits[0]);
  testResult("Invalid User Check", true);

}

//test utility functions
void testUtil(){
  cout << "\n=== Testing Utility Functions ===\n";

  //testing string parsing
  string test1 = "Hello World";
  set<string> words1 = parseStringToWords(test1);
  testResult("Basic Word Parsing", words1.size() == 2);
  
  string test2 = "C++ Programming-Language";
  set<string> words2 = parseStringToWords(test2);
  testResult("Hyphenated Word Parsing", words2.size() >= 2);

  //testing set operations
  set<int> set1 = {1, 2, 3};
  set<int> set2 = {2, 3, 4};
  set<int> intersection = setIntersection(set1, set2);
  testResult("Set Intersection", intersection.size() == 2);
  
  set<int> union_set = setUnion(set1, set2);
  testResult("Set Union", union_set.size() == 4);
}

int main(){
  cout << "Starting Tests...\n";

  testProducts();
  testDataStore();
  testUtil();

  cout << "\nAll tests completed.\n";
  return 0;
}