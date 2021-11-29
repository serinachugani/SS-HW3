#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

#define MAX_CODE_LENGTH 1000
#define MAX_SYMBOL_COUNT 100

instruction *code;
int cIndex;
symbol *table;
int tIndex;
int index; // Used to keep track of index in list.
int error; // Checks if error has been encountered.

void emit(int opname, int level, int mvalue);
void addToSymbolTable(int k, char n[], int v, int l, int a, int m);
void printparseerror(int err_code);
void printsymboltable();
void printassemblycode();
void program(lexeme *list, int level);
void block(lexeme *list, int level);
void constDecl(lexeme *list, int level);
int varDecl(lexeme *list, int level);
void procDecl(lexeme *list, int level);
void statement(lexeme *list, int level);
void condition(lexeme *list, int level);
void expression(lexeme *list, int level);
void term(lexeme *list, int level);
void factor(lexeme *list, int level);

instruction *parse(lexeme *list, int printTable, int printCode)
{
	code = NULL;
	/*this line is EXTREMELY IMPORTANT, you MUST uncomment it
		when you test your code otherwise IT WILL SEGFAULT in 
		vm.o THIS LINE IS HOW THE VM KNOWS WHERE THE CODE ENDS
		WHEN COPYING IT TO THE PAS*/
	cIndex = 0;
	tIndex = 0;
	index = 0;
	
	
	code[cIndex].opcode = -1;
	
	return code;
}


void emit(int opname, int level, int mvalue)
{
	code[cIndex].opcode = opname;
	code[cIndex].l = level;
	code[cIndex].m = mvalue;
	cIndex++;
}

void addToSymbolTable(int k, char n[], int v, int l, int a, int m)
{
	table[tIndex].kind = k;
	strcpy(table[tIndex].name, n);
	table[tIndex].val = v;
	table[tIndex].level = l;
	table[tIndex].addr = a;
	table[tIndex].mark = m;
	tIndex++;
}


void printparseerror(int err_code)
{
	switch (err_code)
	{
		case 1:
			printf("Parser Error: Program must be closed by a period\n");
			break;
		case 2:
			printf("Parser Error: Constant declarations should follow the pattern 'ident := number {, ident := number}'\n");
			break;
		case 3:
			printf("Parser Error: Variable declarations should follow the pattern 'ident {, ident}'\n");
			break;
		case 4:
			printf("Parser Error: Procedure declarations should follow the pattern 'ident ;'\n");
			break;
		case 5:
			printf("Parser Error: Variables must be assigned using :=\n");
			break;
		case 6:
			printf("Parser Error: Only variables may be assigned to or read\n");
			break;
		case 7:
			printf("Parser Error: call must be followed by a procedure identifier\n");
			break;
		case 8:
			printf("Parser Error: if must be followed by then\n");
			break;
		case 9:
			printf("Parser Error: while must be followed by do\n");
			break;
		case 10:
			printf("Parser Error: Relational operator missing from condition\n");
			break;
		case 11:
			printf("Parser Error: Arithmetic expressions may only contain arithmetic operators, numbers, parentheses, constants, and variables\n");
			break;
		case 12:
			printf("Parser Error: ( must be followed by )\n");
			break;
		case 13:
			printf("Parser Error: Multiple symbols in variable and constant declarations must be separated by commas\n");
			break;
		case 14:
			printf("Parser Error: Symbol declarations should close with a semicolon\n");
			break;
		case 15:
			printf("Parser Error: Statements within begin-end must be separated by a semicolon\n");
			break;
		case 16:
			printf("Parser Error: begin must be followed by end\n");
			break;
		case 17:
			printf("Parser Error: Bad arithmetic\n");
			break;
		case 18:
			printf("Parser Error: Confliciting symbol declarations\n");
			break;
		case 19:
			printf("Parser Error: Undeclared identifier\n");
			break;
		default:
			printf("Implementation Error: unrecognized error code\n");
			break;
	}
	
	free(code);
	free(table);
}

