#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main() {
  std::ifstream in("test.txt");

  if (in.is_open()) {
    std::vector<std::string> lines_in_order;
    std::string line;
    std::cout << "\n================================\n";
    while (std::getline(in, line)) {
        std::cout << line << "\n";
        lines_in_order.push_back(line);
    }

    std::cout << "\n================================\n";
    //std::reverse(lines_in_order.begin(), lines_in_order.end());
    //for(int t = 0; t < lines_in_order.size(); t++) {
      //  std::cout << lines_in_order.at(t) << "\n";
    //}

    for(int t = lines_in_order.size() - 1; t >= 0; t--) {
        line = lines_in_order.at(t);
        
        int i = 0;
        while(line.length() && line.at(i) == ' ') {
          i++;
        }
        if (line.length() == 0 || line.at(i) == '>') {
          std::cout << line;
          std::cout << " - IGNORED\n";
          lines_in_order.erase(lines_in_order.begin()+t);
        } else {
          break;
        }

    }

    std::cout << "\n================================\n";

    for(int t = 0; t < lines_in_order.size(); t++) {
      std::cout << lines_in_order.at(t) << "\n";
    }

    std::cout << "\n================================\n";
  }
}