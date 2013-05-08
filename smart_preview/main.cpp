#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void text_smart_preview() {
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

void html_smart_preview() {
  // find the location of all "<blockquote>" as well as "</blockquote>"

  std::ifstream ifs("test_html.txt");
  std::string content( (std::istreambuf_iterator<char>(ifs)),
                       (std::istreambuf_iterator<char>()));


  std::cout << "\n================================\n";
  std::cout << content << "\n";
  std::cout << "\n================================\n";

  std::vector<int> sbq;
  std::vector<int> ebq;
  
  int i = 0;
  while ((i = content.find("<blockquote", i)) !=  std::string::npos) {
    sbq.push_back(i);
    std::cout << "<blockquote> found in: " << i++ << "\n";
  }
  i = 0;
  while ((i = content.find("</blockquote>", i)) !=  std::string::npos) {
    ebq.push_back(i);
    std::cout << "</blockquote> found in: " << i++ << "\n";
  }

  for(i = 0; i < sbq.size(); i++) {
    std::cout << "<blockquote> found in: " << sbq.at(i) << "\n";
  }
  for(i = 0; i < ebq.size(); i++) {
    std::cout << "</blockquote> found in: " << ebq.at(i) << "\n";
  }

  // logic to find the last blockquote 
  int s = 0;
  int e = 0;

  int fs = 0;
  int fe = 0;
  int ss = 0;
  if (sbq.size() == 0 || ebq.size() == 0) {
    std::cout << "NOT FOUND\n";
    return;
  }
  fs = sbq.at(s++);
  fe = ebq.at(e++);

  /*
  if (sbq.size() == 1) {
    goto DONE;
  }
  ss = sbq.at(s++);

  while (1) {
    if (fe < ss) {
      if (ebq.size() == e) {
        break;
      }
      fe = ebq.at(e++);
      fs = ss;
      if (sbq.size() == s) {
        break;
      }
      ss = sbq.at(s++);
    } else {
      if (sbq.size() == s) {
        break;
      }
      ss = sbq.at(s++);
      if (ss > fe) {
        if (ebq.size() == e) {
          break;
        }
        fe = ebq.at(e++);
      }
    }
  }
  */


/////////////////////////////////////////////////////////

  int bq_count = 1;
  int ns = 1;

  if (sbq.size() == 1) {
    goto DONE;
  }
  ns = sbq.at(s++);
  while (1) {
    if (ns < fe) {
      bq_count++;
      if (sbq.size() == s) {
        if (ebq.size() == e) {
          break;
        }
        fe = ebq.at(e++);
        break;
      }
      ns = sbq.at(s++);
    } else {
      bq_count--;
      if (bq_count == 0) {
        if (sbq.size() == s) {
          break;
        }
        fs = sbq.at(s++);
      }
      if (ebq.size() == e) {
        break;
      }
      fe = ebq.at(e++);
    }
  }





/////////////////////////////////////////////////////////


DONE:
  // we need to check teh remaining buffer contains some VALUABLE text
  // ie, don't remove 
  // simple logic is to look for non HTML text after 'fe'
  std::cout << "\n================================\n";
  std::cout << "\n================================\n";
  std::cout << fs << " --- " << fe << "\n";
  std::cout << "\n================================\n";
}

int main() {
  //text_smart_preview();
  html_smart_preview();
  return 0;
}