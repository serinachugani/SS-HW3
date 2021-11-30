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
int listIndex; // Used to keep track of index in list.
int error; // Checks if error has been encountered.
int level; // Current level.

void emit(int opname, int level, int mvalue);
void addToSymbolTable(int k, char n[], int v, int l, int a, int m);
void printparseerror(int err_code);
void printsymboltable();
void printassemblycode();
void program(lexeme *list);
void block(lexeme *list);
void constDecl(lexeme *list);
int varDecl(lexeme *list);
void procDecl(lexeme *list);
void statement(lexeme *list);
void condition(lexeme *list);
void expression(lexeme *list);
void term(lexeme *list);
void factor(lexeme *list);
int multipleDeclarationCheck(char n[]);
int findSymbol(char* n, int kind);
void mark();

instruction *parse(lexeme *list, int printTable, int printCode)
{
	code = NULL;
	/*this line is EXTREMELY IMPORTANT, you MUST uncomment it
		when you test your code otherwise IT WILL SEGFAULT in 
		vm.o THIS LINE IS HOW THE VM KNOWS WHERE THE CODE ENDS
		WHEN COPYING IT TO THE PAS*/
	cIndex = 0;
	tIndex = 0;
	listIndex = 0;
	
	
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

void program(lexeme *list)
{
	emit(7, 0, 0);
	addToSymbolTable(3, "main", 0, 0, 0, 0);
	level = -1;
	block(list);
	if (error == 1)
		return;
	if (list[listIndex].type != periodsym)
	{
		printparseerror(1);
		error = 1;
		return;
	}
	emit(9, 0, 3);
	for (int i = 0; i < cIndex; i++)
	{
		if (code[i].opcode == 5)
		{
			code[i].m = table[code[i].m].addr;
		}
	}
	code[0].m = table[0].addr;
}

void block(lexeme *list)
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
	if (error == 1)
		return;
	mark();
	level--;
}

void constDecl(lexeme *list)
{
	if (list[listIndex].type == const)
		do
		{
			listIndex++;
			
			if (list[listIndex].type != identsym)
				error;
			
			symidx = multipleDeclarationCheck(list[listIndex].type);
			
			if (symidx != -1)
				error;
			// save indent name
			listIndex++;
			
			if (list[listIndex].type != assignsym)
				error;
			listIndex++;
			
			if (list[listIndex].type != numbersym)
				error;
			
			addToSymbolTable(1, saved name, number, level, 0, 0);
			listIndex++;
			
		}
	while (list[listIndex].type != commasym)
	{
		if (list[listIndex].type != semicolonsym)
			if (list[listIndex].type == identsym)
				error;
			else
				error;
		listIndex++;
	}
}

int varDecl(lexeme *list)
{
	numVars = 0;
	if (list[index].type == varsym)
		do
		{
			numVars++;
			index++;
			
			if (list[index].type != identsym)
				error;
			
			symidx = multipleDeclarationCheck(list[index].type);
			
			if (symidx != -1)
				error;
			if (level == 0)
				addToSymbolTable(2, ident, 0, level, numVars - 1, 0);
			else
				addToSymbolTable(2, ident, 0, level, numVars + 2, 0);
			index++;
		}
	
	while (list[index].type == commasym)
	{
		if (list[index].type != semicolonsym)
		{
			if (list[index].type == identsym)
				error;
			else
				error;
		}
	
		index;
	}
	
	return numVars;
}

void procDecl(lexeme *list, int level)
{
	while (list[index].type == procsym)	//so im not sure how much of this is supposed to be in the while loop based on the pseudocode so i just put all of it
	{
		index++;
		
		if (list[index].type != identsym)
			error;		//need to go back and fix this
		
		symidx = multipleDeclarationCheck(list[index].type);	//also might need to fix this bc idk how the function works
		
		if (symidx != -1)
			error;
		
		addToSymbolTable(3, indent, 0, level, 0, unmarked)
		index++;
		
		if (list[index].type != semicolonsym)
			error;
		
		index;
		
		block(list, level);
		
		if (list[index].type != semicolonsym)
			error;
		
		index++;
		emit(2, 0, 0);
	}		
}

