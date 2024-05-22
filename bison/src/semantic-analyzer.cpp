#include "semantic-analyzer.hpp"

namespace sa {

void SemanticAnalyzer::error(std::string msg) {
  std::cerr << "error: " << msg << '\n';
}

SemanticAnalyzer::SemanticAnalyzer(Classes classes) : classes_(classes) {
  classes_names_ = {"Object", "Bool", "Int", "String", "SELF_TYPE"};
}

Class_ SemanticAnalyzer::find_class(std::string name) {
  for (int i = classes_->first(); classes_->more(i); i = classes_->next(i)) {
    auto obj = classes_->nth(i);
    obj->accept(gnv_);
    if (gnv_.value == name) {
      return obj;
    }
  }

  return nullptr;
}

Feature SemanticAnalyzer::find_feature(Class_ obj, std::string name) {
  obj->accept(gftv_);
  auto features = gftv_.value;
  for (int i = features->first(); features->more(i); i = features->next(i)) {
    auto feature = features->nth(i);
    feature->accept(gnv_);
    if (gnv_.value == name) {
      return feature;
    }
  }

  return nullptr;
}

bool SemanticAnalyzer::check_signatures(Feature obj) {
  if (parent_name_ == "Object") {
    return true;
  }

  if (classes_names_.find(parent_name_) == classes_names_.end()) {
    return true;
  }

  auto feature = find_feature(find_class(parent_name_), feature_name_);
  if (feature == nullptr) {
    return true;
  }

  obj->accept(gtv_);
  std::string type1 = gtv_.value;
  feature->accept(gtv_);
  std::string type2 = gtv_.value;

  if (type1 != type2) {
    return false;
  }

  obj->accept(gfrv_);
  auto formals1 = gfrv_.value;
  feature->accept(gfrv_);
  auto formals2 = gfrv_.value;

  if (formals1->len() != formals2->len()) {
    return false;
  }

  for (int i = formals1->first(); formals1->more(i); i = formals1->next(i)) {
    auto formal1 = formals1->nth(i);
    formal1->accept(gnv_);
    formal1->accept(gtv_);
    std::string name1 = gnv_.value;
    std::string type1 = gtv_.value;
    auto formal2 = formals2->nth(i);
    formal2->accept(gnv_);
    formal2->accept(gtv_);
    std::string name2 = gnv_.value;
    std::string type2 = gtv_.value;

    if (name1 != name2) {
      return false;
    }

    if (type1 != type2) {
      return false;
    }
  }

  return true;
}

void SemanticAnalyzer::check_vars(Expressions objs) {
  for (int i = objs->first(); objs->more(i); i = objs->next(i)) {
    auto obj = objs->nth(i);
    if (obj->type_of() != Expression_class::Type::Let) {
      continue;
    }

    obj->accept(gnv_);
    obj->accept(gtv_);
    std::string var_name = gnv_.value;
    std::string type = gtv_.value;

    bool is_inserted = vars_names_.insert(var_name).second;
    if (!is_inserted) {
      error("redefinition of var " + var_name);
    }

    if (var_name == "self") {
      error("don't use 'self' as name");
    }

    if (classes_names_.find(type) == classes_names_.end()) {
      error("unknown type " + type);
    }
  }
}

void SemanticAnalyzer::check_formals(Formals objs) {
  vars_names_.clear();

  for (int i = objs->first(); objs->more(i); i = objs->next(i)) {
    auto obj = objs->nth(i);
    obj->accept(gnv_);
    obj->accept(gtv_);
    std::string formal_name = gnv_.value;
    std::string type = gtv_.value;

    bool is_inserted = vars_names_.insert(formal_name).second;
    if (!is_inserted) {
      error("redefinition of formal " + formal_name);
    }

    if (formal_name == "self") {
      error("don't use 'self' as name");
    }

    if (classes_names_.find(type) == classes_names_.end()) {
      error("unknown type " + type);
    }
  }
}

void SemanticAnalyzer::check_method(Feature obj) {
  obj->accept(gfrv_);
  Formals formals = gfrv_.value;

  check_formals(formals);

  obj->accept(gev_);
  Expression expr = gev_.value;
  if (expr->type_of() == Expression_class::Type::Block) {
    expr->accept(gesv_);
    Expressions exprs = gesv_.value;

    check_vars(exprs);
  }

  if (!check_signatures(obj)) {
    error("method " + feature_name_ +
          " does not override method from base class ");
  }
}

void SemanticAnalyzer::check_features(Features objs) {
  features_names_.clear();
  for (int i = objs->first(); objs->more(i); i = objs->next(i)) {
    auto obj = objs->nth(i);
    obj->accept(gnv_);
    obj->accept(gtv_);
    feature_name_ = gnv_.value;
    std::string type = gtv_.value;

    bool is_inserted = features_names_.insert(feature_name_).second;
    if (!is_inserted) {
      error("redefinition of feature " + feature_name_);
    }

    if (feature_name_ == "self") {
      error("don't use 'self' as name");
    }

    if (classes_names_.find(type) == classes_names_.end()) {
      error("unknown type " + type);
    }

    if (obj->type_of() == Feature_class::Type::Method) {
      check_method(obj);
    }
  }

  if (class_name_ == "Main") {
    if (features_names_.find("main") == features_names_.end()) {
      error("no main method in Main class");
    }
  }
}

void SemanticAnalyzer::check_classes(Classes objs) {
  std::unordered_set<std::string> builtin_classes{"Bool", "Int", "String",
                                                  "SELF_TYPE"};
  for (int i = objs->first(); objs->more(i); i = objs->next(i)) {
    auto obj = objs->nth(i);
    obj->accept(gnv_);
    obj->accept(gpnv_);
    obj->accept(gftv_);
    class_name_ = gnv_.value;
    parent_name_ = gpnv_.value;
    Features features = gftv_.value;

    bool is_inserted = classes_names_.insert(class_name_).second;
    if (!is_inserted) {
      error("redefinition of class " + class_name_);
    }

    inheritance_hierarchy_[class_name_] = parent_name_;

    if (parent_name_ != "Object") {
      if (classes_names_.find(parent_name_) == classes_names_.end()) {
        error("class " + parent_name_ + " doesn't exist");
      } else if (class_name_ == parent_name_) {
        error("invalid use of incomplete type 'class " + class_name_ + "'");
      } else if (builtin_classes.find(parent_name_) != builtin_classes.end()) {
        error("class " + parent_name_ + " can't be a parent");
      } else if (inheritance_hierarchy_.find(parent_name_) ==
                 inheritance_hierarchy_.end()) {
        error("unknown class " + parent_name_);
      } else if (inheritance_hierarchy_[class_name_] == parent_name_ &&
                 inheritance_hierarchy_[parent_name_] == class_name_) {
        error("inheritance loop: " + class_name_ + " <-> " + parent_name_ + "");
      }
    }

    check_features(features);
  }

  if (classes_names_.find("Main") == classes_names_.end()) {
    error("no Main class");
  }
}

void SemanticAnalyzer::analyze() { check_classes(classes_); }

}  // namespace sa
