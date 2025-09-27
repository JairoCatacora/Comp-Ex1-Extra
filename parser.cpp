#include <iostream>
#include <stdexcept>
#include "token.h"
#include "scanner.h"
#include "ast.h"
#include "parser.h"

using namespace std;

// =============================
// Métodos de la clase Parser
// =============================

Parser::Parser(Scanner* sc) : scanner(sc) {
    previous = nullptr;
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        throw runtime_error("Error léxico");
    }
}

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;

        if (check(Token::ERR)) {
            throw runtime_error("Error lexico");
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}


// =============================
// Reglas gramaticales
// =============================

Program* Parser::parseProgram() {
    Program * programa = new Program();
    programa->slist.push_back(parseStm());
    while(match(Token::SEMICOL)) {
        programa->slist.push_back(parseStm());
    }

    if (!isAtEnd()) {
        throw runtime_error("Error sintáctico");
    }
    cout << "Parseo exitoso" << endl;
    return programa;
}

Stm* Parser::parseStm() {
    Stm* stm;
    PType* e;
    string nombre;
    if(match(Token::PRINT)) {
        match(Token::LPAREN);
        e = parseCE();
        match(Token::RPAREN);
        return new PrintStm(e);
    }
    else if(match(Token::ID)) {
        nombre = previous->text;
        match(Token::ASSIGN);
        e = parseCE();
        return new AssignStm(nombre, e);
    }
    return stm;
}

PType* Parser::parseCE() {
    if (check(Token::LKEY)) {
        return parseSE();
    }
    else {
        return parseE();
    }
}

Exp* Parser::parseE() {
    Exp* l = parseT();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS_OP;
        }
        else{
            op = MINUS_OP;
        }
        Exp* r = parseT();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

Exp* Parser::parseT() {
    Exp* l = parseF();
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL){
            op = MUL_OP;
        }
        else{
            op = DIV_OP;
        }
        Exp* r = parseF();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

Exp* Parser::parseF() {
    Exp* e; 
    if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    } 
    else if (match(Token::LPAREN))
    {
        PType* t = parseCE();
        e = dynamic_cast<Exp*>(t);
        if (!e) e = parseE();
        match(Token::RPAREN);
        return e;
    }
    else if (match(Token::SQRT))
    {   
        match(Token::LPAREN);
        PType* t = parseCE();
        e = dynamic_cast<Exp*>(t);
        if (!e) e = parseE();
        match(Token::RPAREN);
        return new SqrtExp(e);
    }
    else if (match(Token::ID))
    {
        return new IdExp(previous->text);
    }
    else {
        throw runtime_error("Error sintáctico");
    }
}

Set* Parser::parseSE() {
    Set* l = parseST();
    while (match(Token::UNION) || match(Token::INTERSECT) || match(Token::DIFFERENCE)) {
        SetOp op;
        if (previous->type == Token::UNION){
            op = UNION_OP;
        }
        else if (previous->type == Token::INTERSECT){
            op = INTERSECT_OP;
        }
        else{
            op = DIFFERENCE_OP;
        }
        Set* r = parseST();
        l = new BinarySetExp(l, r, op);
    }
    return l;
}

Set* Parser::parseST() {
    return parseSF();
}

Set* Parser::parseSF() {
    if (check(Token::LKEY)) {
        return parseS();
    } 
    else if (match(Token::LPAREN))
    {
        Set* s = parseSE();
        match(Token::RPAREN);
        return s;
    }
    else {
        throw runtime_error("Error sintáctico");
    }
}

Set* Parser::parseS() {
    list<Set*> el;
    list<Exp*> ex;
    if (match(Token::LKEY)) {
        if (!check(Token::RKEY)) {
            if (check(Token::LKEY)) {
                el.push_back(parseSE());
            } else {
                PType* ptype = parseCE();
                Exp* exp = dynamic_cast<Exp*>(ptype);
                if (exp) {
                    ex.push_back(exp);
                }
            }
            while (match(Token::COMA)) {
                if (check(Token::LKEY)) {
                    el.push_back(parseSE());
                } else {
                    PType* ptype = parseCE();
                    Exp* exp = dynamic_cast<Exp*>(ptype);
                    if (exp) {
                        ex.push_back(exp);
                    }
                }
            }
        }
        match(Token::RKEY);
        return new SetExp(el, ex);
    } 
    else {
        throw runtime_error("Error sintáctico");
    }
}