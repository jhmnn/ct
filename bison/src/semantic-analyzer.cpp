#include "semantic-analyzer.hpp"

namespace sa {

void error(std::string msg) { std::cerr << "error: " << msg << '\n'; }

SemanticAnalyzer::SemanticAnalyzer(Classes classes) : classes_(classes) {
  classes_names_ = {"Object", "Bool", "Int", "String", "SELF_TYPE"};
}

void SemanticAnalyzer::check_method(Feature obj) {
  return;
}

void SemanticAnalyzer::check_class(Class_ obj) {
  GetName gnv;
  GetParentName gpnv;
  
  std::unordered_set<std::string> builtin_classes{"Bool", "Int", "String",
                                                  "SELF_TYPE"};

    obj->accept(gnv);
    obj->accept(gpnv);
    std::string class_name = gnv.value;
    std::string parent_name = gpnv.value;

    auto is_inserted = classes_names_.insert(class_name).second;
    if (!is_inserted) {
      error("redefinition of class " + class_name);
    }

    inheritance_hierarchy_[class_name] = parent_name;
    
    if (parent_name == "Object") {
      return;
    }

    if (classes_names_.find(parent_name) == classes_names_.end()) {
      error("class " + parent_name + " doesn't exist");
    } else if (class_name == parent_name) {
      error("invalid use of incomplete type 'class " + class_name + "'");
    } else if (builtin_classes.find(parent_name) != builtin_classes.end()) {
      error("class " + parent_name + " can't be a parent");
    } else if (inheritance_hierarchy_.find(parent_name) ==
               inheritance_hierarchy_.end()) {
      error("unknown class " + parent_name);
    } else if (inheritance_hierarchy_[class_name] == parent_name &&
               inheritance_hierarchy_[parent_name] == class_name) {
      error("inheritance loop: " + class_name + " <-> " + parent_name + "");
    }

}

void SemanticAnalyzer::analyze() {
  for (int i = classes_->first(); classes_->more(i); i = classes_->next(i)) {
    check_class(classes_->nth(i));
  }
}

} // namespace sa
