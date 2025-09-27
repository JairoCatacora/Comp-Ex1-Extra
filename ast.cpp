#include "ast.h"

#include <complex.h>
#include <iostream>

using namespace std;

PType::~PType() {}

// ------------------ Exp ------------------
Exp::~Exp() {}

string Exp::binopToChar(BinaryOp op) {
    switch (op) {
        case PLUS_OP:  return "+";
        case MINUS_OP: return "-";
        case MUL_OP:   return "*";
        case DIV_OP:   return "/";
        case POW_OP:   return "**";
        default:       return "?";
    }
}

// ------------------ BinaryExp ------------------
BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp o)
    : left(l), right(r), op(o) {}

    
BinaryExp::~BinaryExp() {
    delete left;
    delete right;
}



// ------------------ NumberExp ------------------
NumberExp::NumberExp(int v) : value(v) {}

NumberExp::~NumberExp() {}


// ------------------ SqrtExp ------------------
SqrtExp::SqrtExp(Exp* v) : value(v) {}

SqrtExp::~SqrtExp() {}

///////////////////////////////////////////////
Program::Program(){}
Program::~Program(){}
Stm::~Stm(){}
PrintStm::PrintStm(PType* e1){
    e=e1;
}
PrintStm::~PrintStm(){}
AssignStm::AssignStm(string id1, PType* rhs1){
    id = id1;
    rhs=rhs1;
}
AssignStm::~AssignStm(){}

IdExp::IdExp(string v) {
    value=v;
}
IdExp::~IdExp() {
}

Set::~Set() {}

BinarySetExp::BinarySetExp(Set* s1, Set* s2, SetOp op): s1(s1), s2(s2), op(op) {}
BinarySetExp::~BinarySetExp() {}

SetExp::SetExp(list<Set*> el, list<Exp*> ex): el(el), ex(ex) {}
SetExp::~SetExp() {}

IdSet::IdSet(string v) : value(v) {}
IdSet::~IdSet() {}