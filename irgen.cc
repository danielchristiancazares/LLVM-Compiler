/* irgen.cc -  LLVM IR generator
 *
 * You can implement any LLVM related functions here.
 */

#include "irgen.h"
#include "ast_type.h"

IRGenerator::IRGenerator() :
    context(NULL),
    module(NULL),
    currentFunc(NULL),
    currentBB(NULL) {
}

IRGenerator::~IRGenerator() {
}

llvm::Type *IRGenerator::Converter(Type *astTy) {
  llvm::Type *ty = NULL;
  if(astTy == Type::intType) {
    ty = llvm::Type::getInt32Ty(*context);
  } else if(astTy == Type::voidType) {
    ty = llvm::Type::getVoidTy (*context);
  } else if(astTy == Type::boolType) {
    ty = llvm::Type::getInt1Ty(*context);
  } else if(astTy == Type::floatType) {
    ty = llvm::Type::getFloatTy(*context);
  } else if(astTy == Type::vec2Type) {
    ty = llvm::VectorType::get(llvm::Type::getFloatTy(*context), 2);
  } else if(astTy == Type::vec3Type) {
    ty = llvm::VectorType::get(llvm::Type::getFloatTy(*context), 3);
  } else if(astTy == Type::vec4Type) {
    ty = llvm::VectorType::get(llvm::Type::getFloatTy(*context), 4);
  } else if(astTy == Type::mat2Type) {
    ty = llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(*context), 2), 2);
  } else if(astTy == Type::mat3Type) {
    ty = llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(*context), 3), 3);
  } else if(astTy == Type::mat4Type) {
    ty = llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(*context), 4), 4);
  } 
  /*else if(astTy == Type::ArrayType) {
    Type *arraytype = astTy->GetElemType();
    int elemCount = astTy->getElemCount();
    if(arraytype == Type::intType) {
      ty = llvm::ArrayType::get(llvm::Type::getInt32Ty(*context), elemCount);
    } else if(arraytype == Type::boolType) {
      ty = llvm::ArrayType::get(llvm::Type::getInt1Ty(*context), elemCount);
    } else if(arraytype == Type::floatType) {
      ty = llvm::ArrayType::get(llvm::Type::getFloatTy(*context), elemCount);
    } else if(arraytype == Type::vec2Type) {
      ty = llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(*context), 2), elemCount);
    } else if(arraytype == Type::vec3Type) {
      ty = llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(*context), 3), elemCount);
    } else if(arraytype == Type::vec4Type) {
      ty = llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(*context), 4), elemCount);
    } else if(arraytype == Type::mat2Type) {
      ty = llvm::ArrayType::get(llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(*context), 2), 2), elemCount);
    } else if(arraytype == Type::mat3Type) {
      ty = llvm::ArrayType::get(llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(*context), 3), 3), elemCount);
    } else if(arraytype == Type::mat4Type) {
      ty = llvm::ArrayType::get(llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(*context), 4), 4), elemCount);
    }
  }*/
  return ty;
}

llvm::Module *IRGenerator::GetOrCreateModule(const char *moduleID) {
  if(module == NULL) {
    context = new llvm::LLVMContext();
    module = new llvm::Module(moduleID, *context);
    module->setTargetTriple(TargetTriple);
    module->setDataLayout(TargetLayout);
  }
  return module;
}

void IRGenerator::SetFunction(llvm::Function *func) {
  currentFunc = func;
}

llvm::Function *IRGenerator::GetFunction() const {
  return currentFunc;
}

void IRGenerator::SetBasicBlock(llvm::BasicBlock *bb) {
  currentBB = bb;
}

llvm::BasicBlock *IRGenerator::GetBasicBlock() const {
  return currentBB;
}

llvm::Type *IRGenerator::GetIntType() const {
  llvm::Type *ty = llvm::Type::getInt32Ty(*context);
  return ty;
}

llvm::Type *IRGenerator::GetBoolType() const {
  llvm::Type *ty = llvm::Type::getInt1Ty(*context);
  return ty;
}

llvm::Type *IRGenerator::GetFloatType() const {
  llvm::Type *ty = llvm::Type::getFloatTy(*context);
  return ty;
}

llvm::Type *IRGenerator::GetVec2Type() const {
  llvm::Type *ty = llvm::VectorType::get(llvm::Type::getFloatTy(*context), 2);
  return ty;
}

llvm::Type *IRGenerator::GetVec3Type() const {
  llvm::Type *ty = llvm::VectorType::get(llvm::Type::getFloatTy(*context), 3);
  return ty;
}

llvm::Type *IRGenerator::GetVec4Type() const {
  llvm::Type *ty = llvm::VectorType::get(llvm::Type::getFloatTy(*context), 4);
  return ty;
}

llvm::Type *IRGenerator::GetMat2Type() const {
  llvm::Type *ty = llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(*context), 2), 2);
  return ty;
}

llvm::Type *IRGenerator::GetMat3Type() const {
  llvm::Type *ty = llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(*context), 3), 3);
  return ty;
}

llvm::Type *IRGenerator::GetMat4Type() const {
  llvm::Type *ty = llvm::ArrayType::get(llvm::VectorType::get(llvm::Type::getFloatTy(*context), 4), 4);
  return ty;
}

const char *IRGenerator::TargetLayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128";

const char *IRGenerator::TargetTriple = "x86_64-redhat-linux-gnu";

