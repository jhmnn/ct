#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "cool-parse.h"
#include "cool-tree.h"

namespace sa {

class SemanticAnalyzer {
 public:
  SemanticAnalyzer(Classes classes);
  ~SemanticAnalyzer() = default;

  void analyze();

 private:
  void check_classes_unique();
  void check_classes_inheritance();

 private:
  Classes classes_;
  std::unordered_set<std::string> classes_names_;

  std::unordered_map<std::string, std::string> inheritance_hierarchy_;
};

}  // namespace sa
