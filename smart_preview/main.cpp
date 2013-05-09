#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <QtCore>
#include <time.h>
#include "diff_match_patch.h"

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

bool is_signature(std::string str) {
  // remove white spaces and compare if first two chars are hypen and followed by a new line, 
  // it can have any number of white chars between this hypen and new line.

  // remove all white chars
  // compare new next bytes are -(hypen)
  // remove all space(' ')
  // compare next char is new line (May be we need to create an exception for html emails)
  for (size_t i = 0; i < str.length(); i++) {
    if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t') {
        str.erase(i, 1);
        i--;
    } else {
      break;
    }
  }

  if (str.length() == 0) {
    return true;
  }

  if (strnicmp(str.c_str(), "--", 2) != 0) {
    return false;
  }

  str.erase(0, 2);

  for (size_t i = 0; i < str.length(); i++) {
    if (str[i] == ' ' || str[i] == '\t') {
        str.erase(i, 1);
        i--;
    } else {
      break;
    }
  }

  if (str.length() == 0) {
    return true;
  }

  if (strnicmp(str.c_str(), "\n", 1) == 0) {
    return true;
  }

  return false;
}

void text_diff() {
#define OLD_MAIL "hi\n\
This is a test mail\n\
--\
MM"

#define NEW_MAIL "thanks\n\
Manoj\n\
hi\n\
This is a test mail\n\
test\n\
--\
MM"

  diff_match_patch dmp;
  
#define FILE_FETCH
//#define MACRO_FETCH

#ifdef FILE_FETCH
  std::ifstream ifs("oldmail.txt");
  std::string oldmail( (std::istreambuf_iterator<char>(ifs)),
                       (std::istreambuf_iterator<char>()));

  std::ifstream ifs2("newmail.txt");
  std::string newmail( (std::istreambuf_iterator<char>(ifs2)),
                       (std::istreambuf_iterator<char>()));

  QString str1 = QString(oldmail.c_str());
  QString str2 = QString(newmail.c_str());
#else
#ifdef MACRO_FETCH
  QString str1 = QString(OLD_MAIL);
  QString str2 = QString(NEW_MAIL);
#else
  QString str1 = QString("a");
  QString str2 = QString("d\na\nb\nc");
#endif
#endif
  QList<Diff> diffs = dmp.diff_main(str1, str2);
  //QPair<QString, QVector<bool> > out = dmp.patch_apply(dmp.patch_fromText(strPatch), str1);
  //QString strResult = out.first;

  std::string lastequalstr; 
  std::string laststr; 
  foreach (Diff myDiff, diffs) {
    laststr = "";
    if (myDiff.operation == INSERT) {
      //qDebug("%s[INSERT]\n", qPrintable(myDiff.text));
      laststr = myDiff.text.toUtf8().data();
    } else if (myDiff.operation == DELETE) {
      //qDebug("%s[DELETE]\n", qPrintable(myDiff.text));
    } else {
      //qDebug("%s[EQUAL]\n", qPrintable(myDiff.text));
      lastequalstr = myDiff.text.toUtf8().data();
      laststr = lastequalstr;
    }
  }

  if (laststr.length() == 0) {
    qDebug("Failed to find a match in last\n");
    return;
  }

  if (laststr != lastequalstr) { // check if laststr is a signature only
    qDebug("Last INSERT buffer\n%s\n", laststr.c_str());
    if (is_signature(laststr) == false) {
      return;
    }
  }

  std::string src = str2.toUtf8().data();
  char *ptr = (char*)src.c_str();
  char *ptrold = (char*)src.c_str();
  
  char *prevptr = NULL;

  while( (ptr = strstr(ptr, lastequalstr.c_str())))
  {
    // increment ptr here to prevent
    // an infinite loop
    prevptr = ptr++;
  }
  
  char out[10240] = {0};

  int size = prevptr-ptrold;

  strncpy(out, src.c_str(), size);
  printf("New text: \n%s\n", out);
}

int main() {
  //text_smart_preview();
  //html_smart_preview();

  text_diff();
  return 0;
}