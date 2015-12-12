#include<iostream>
#include<string>
#include "Lexico.h"
#include "Sintactico.h"
#include <cstdlib>

using namespace std;



Lexico::Lexico(string in): input(in), charClass(ERROR), nextChar(' '), lexeme("")
{
    getChar();
}

Lexico::~Lexico()
{
}

void Lexico::setNewInput(string in)
{
    input=in;
    getChar();
}

//clasifica los tokens segun los char, es decir aquellos que no son de expreisones regulares
void Lexico::getChar()
{
    //obtenemos el primer caracter y lo borramos del input
    if(input.size()>0){
        nextChar=input[0];
        input.erase(0,1);
    }
    //si no la cadena est� vac�a, marcamos fin de cadena con $
    else nextChar='$';

    charClass=ERROR;
//verificamos si el char es una letra
    if((nextChar > 64 && nextChar<91) || (nextChar > 96 && nextChar < 123)){
        charClass= LETTER;
    }
    //verificamos si el char es un digito
    if(nextChar > 47 && nextChar<58){
        charClass=DIGIT;
    }

    if(nextChar=='(')charClass=PARENI;
    if(nextChar==')')charClass=PAREND;
    if(nextChar==';')charClass=PCOMA;
    if(nextChar==':')charClass=DPUNTOS;
    if(nextChar=='=')charClass=IGUAL;
    if(nextChar==' ')charClass=SPACE;
    if(nextChar=='_')charClass=GBAJO;
    if(nextChar==',')charClass=COMA;
    if(nextChar=='$')charClass=STOP;
}

void Lexico::addChar(){
    lexeme+=nextChar;
}

//comenzamos el analisi lexico
int Lexico::lex()
{
    lexeme="";
    while(charClass == SPACE) getChar();

    if(charClass==ERROR){addChar(); getChar(); return ERROR;}
    if(charClass==STOP){return STOP;}

    switch(charClass){
        case LETTER:
            addChar();
            getChar();
            while(charClass == LETTER || charClass==DIGIT || charClass==GBAJO){
                addChar();
                getChar();
            }
            if(lexeme=="SELECT")
                return SELECT;
            else if(lexeme=="CREATE")
                return CREATE;
            else if(lexeme=="NEAR_BY")
                return NEAR_BY;
            else if(lexeme=="DELETE")
                return DELETE;
            else if(lexeme=="ALUMNO"||lexeme=="DOCTOR"||lexeme=="MATERIA"||lexeme=="ESPECIALIDAD")
                return TOKEN_NODOTIPO;
            else if(lexeme=="AND"||lexeme=="OR")
                return ANDOR;
            else if(lexeme=="FROM")
                return FROM;
            else if(lexeme=="WHERE")
                return WHERE;
            else
                return TOKEN_NODO;
            break;
        case DIGIT:
            addChar();
            getChar();
            while(charClass== DIGIT){
                addChar();
                getChar();
            }
            return ERROR;
            break;
        case GBAJO:
            addChar();
            getChar();
            return ERROR;
            break;
        case PARENI:
                addChar();
                getChar();
            return PARENI;
            break;
        case PAREND:
                addChar();
                getChar();
            return PAREND;
            break;
        case PCOMA:
            addChar();
            getChar();
            return PCOMA;
            break;
        case COMA:
            addChar();
            getChar();
            return COMA;
            break;
        case DPUNTOS:
            addChar();
            getChar();
            return DPUNTOS;
            break;
    }
}


Sintactico::Sintactico(string in)
{
    LA.setNewInput(in);
    cout<<"input: "<<in<<endl;
    nextToken=LA.lex();
}

Sintactico::~Sintactico()
{
}

void Sintactico::setNewInput(string in)
{
    LA.setNewInput(in);
    cout<<"input: "<<in<<endl;
    nextToken=LA.lex();
}

void Sintactico::syntax()
{
    S();
}

/*
rezamos para que funcione con las siguientes producciones

<S>::=<CREATE>|<DELETE>|<NEARBY>|<SELECT>
<DELETE>::=DELETE PARENI TOKEN_NODO PAREND STOP
<NEARBY>::=NEARBY PARENI TOKEN_NODO PAREND STOP
<CREATE>::=CREATE PARENI TOKEN_NODO DPUNTOS TOKEN_TIPONODO PCOMA [TOKEN_NODO [{COMA TOKEN_NODO}] DPUNTOS TOKEN_TIPONODO]PCOMA [TOKEN_NODO [{COMA TOKEN_NODO}] DPUNTOS TOKEN_TIPONODO]PCOMA [TOKEN_NODO [{COMA TOKEN_NODO}] DPUNTOS TOKEN_TIPONODO]PCOMA PAREND STOP
<SELECT>::=SELECT PARENI TOKEN_NODO [FROM TOKEN_NODO [WHERE TOKEN_NODO IGUAL TOKEN_NODO [AND TOKEN_NODO IGUAL TOKEN_NODO|OR TOKEN_NODO IGUAL TOKEN_NODO] ] ] PAREND STOP

*/
void Sintactico::S()
{
    switch(nextToken){
    case DELETE:
        DELETE_();
        break;
   // case NEAR_BY:
   //     NEAR_BY();
//    break;
    default:
        cout<<"ERROR";
        exit(1);
        break;
    }
}
//<DELETE>::=DELETE PARENI TOKEN_NODO PAREND STOP
void Sintactico::DELETE_(){
    if(nextToken==DELETE){
        nextToken = LA.lex();
        if(nextToken==PARENI){
            nextToken=LA.lex();
            if(nextToken==TOKEN_NODO){
                nextToken=LA.lex();
                if(nextToken==PAREND){
                    nextToken=LA.lex();
                      if(nextToken==STOP) cout <<"Sintaxis correcta\n";
                }
            }
        }
    }
}


int main(){
Lexico LA("SELECT(NEAR_BY;a3b  223  ALUMNOvd3 ghg  g 1234 ALUMNO,DOCTOR");
	int token=SPACE;
	while (token!=STOP) {
		token = LA.lex();
		cout<<"lexeme: \""<<LA.lexeme<<"\"   token: "<<token<<endl;
	}
cout<<endl;



Sintactico SA("DELETE(jalil)");
SA.syntax();

return 0;
}