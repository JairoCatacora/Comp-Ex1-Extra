#ifndef VISITOR_H
#define VISITOR_H
#include "ast.h"
#include <list>

class BinaryExp;
class NumberExp;
class SqrtExp;
class Program;
class PrintStm;
class AssignStm;

class Visitor {
public:
    virtual int visit(BinaryExp* exp) = 0;
    virtual int visit(NumberExp* exp) = 0;
    virtual int visit(IdExp* exp) = 0;
    virtual int visit(SqrtExp* exp) = 0;
    virtual int visit(Program* exp) = 0;
    virtual int visit(PrintStm* exp) = 0;
    virtual int visit(AssignStm* exp) = 0;
    virtual list<int> visitSet(SetExp* exp) = 0;
    virtual list<int> visitSet(BinarySetExp* exp) = 0;
    virtual list<int> visitSet(IdSet* exp) = 0;
};

class PrintVisitor : public Visitor {
public:

    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(SqrtExp* exp) override;
    int visit(PrintStm* exp) override;
    int visit(AssignStm* exp) override;
    int visit(Program* exp) override;
    int visit(IdExp* exp) override;
    list<int> visitSet(SetExp* exp) override;
    list<int> visitSet(BinarySetExp* exp) override;
    list<int> visitSet(IdSet* exp) override;
    void imprimir(Program* program);
};

class EVALVisitor : public Visitor {
public:
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(SqrtExp* exp) override;
    int visit(PrintStm* exp) override;
    int visit(IdExp* exp) override;
    int visit(AssignStm* exp) override;
    int visit(Program* exp) override;
    list<int> visitSet(SetExp* exp) override;
    list<int> visitSet(BinarySetExp* exp) override;
    list<int> visitSet(IdSet* exp) override;
    void interprete(Program* program);
};



#endif // VISITOR_H