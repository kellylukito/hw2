#include "book.h"
#include <sstream>
#include <iomanip>

Book::Book(const std::string category, const std::string name, double price, int qty,
           const std::string isbn, const std::string author) 
    : Product(category, name, price, qty), isbn_(isbn), author_(author) 
{
}

Book::~Book() { }

std::set<std::string> Book::keywords() const {
    std::set<std::string> result;
    
    // Add keywords from name
    std::set<std::string> nameWords = parseStringToWords(name_);
    std::cout << "Name keywords:" << std::endl;
    std::set<std::string>::iterator it;
    for(it = nameWords.begin(); it != nameWords.end(); ++it) {
        std::cout << *it << std::endl;
    }
    result = nameWords;
    
    // Add keywords from author
    std::set<std::string> authorWords = parseStringToWords(author_);
    std::cout << "Author keywords:" << std::endl;
    for(it = authorWords.begin(); it != authorWords.end(); ++it) {
        std::cout << *it << std::endl;
    }
    result = setUnion(result, authorWords);
    
    // Add ISBN
    result.insert(isbn_);
    
    return result;
}

std::string Book::displayString() const {
    std::stringstream ss;
    ss << name_ << "\n";
    ss << "Author: " << author_ << " ISBN: " << isbn_ << "\n";
    ss << std::fixed << std::setprecision(2) << price_ << " " << qty_ << " left.";
    return ss.str();
}

void Book::dump(std::ostream& os) const {
    Product::dump(os);
    os << isbn_ << "\n" << author_ << std::endl;
}