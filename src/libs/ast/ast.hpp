#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

using namespace std;

class Value; 
class NodeVisitor;

// Define the AST node types
enum class AstNodeType {
    // Root
    Program,
    
    // Statements
    Print, While, If, 

    // Expressions
    UnaryOp, BinaryOp, TernaryOp,
    Assign, Call,

    // Literals
    Number, Boolean, Name, Null
};

class AstNode {

public:

    AstNode(AstNodeType type) : type(type) {}
    
    virtual ~AstNode() {}

    virtual Value* accept(NodeVisitor* visitor) = 0; 

    AstNodeType type;
    
    bool is_name_node() { return type == AstNodeType::Name; }
};

class PrintNode : public AstNode {

public:

    PrintNode(AstNode* args) 
      : AstNode(AstNodeType::Print),  args(args) {}

    virtual Value *accept(NodeVisitor* visitor) override;

    AstNode* args;
};

class ProgramNode : public AstNode {
public:

    ProgramNode(std::vector<AstNode*> statements) 
      : AstNode(AstNodeType::Program) , statements(statements) {}
    virtual ~ProgramNode() {}
    
    virtual Value *accept(NodeVisitor* visitor) override;

    std::vector<AstNode*> statements;
};

class TernaryOpNode : public AstNode {
public:
    TernaryOpNode(AstNode* condition, AstNode* left, AstNode* right) 
      : AstNode(AstNodeType::TernaryOp), cond(condition), left(left), right(right) {}

    AstNode *cond;
    AstNode *left;
    AstNode *right;

    virtual Value *accept(NodeVisitor* visitor) override;
};

class BinaryOpNode : public AstNode {
public:

    BinaryOpNode(AstNode* left, std::string op, AstNode* right) 
      : AstNode(AstNodeType::BinaryOp), left(left), op(op), right(right) {}

    virtual Value *accept(NodeVisitor* visitor) override;

    AstNode* left;
    std::string op;
    AstNode* right;
};

class UnaryOpNode : public AstNode {
public:
    UnaryOpNode(string op, AstNode* right) 
      : AstNode(AstNodeType::UnaryOp), op(op), right(right) {}

    std::string op;
    AstNode* right;

    virtual Value *accept(NodeVisitor* visitor) override;
};

class NumNode : public AstNode {
public:
    NumNode(double value) 
      : AstNode(AstNodeType::Number), value(value) {}

    virtual Value *accept(NodeVisitor* visitor) override;

    double value;
};

class NameNode : public AstNode {
public:
    NameNode(const std::string& name) 
      : AstNode(AstNodeType::Name), name(name) {}
    std::string name;

    virtual Value *accept(NodeVisitor* visitor) override;
};

class BooleanNode : public AstNode {
public:
    BooleanNode(bool value) 
      : AstNode(AstNodeType::Boolean), value(value) {}
    bool value;

    virtual Value *accept(NodeVisitor* visitor) override;
};

class NullNode : public AstNode {
public:
    NullNode() : AstNode(AstNodeType::Null) {}

    virtual Value *accept(NodeVisitor* visitor) override;
};

class CallNode : public AstNode {
public:
    CallNode(AstNode* name, std::vector<AstNode*> args) 
      : AstNode(AstNodeType::Call), name(name), args(args) {}

    AstNode* name;
    std::vector<AstNode*> args;

    virtual Value *accept(NodeVisitor * visitor) override;
};

class AssignNode : public AstNode {
public:
    AssignNode(AstNode* name, AstNode* value) 
      : AstNode(AstNodeType::Assign), name(name), value(value) {}

    virtual Value *accept(NodeVisitor* visitor) override;

    AstNode* name;
    AstNode* value;
};

class WhileNode : public AstNode {
public:

    WhileNode(AstNode* cond, std::vector<AstNode*> *stmts) 
      : AstNode(AstNodeType::While), cond(cond), stmts(stmts) {}

    AstNode* cond;
    std::vector<AstNode*> *stmts;

    virtual Value *accept(NodeVisitor* visitor) override;
};

class IfNode : public AstNode {
public:

    IfNode(AstNode* cond, std::vector<AstNode*> *stmts) 
      : AstNode(AstNodeType::If), cond(cond), stmts(stmts) {}

    AstNode* cond;
    std::vector<AstNode*> *stmts;

    virtual Value *accept(NodeVisitor* visitor) override;
};

class NodeVisitor {

public:

    ~NodeVisitor() = default;

    virtual Value* visitProgramNode(ProgramNode* node) = 0;
    virtual Value* visitPrintNode(PrintNode* node) = 0;
    virtual Value* visitWhileNode(WhileNode* node) = 0;
    virtual Value* visitIfNode(IfNode* node) =  0;
    virtual Value* visitAssignNode(AssignNode*  node) = 0;
    virtual Value* visitTernaryOpNode(TernaryOpNode* node) = 0;
    virtual Value* visitBinaryOpNode(BinaryOpNode* node) = 0;
    virtual Value* visitUnaryOpNode(UnaryOpNode* node)  = 0;
    virtual Value* visitNumNode(NumNode* node) = 0;
    virtual Value* visitNameNode(NameNode* node) = 0;
    virtual Value* visitBooleanNode(BooleanNode* node)  = 0;
    virtual Value* visitNullNode(NullNode* node) = 0;
  //  virtual Value * visitCallNode(const CallNode*  expr) = 0;
};