void statement(lexeme *list)
{
	if (list[listIndex].type == identsym)
	{
		symIdx = findSymbol(list[listIndex].name, 2);
		if (symIdx == -1)
		{
			if (findSymbol(list[listIndex].name, 1) != findSymbol(list[listIndex].name,3))
			{
				error = 1;
				return;
			}
			else
			{
				error = 1;
				return;
			}
		}
		listIndex++;
		if (list[listIndex].type != assignsym)
		{
			error = 1;
			return;
		}
		listIndex++;
		expression(list, level);
		emit STO (L = level - table[symIdx].level, M = table[symIdx].addr);
		return;
	}
	if (list[listIndex].type == beginsym)
	{
		do {
			listIndex++;
			statement(list, level);
		} while (list[listIndex].type == semicolonsym)
		if (list[listIndex].type != endsym)
		{
			if (list[listIndex].type == identsym || list[listIndex].type == beginsym || list[listIndex].type == ifsym || list[listIndex].type == whilesym 
			    || list[listIndex].type == readsym || list[listIndex].type == writesym || list[listIndex].type == callsym)
			{
				error = 1;
				return;
			}
			else
			{
				error = 1;
				return;
			}
		}
		listIndex++;
		return;
	}
	if (list[listIndex].type == ifsym)
	{
		listIndex++;
		condition(list, level);
		jpcIdx = listIndex;
		emit JPC;
		if (list[listIndex].type == elsesym)
		{
			jmpIdx = listIndex;
			emit JMP;
			code[jpcIdx].m = listIndex * 3;
			statement(list, level);
			listIndex++;
			code[jmpIdx].m = listIndex * 3;
		}
		else 
		{
			code[jpcIdx].m = listIndex * 3;
		}
		return;
	}
	if (list[listIndex].type == whilesym)
	{
		listIndex++;
		loopIdx = listIndex;
		condition(list, level);
		if (list[listIndex].type != dosym)
		{
			error = 1;
			return;
		}
		listIndex++;
		jpcIdx = listIndex;
		emit JPC;
		statement(list, level);
		emit JMP M = loopIdx * 3;
		code[jpcIdx].m = listIndex * 3;
		return;
	}
	if (list[listIndex].type == readsym)
	{
		listIndex++;
		if (list[listIndex].type != identsym)
		{
			error = 1;
			return;
		}
		symIdx = findSymbol(list[listIndex].name, 2);
		if (symIdx == -1)
		{
			if (findSymbol(list[listIndex].name, 1) != findSymbol(list[listIndex].name, 3))
			{
				error = 1;
				return;
			}
			else
			{
				error = 1;
				return;
			}
		}
		listIndex++;
		emit READ;
		emit STO (L = level - table[symIdx].level, M = table[symIdx].addr);
		return;
	}
	if (list[listIndex].type == writesym)
	{
		listIndex++;
		expression(list, level);
		emit WRITE;
		return;
	}
	if (list[listIndex].type == callsym)
	{
		listIndex++;
		symIdx = findSymbol(list[listIndex].name, 3);
		if (symIdx == -1)
		{
			if (findSymbol(list[listIndex].name, 1) != findSymbol(list[listIndex].name, 2))
			{
				error = 1;
				return;
			}
			else
			{
				error = 1;
				return;
			}
		}
		listIndex++;
		emit CAL(L = level - table[symIdx].level, symIdx);
	}			    
}

void condition(lexeme *list)
{
	if (list[index].type == oddsym)
	{
		index;
		expression(list, level);
		emit(2, 0, 6);
	}
	else
	{
		expression(list, level);
		if (list[index].type == eqlsym)
		{
			index++;
			expression(list, level);
			emit(2, 0, 8);
		}
		if (list[index].type == neqsym)
		{
			index++;
			expression(list, level);
			emit(2, 0, 9);
		}
		if (list[index].type == lsssym)
		{
			index++;
			expression(list, level);
			emit(2, 0, 10);
		}
		if (list[index].type == leqsym)
		{
			index++;
			expression(list, level);
			emit(2, 0, 11);
		}
		if (list[index].type == gtrsym)
		{
			index++;
			expression(list, level);
			emit(2, 0, 12);
		}
		if (list[index].type == geqsym)
		{
			index++;
			expression(list, level);
			emit(2, 0, 13);
		}
		else
			error;
	}
}