void printsymboltable()
{
	int i;
	printf("Symbol Table:\n");
	printf("Kind | Name        | Value | Level | Address | Mark\n");
	printf("---------------------------------------------------\n");
	for (i = 0; i < tIndex; i++)
		printf("%4d | %11s | %5d | %5d | %5d | %5d\n", table[i].kind, table[i].name, table[i].val, table[i].level, table[i].addr, table[i].mark); 
	
	free(table);
	table = NULL;
}

void printassemblycode()
{
	int i;
	printf("Line\tOP Code\tOP Name\tL\tM\n");
	for (i = 0; i < cIndex; i++)
	{
		printf("%d\t", i);
		printf("%d\t", code[i].opcode);
		switch (code[i].opcode)
		{
			case 1:
				printf("LIT\t");
				break;
			case 2:
				switch (code[i].m)
				{
					case 0:
						printf("RTN\t");
						break;
					case 1:
						printf("NEG\t");
						break;
					case 2:
						printf("ADD\t");
						break;
					case 3:
						printf("SUB\t");
						break;
					case 4:
						printf("MUL\t");
						break;
					case 5:
						printf("DIV\t");
						break;
					case 6:
						printf("ODD\t");
						break;
					case 7:
						printf("MOD\t");
						break;
					case 8:
						printf("EQL\t");
						break;
					case 9:
						printf("NEQ\t");
						break;
					case 10:
						printf("LSS\t");
						break;
					case 11:
						printf("LEQ\t");
						break;
					case 12:
						printf("GTR\t");
						break;
					case 13:
						printf("GEQ\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			case 3:
				printf("LOD\t");
				break;
			case 4:
				printf("STO\t");
				break;
			case 5:
				printf("CAL\t");
				break;
			case 6:
				printf("INC\t");
				break;
			case 7:
				printf("JMP\t");
				break;
			case 8:
				printf("JPC\t");
				break;
			case 9:
				switch (code[i].m)
				{
					case 1:
						printf("WRT\t");
						break;
					case 2:
						printf("RED\t");
						break;
					case 3:
						printf("HAL\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			default:
				printf("err\t");
				break;
		}
		printf("%d\t%d\n", code[i].l, code[i].m);
	}
	if (table != NULL)
		free(table);
}

void program(lexeme *list, int level)
{
	
}

void block(lexeme *list, int level)
{
	level++;

	int procedure_idx = tIndex - 1;
	
	// Call each of the declarations.
	constDecl(list, level);
	
	int x = varDecl(list, level);
	
	procDecl(list, level);

	table[procedure_idx].addr = cIndex * 3;
	
	// If level is 0, add INC instruction to code array with M = x
	if (level == 0)
	{
		emit(6, 0, x);
	}
	// Else, add INC instruction to code array with M = x + 3
	else
	{
		emit(6, 0, x + 3);
	}
	
	// Call statement
	statement(list, level);
	MARK; //FIXXXXX
	level--;
}

void constDecl(lexeme *list, int level)
{
	
}

int varDecl(lexeme *list, int level)
{
	
}

void procDecl(lexeme *list, int level)
{
	
}

void statement(lexeme *list, int level)
{
	if (token == identsym)
	{
		symIdx = findSymbol(token, 2);
		if (symIdx == -1)
		{
			if (findSymbol(token, 1) != findSymbol(token,3))
			{
				error
			}
			else
			{
				error
			}
		}
		get next token
		if (token != assignsym)
			error
		get next token
		expression
		emit STO (L = level - table[symIdx].level, M = table[symIdx].addr)
		return;
	}
	if (token == beginsym)
	{
		do {
			get next token
			statement
		} while (token == semicolonsym)
		if (token != endsym)
		{
			if (token == identsym, beginsym, ifsym, whilesym, readsym, writesym, callsym)
				error
			else
				error
		}
		get next token
		return;
	}
	if (token == ifsym)
	{
		get next token
		condition
		jpcIdx = current code index;
		emit JPC;
		if (token == elsesym)
		{
			jmpIdx = current code index;
			emit JMP;
			code[jpcIdx].m = current code index * 3;
			statement
			code[jmpIdx].m = current code index * 3;
		}
		else 
		{
			code[jpcIdx].m = current code index * 3;
		}
		return;
	}
	if (token == whilesym)
	{
		get next token
		loopIdx = current code index;
		condition
		if (token != dosym)
			error
		get next token
		jpcIdx = current code index
		emit JPC
		statement
		emit JMP M = loopIdx * 3;
		code[jpcIdx].m = current code index * 3;
		return;
	}
	if (token == readsym)
	{
		get next token
		if (token != identsym)
			error
		symIdx = findSymbol(token, 2);
		if (symIdx == -1)
		{
			if (findSymbol(token, 1) != findSymbol(token, 3)
			    error
			else
			    error
		}
		get next token
		emit READ
		emit STO (L = level - table[symIdx].level, M = table[symIdx].addr)
		return;
	}
	if (token == writesym)
	{
		get next token
		expression
		emit WRITE
		return;
	}
	if (token == callsym)
	{
		get next token
		symIdx = findSymbol(token, 3);
		if (symIdx == -1)
		{
			if (findSymbol(token,1) != findSymbol(token,2)
			    error
			else
			    error
		}
		get next token
		emit CAL(L = level - table[symIdx].level, table[symIdx].addr)
	}			    
}

void condition(lexeme *list, int level)
{
	
}

void expression(lexeme *list, int level)
{
	// If token is a subtract symbol
	if (list[index].type == subsym)
	{
		index++;
		//Call term
		term(list, level);
		// Add NEG instruction to code array
		emit(2, 0, 1);
		
		// While token is an add symbol or substract symbol
		while (list[index].type == addsym || list[index].type == subsym)
		{	
			// If token is an add symbol, go to next token, call term, and add ADD instruction to code array
			if (list[index].type == addsym)
			{
				index++;
				term(list, level);
				emit(2, 0, 2);
			}
			// Else, go to next token, call term, and add SUB instruction to code array
			else
			{
				index++;
				term(list, level);
				emit(2, 0, 3);
			}
		}
	}
	else
	{
		// If token is an add symbol, go to next token
		if (list[index].type == addsym)
		{
			index++;
		}
		// Call term
		term(list, level);
		
		// While token is an add symbol or subtract symbol
		while (list[index].type == addsym || list[index].type == subsym)
		{
			// If token is an add symbol, go to next token, call term, and add ADD instruction to code array
			if (list[index].type == addsym)
			{
				index++;
				term(list, level);
				emit(2, 0, 2);
			}
			// Else, go to token, call term, and add SUB instruction to code array
			else
			{
				index++;
				term(list, level);
				emit(2, 0, 3);
			}
		}
		// If token is (, identifier, number, or odd, print error.
		if (list[index].type == lparensym || list[index].type == identsym, list[index].type == numbersym || list[index].type == oddsym)
		{
			printparseerror(17);
		}
	}
}

void term(lexeme *list, int level)
{
	
}

void factor(lexeme *list, int level)
{
	// Call factor
	factor(list, level);
	
	// While token is a multiplication symbol, dividing symbol, or mod symbol
	while (list[index].type == multsym || list[index].type == divsym || list[index].type == modsym)
	{
		// If token is a multiplication symbol, go to next token, call factor, and add MUL to code array
		if (list[index].type == multsym)
		{
			index++;
			factor(list, level);
			emit(2, 0, 4);
		}
		// If token is a dividing symbol, go to next token, call factor, and add DIV to code array
		else if (list[index].type == divsym)
		{
			index++;
			factor(list, level);
			emit(2, 0, 5);
		}
		// Else, go to next token, call factor, and add MOD to code array
		else
		{
			index++;
			factor(list, level);
			emit(2, 0, 7);
		}
	}
}


