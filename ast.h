#ifndef AST_H
#define AST_H

#include <string>
#include <unordered_map>
#include <list>
#include <ostream>

using namespace std;

class Visitor; 

// Operadores binarios soportados
enum BinaryOp { 
    PLUS_OP, 
    MINUS_OP, 
    MUL_OP, 
    DIV_OP,
    POW_OP
};

enum SetOp {
    UNION_OP,
    INTERSECT_OP,
    DIFFERENCE_OP,
};

class PType {
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~PType() = 0;
};

// Clase abstracta Exp
class Exp : public PType {
public:
    virtual int  accept(Visitor* visitor) = 0;
    virtual ~Exp() = 0;  // Destructor puro → clase abstracta
    static string binopToChar(BinaryOp op);  // Conversión operador → string
};

// Expresión binaria
class BinaryExp : public Exp {
public:
    Exp* left;
    Exp* right;
    BinaryOp op;
    int accept(Visitor* visitor);
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    ~BinaryExp();
};

// Expresión numérica
class NumberExp : public Exp {
public:
    int value;
    int accept(Visitor* visitor);
    NumberExp(int v);
    ~NumberExp();
};

class IdExp : public Exp {
public:
    string value;
    int accept(Visitor* visitor);
    IdExp(string v);
    ~IdExp();
};

// Raiz cuadrada
class SqrtExp : public Exp {
public:
    Exp* value;
    int accept(Visitor* visitor);
    SqrtExp(Exp* v);
    ~SqrtExp();
}
;
class Stm{
public:
    virtual int  accept(Visitor* visitor) = 0;
    virtual ~Stm() = 0;  
};
class AssignStm: public Stm{
public:
    string id;
    PType* rhs;
    int accept(Visitor* visitor);
    AssignStm(string , PType* );
    ~AssignStm();
};

class PrintStm: public Stm{
public:
    PType* e;
    int accept(Visitor* visitor);
    PrintStm(PType*);
    ~PrintStm();
};

class Program{
public:
    list<Stm*> slist;
    int accept(Visitor* visitor);
    Program();
    ~Program();
};

class Set : public PType {
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual list<int> acceptSet(Visitor* visitor) = 0;
    virtual ~Set() = 0;
};

class BinarySetExp : public Set {
public:
    Set* s1;
    Set* s2;
    SetOp op;
    int accept(Visitor* visitor);
    list<int> acceptSet(Visitor* visitor);
    BinarySetExp(Set* s1, Set* s2, SetOp op);
    ~BinarySetExp();
};

class SetExp : public Set {
public:
    list<Set*> el;
    list<Exp*> ex;
    int accept(Visitor* visitor);
    list<int> acceptSet(Visitor* visitor);
    SetExp(list<Set*> el, list<Exp*> ex);
    ~SetExp();
};

class IdSet : public Set {
public:
    string value;
    int accept(Visitor* visitor);
    list<int> acceptSet(Visitor* visitor);
    IdSet(string v);
    ~IdSet();
};

#endif // AST_H