void expression(lexeme *list)
{
	// If token is a subtract symbol
	if (list[listIndex].type == subsym)
	{
		listIndex++;
		//Call term
		term(list, level);
		if (error == 1)
			return;
		// Add NEG instruction to code array
		emit(2, 0, 1);
		
		// While token is an add symbol or substract symbol
		while (list[listIndex].type == addsym || list[listIndex].type == subsym)
		{	
			// If token is an add symbol, go to next token, call term, and add ADD instruction to code array
			if (list[listIndex].type == addsym)
			{
				listIndex++;
				term(list, level);
				if (error == 1)
					return;
				emit(2, 0, 2);
			}
			// Else, go to next token, call term, and add SUB instruction to code array
			else
			{
				listIndex++;
				term(list, level);
				if (error == 1)
					return;
				emit(2, 0, 3);
			}
		}
	}
	else
	{
		// If token is an add symbol, go to next token
		if (list[listIndex].type == addsym)
		{
			listIndex++;
		}
		// Call term
		term(list, level);
		if (error == 1)
			return;
		
		// While token is an add symbol or subtract symbol
		while (list[listIndex].type == addsym || list[listIndex].type == subsym)
		{
			// If token is an add symbol, go to next token, call term, and add ADD instruction to code array
			if (list[listIndex].type == addsym)
			{
				listIndex++;
				term(list, level);
				if (error == 1)
					return;
				emit(2, 0, 2);
			}
			// Else, go to token, call term, and add SUB instruction to code array
			else
			{
				listIndex++;
				term(list, level);
				if (error == 1)
					return;
				emit(2, 0, 3);
			}
		}
		// If token is +, -, *, /, %, (, identifier, number, or odd, print error.
		if (list[listIndex].type == addsym || list[listIndex].type == subsym || list[listIndex].type == multsym || list[listIndex].type == divsym || list[listIndex].type == modsym || list[listIndex].type == lparensym || list[listIndex].type == identsym, list[listIndex].type == numbersym || list[listIndex].type == oddsym)
		{
			printparseerror(17);
			error = 1;
			return;
		}
	}
}

void term(lexeme *list)
{
	// Call factor
	factor(list, level);
	if (error == 1)
		return;
	
	// While token is a multiplication symbol, dividing symbol, or mod symbol
	while (list[listIndex].type == multsym || list[listIndex].type == divsym || list[listIndex].type == modsym)
	{
		// If token is a multiplication symbol, go to next token, call factor, and add MUL to code array
		if (list[listIndex].type == multsym)
		{
			listIndex++;
			factor(list, level);
			if (error == 1)
				return;
			emit(2, 0, 4);
		}
		// If token is a dividing symbol, go to next token, call factor, and add DIV to code array
		else if (list[listIndex].type == divsym)
		{
			listIndex++;
			factor(list, level);
			if (error == 1)
				return;
			emit(2, 0, 5);
		}
		// Else, go to next token, call factor, and add MOD to code array
		else
		{
			listIndex++;
			factor(list, level);
			if (error == 1)
				return;
			emit(2, 0, 7);
		}
	}
}

void factor(lexeme *list)
{
	if (list[listIndex].type == identsym)
	{
		symIdx_var = findSymbol(list[listIndex].type, 2);
		symIdx_const = findSymbol(list[listIndex].type, 1);
		
		if (symIdx_var == -1 && symIdx_const == -1)
			if (findSymbol(list[listIndex].type, 3) != -1)
				error;
			else
				error;
		
		if (symIdx_var == -1)
			emit(6, 0, table[symIdx_const].val);
			
		else if (symIdx_const == -1 || table[symIdx_var].level > table[symIdx_const].level)
			emit(level–table[symIdx_var].level, 0, table[symIdx_var].addr);
			
		else 
			emit(6, 0 , table[symIdx_const].val);
			
		listIndex++;
	}
	
	else if (list[listIndex].type == numbersym)
	{
		// emit LIT
		emit(6, 0, 0);
		listIndex++;
	}
		
     	else if (list[listIndex].type == lparentsym)
     	{
		listIndex++;
		expression(list, level);
		
		if (list[listIndex].type != rparentsym)
			error;
		
		listIndex++;	
	}
     	
     	else
     		error;
		
}

int findSymbol(char *name, int kind)
{
	int symIdx = -1;
	for (int i = 0; i < table.length; i++)
	{
		if (strcmp(table[i].name, name) == 0 && table[i].kind == kind && table[i].mark == 0)
		{
			symIdx = i;
		}
	}
	level = maxLevel;
	return symIdx;
}
	
void mark()
{
	for (int i = table.length; i >= 0; i--)
	{
		if (table[i].mark = 0)
		{
			if (table[i].level < level)
			{
				return;
			}
			if (table[i].level == level)
			{
				table[i].mark = 1;
			}
		}
	}
}

int multipleDeclarationCheck(char n[])
{
	
}
				    


