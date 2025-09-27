#include <iostream>
#include <fstream>
#include <cmath>
#include "ast.h"
#include "visitor.h"
#include <algorithm>
#include <set>

using namespace std;
unordered_map<std::string, int> memoria;
///////////////////////////////////////////////////////////////////////////////////
int BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IdExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int SqrtExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int Program::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int AssignStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int PrintStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int SetExp::accept(Visitor* visitor) {
    return 0;
}

int BinarySetExp::accept(Visitor* visitor) {
    return 0;
}

int IdSet::accept(Visitor* visitor) {
    return 0;
}

list<int> SetExp::acceptSet(Visitor* visitor) {
    return visitor->visitSet(this);
}

list<int> BinarySetExp::acceptSet(Visitor* visitor) {
    return visitor->visitSet(this);
}

list<int> IdSet::acceptSet(Visitor* visitor) {
    return visitor->visitSet(this);
}

///////////////////////////////////////////////////////////////////////////////////

int PrintVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    exp->right->accept(this);
    return 0;
}

int PrintVisitor::visit(NumberExp* exp) {
    cout << exp->value;
    return 0;
}

int PrintVisitor::visit(SqrtExp* exp) {
    cout << "sqrt(";
    exp->value->accept(this);
    cout <<  ")";
    return 0;
}


void PrintVisitor::imprimir(Program* programa){
    if (programa)
    {
        cout << "Codigo:" << endl; 
        programa->accept(this);
        cout << endl;
    }
    return ;
}

///////////////////////////////////////////////////////////////////////////////////
int EVALVisitor::visit(BinaryExp* exp) {
    int result;
    int v1 = exp->left->accept(this);
    int v2 = exp->right->accept(this);
    switch (exp->op) {
        case PLUS_OP:
            result = v1 + v2;
            break;
        case MINUS_OP:
            result = v1 - v2;
            break;
        case MUL_OP:
            result = v1 * v2;
            break;
        case DIV_OP:
            if (v2 != 0)
                result = v1 / v2;
            else {
                cout << "Error: división por cero" << endl;
                result = 0;
            }
            break;
        case POW_OP:
            result = pow(v1,v2);
            break;
        default:
            cout << "Operador desconocido" << endl;
            result = 0;
    }
    return result;
}

int EVALVisitor::visit(NumberExp* exp) {
    return exp->value;
}

int EVALVisitor::visit(SqrtExp* exp) {
    return floor(sqrt( exp->value->accept(this)));
}

list<int> EVALVisitor::visitSet(SetExp* exp) {
    list<int> result;
    
    for (auto e : exp->ex) {
        int val = e->accept(this);
        result.push_back(val);
    }
    return result;
}

list<int> EVALVisitor::visitSet(BinarySetExp* exp) {
    list<int> set1 = exp->s1->acceptSet(this);
    list<int> set2 = exp->s2->acceptSet(this);
    set<int> result;
    
    switch (exp->op) {
        case UNION_OP:
            result.insert(set1.begin(), set1.end());
            result.insert(set2.begin(), set2.end());
            break;
        case INTERSECT_OP:
            for (int val : set1) {
                if (find(set2.begin(), set2.end(), val) != set2.end()) {
                    result.insert(val);
                }
            }
            break;
        case DIFFERENCE_OP:
            for (int val : set1) {
                if (find(set2.begin(), set2.end(), val) == set2.end()) {
                    result.insert(val);
                }
            }
            break;
    }
    
    return list<int>(result.begin(), result.end());
}

list<int> EVALVisitor::visitSet(IdSet* exp) {
    return list<int>();
}

void EVALVisitor::interprete(Program* programa){
    if (programa)
    {
        cout << "Interprete:";
        programa->accept(this);
        cout<<endl;
    }
    return;

}

///////////////////////////////////////////////////////////////////////

int EVALVisitor::visit(PrintStm* stm) {
    Set* set = dynamic_cast<Set*>(stm->e);
    if (set) {
        list<int> result = set->acceptSet(this);
        
        cout << "{";
        bool first = true;
        for (int val : result) {
            if (!first) cout << ",";
            cout << val;
            first = false;
        }
        cout << "}";
    } else {
        cout << stm->e->accept(this);
    }
    return 0;
}

int EVALVisitor::visit(AssignStm* stm) {
    memoria[stm->id] = stm->rhs->accept(this);
    return 0;
}
int EVALVisitor::visit(IdExp* exp) {
    return memoria[exp->value];
}


int PrintVisitor::visit(IdExp* exp) {
    cout << exp->value;
    return 0;
}
int EVALVisitor::visit(Program* p) {
    for(auto i:p->slist) {
        i->accept(this);
    }
    return 0;
}

int PrintVisitor::visit(PrintStm* stm) {
    cout << "print(";
    Set* set = dynamic_cast<Set*>(stm->e);
    if (set) {
        set->acceptSet(this);
    } else {
        stm->e->accept(this);
    }
    cout << ")" << endl;
    return 0;
}

int PrintVisitor::visit(AssignStm* stm) {
    cout << stm->id << " = ";
    Set* set = dynamic_cast<Set*>(stm->rhs);
    if (set) {
        set->acceptSet(this);
    } else {
        stm->rhs->accept(this);
    }
    cout << endl;
    return 0;
}

int PrintVisitor::visit(Program* p) {
    for(auto i:p->slist) {
        i->accept(this);
    }
    return 0;
}

list<int> PrintVisitor::visitSet(SetExp* exp) {
    cout << "{";
    bool first = true;
    
    for (auto e : exp->ex) {
        if (!first) cout << ",";
        e->accept(this);
        first = false;
    }
        
    cout << "}";
    return list<int>();
}

list<int> PrintVisitor::visitSet(BinarySetExp* exp) {
    exp->s1->acceptSet(this);
    switch (exp->op) {
        case UNION_OP:
            cout << " cup ";
            break;
        case INTERSECT_OP:
            cout << " cap ";
            break;
        case DIFFERENCE_OP:
            cout << " \\ ";
            break;
        default:
            cout << " ? ";
    }
    exp->s2->acceptSet(this);
    return list<int>();
}

list<int> PrintVisitor::visitSet(IdSet* exp) {
    cout << exp->value;
    return list<int>();
}