#include "mydatastore.h"
#include "util.h"
#include <iostream>

MyDataStore::MyDataStore() {}

MyDataStore::~MyDataStore() {
  // Delete all products
  std::set<Product*>::iterator pit;
  for(pit = products.begin(); pit != products.end(); ++pit) {
      delete *pit;
  }
  // Delete all users
  std::map<std::string, User*>::iterator uit;
  for(uit = users.begin(); uit != users.end(); ++uit) {
      delete uit->second;
  }
}

void MyDataStore::addUser(User* u) {
  if (u == nullptr) return;
  
  std::string username = convToLower(u->getName());
  users[username] = u;
  
  // Initialize an empty cart for the user if not exists
  if (carts.find(username) == carts.end()) {
      carts[username] = std::deque<Product*>();
  }
}

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type) {
  if(terms.empty()) {
      return std::vector<Product*>();
  }

  std::set<Product*> results;
  bool firstTerm = true;

  std::vector<std::string>::iterator termIt;
  for(termIt = terms.begin(); termIt != terms.end(); ++termIt) {
      if(termIt->empty()) continue;
      
      std::string searchTerm = convToLower(*termIt);
      std::set<Product*> termResults;

      // Partial keyword matching
      std::map<std::string, std::set<Product*>>::iterator mapIt;
      for(mapIt = keywordMap.begin(); mapIt != keywordMap.end(); ++mapIt) {
          if(mapIt->first.find(searchTerm) != std::string::npos) {
              termResults.insert(mapIt->second.begin(), mapIt->second.end());
          }
      }

      if(firstTerm) {
          results = termResults;
          firstTerm = false;
      } else {
          if(type == 0) {  // AND search
              results = setIntersection(results, termResults);
          } else {  // OR search
              results = setUnion(results, termResults);
          }
      }

      // For AND search, if no results, return empty
      if(type == 0 && results.empty()) {
          return std::vector<Product*>();
      }
  }
  
  return std::vector<Product*>(results.begin(), results.end());
}

void MyDataStore::dump(std::ostream& ofile) {
  ofile << "<products>" << std::endl;
  std::set<Product*>::iterator pit;
  for(pit = products.begin(); pit != products.end(); ++pit) {
      (*pit)->dump(ofile);
  }
  ofile << "</products>" << std::endl;
  
  ofile << "<users>" << std::endl;
  std::map<std::string, User*>::iterator uit;
  for(uit = users.begin(); uit != users.end(); ++uit) {
      uit->second->dump(ofile);
  }
  ofile << "</users>" << std::endl;
}

bool MyDataStore::isValidUser(std::string username) {
  return users.find(convToLower(username)) != users.end();
}

void MyDataStore::addProduct(Product* p) {
  if (p == nullptr) return;
  
  products.insert(p);
  
  // Get all keywords for the product and add to keyword map
  std::set<std::string> keywords = p->keywords();
  std::set<std::string>::iterator it;
  for(it = keywords.begin(); it != keywords.end(); ++it) {
      std::string lowercaseKeyword = convToLower(*it);
      
      // Optional length validation
      if(lowercaseKeyword.length() >= 2) {
          keywordMap[lowercaseKeyword].insert(p);
      }
  }
}



void MyDataStore::viewCart(std::string username) {
  username = convToLower(username);
  if(!isValidUser(username)) {
      std::cout << "Invalid username" << std::endl;
      return;
  }
  
  const std::deque<Product*>& cart = carts[username];
  int index = 1;
  std::deque<Product*>::const_iterator it;
  for(it = cart.begin(); it != cart.end(); ++it) {
      std::cout << "Item " << index++ << std::endl;
      std::cout << (*it)->displayString() << std::endl;
      std::cout << std::endl;
  }
}

void MyDataStore::buyCart(std::string username) {
  username = convToLower(username);
  if(!isValidUser(username)) {
      std::cout << "Invalid username" << std::endl;
      return;  // Return instead of throwing an exception
  }
  
  User* user = users[username];
  std::deque<Product*>& cart = carts[username];
  std::deque<Product*> remainingItems;
  
  std::deque<Product*>::iterator it = cart.begin();
  while(it != cart.end()) {
      Product* p = *it;
      
      if(p->getQty() > 0 && user->getBalance() >= p->getPrice()) {
          user->deductAmount(p->getPrice());
          p->subtractQty(1);
          it = cart.erase(it);
      } else {
          ++it;
      }
  }
}

void MyDataStore::addToCart(std::string username, Product* p) {
  // Convert username to lowercase for consistency
  username = convToLower(username);
  
  // Check if the user exists
  if (!isValidUser(username)) {
      std::cout << "Invalid username" << std::endl;
      return;
  }
  
  // Check if the product is valid
  if (p == nullptr) {
      std::cout << "Invalid product" << std::endl;
      return;
  }
  
  // Add the product to the user's cart
  carts[username].push_back(p);
}