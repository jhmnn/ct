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
  void error(std::string msg);

  Class_ find_class(std::string name);
  Feature find_feature(Class_ obj, std::string name);

  bool check_signatures(Feature obj);
  void check_vars(Expressions objs);
  void check_formals(Formals objs);
  void check_method(Feature obj);
  void check_features(Features objs);
  void check_classes(Classes objs);

 private:
  Classes classes_;

  std::unordered_map<std::string, std::string> inheritance_hierarchy_;

  std::unordered_set<std::string> classes_names_;
  std::unordered_set<std::string> features_names_;
  std::unordered_set<std::string> vars_names_;

  GetName gnv_;
  GetType gtv_;
  GetParentName gpnv_;
  GetFeatures gftv_;
  GetFormals gfrv_;
  GetExpression gev_;
  GetExpressions gesv_;

  std::string class_name_;
  std::string parent_name_;
  std::string feature_name_;
};

}  // namespace sa
