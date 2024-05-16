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
  void check_class(Class_ obj);
  void check_method(Feature obj);
  // void check_classes_inheritance();
  // void check_feature_unique();

private:
  Classes classes_;

  std::unordered_map<std::string, std::string> inheritance_hierarchy_;

  std::unordered_set<std::string> classes_names_;
  std::unordered_set<std::string> features_names_;
};

} // namespace sa
