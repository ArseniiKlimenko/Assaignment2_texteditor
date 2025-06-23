#ifndef TEXT_H
#define TEXT_H
#include <fstream>
#include <string>
#include <vector>

#include "Line.h"


class Text {
  std::vector<Line*> lines;

 public:
  void clear() {
   for (auto p : lines) delete p;
    lines.clear();
   }

  ~Text(){
   clear();
  }


 void add(Line* ln) {
  lines.push_back(ln);
 }

 size_t size() const {
  return lines.size();
 }

 Line* get(size_t i) {
  return lines[i];
 }

std::vector<std::string> state() const {
  std::vector<std::string> out;
  for (auto p : lines)
    out.push_back(p->serialize());
   return out;


 }


void restore(const std::vector<std::string>& state) {
  clear();
  for (auto& s : state) {
   lines.push_back(Line::deserialize(s));
  }
 }

bool SaveToFile(const std::string& path) {
  std::ofstream out(path);
  if (!out) return false;
  for (auto p : lines) {
   out << p->serialize() << "\n";
  }
  return true;
 }
bool LoadFromFile(const std::string& path) {
  std::ifstream in(path);
  if (!in) return false;
  clear();
  std::string buffer;
  while (std::getline(in, buffer)) {
   lines.push_back(Line::deserialize(buffer));
  }
  return true;
 }

void printAll() const {
  for (auto p : lines)
   p->print();
 }

};


#endif //TEXT_H
