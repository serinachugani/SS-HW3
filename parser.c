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
int multipleDeclarationCheck(char *n);
int findSymbol(char* n, int kind);
void mark();

instruction *parse(lexeme *list, int printTable, int printCode)
{
	code = malloc(MAX_CODE_LENGTH * sizeof *code);
  	table = malloc(MAX_SYMBOL_COUNT * sizeof *table);
	cIndex = 0;
	tIndex = 0;
	listIndex = 0;
	error = 0;
 
	program(list);

	if (error == 1) 
		return NULL;
	
	code[cIndex].opcode = -1;
	
	if (printTable == 1)
		printsymboltable();
	
	if (printCode == 1)
		printassemblycode();
	
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
	// Add JMP instruction to code array
	emit(7, 0, 0);
	// Add "main" to symbol table
	addToSymbolTable(3, "main", 0, 0, 0, 0);
	
	level = -1;
	
	block(list);
	
	// Check if error has already been encountered. If not, continue.
	if (error == 1)
		return;
	
	// If token type is not a period, print error
	if (list[listIndex].type != periodsym)
	{
		printparseerror(1);
		error = 1;
		return;
	}
	
	// Add Halt instruction to code array
	emit(9, 0, 3);
	
	// Fix M for Cal instructions
	for (int i = 0; i < cIndex; i++)
	{
		if (code[i].opcode == 5)
		{
			code[i].m = table[code[i].m].addr;
		}
	}
	// Fix M value for JMP instruction
	code[0].m = table[0].addr;
}

void block(lexeme *list)
{
	level++;

	int procedure_idx = tIndex - 1;
	
	// Call each of the declarations.
	constDecl(list);
	
	// Check if error has been encountered. If not, return.
	if (error == 1)
		return;
	
	int x = varDecl(list);
	// Check if error has been encountered. If not, return.
	if (error == 1)
		return;
	
	procDecl(list);
	// Check if error has been encountered. If not, return.
	if (error == 1)
		return;

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
	statement(list);
	// Check if error has been encountered already. If not, continue.
	if (error == 1)
		return;
	mark();
	level--;
}

void constDecl(lexeme *list)
{
	char ident[12] = "";
	int symidx;
	if (list[listIndex].type == constsym)
	{
		do
		{
			listIndex++;
			
			if (list[listIndex].type != identsym)
			{
				printparseerror(2);
				error = 1;
				return;
			}
			
			symidx = multipleDeclarationCheck(list[listIndex].name);
			
			if (symidx != -1)
			{
				printpareseerror(18);
				error = 1;
				return;
			}
			
			// save indent name
			strcpy(ident, list[listIndex].name);
			listIndex++;
			
			if (list[listIndex].type != assignsym)
			{
				printpareseerror(2);
				error = 1;
				return;
			}
			listIndex++;
			
			if (list[listIndex].type != numbersym)
			{
				printpareseerror(2);
				error = 1;
				return;
			}
			
			addToSymbolTable(1, ident, list[listIndex].value, level, 0, 0);
			listIndex++;
			
		} while (list[listIndex].type != commasym);
			
		if (list[listIndex].type != semicolonsym)
			if (list[listIndex].type == identsym)
			{
				printpareseerror(13);
				error = 1;
				return;
			}
			else
			{
				printpareseerror(14);
				error = 1;
				return;
			}
		listIndex++;
	}	
}

int varDecl(lexeme *list)
{
	int numVars = 0;
	int symidx;
	if (list[listIndex].type == varsym)
	{
		do
		{
			numVars++;
			listIndex++;
			
			if (list[listIndex].type != identsym)
			{
				printparseerror(3);
				error = 1;
				return -1;
			}
			
			symidx = multipleDeclarationCheck(list[listIndex].name);
			
			if (symidx != -1)
			{
				printparseerror(18);
				error = 1;
				return -1;
			}
			if (level == 0)
				addToSymbolTable(2, list[listIndex].name, 0, level, numVars - 1, 0);
			else
				addToSymbolTable(2, list[listIndex].name, 0, level, numVars + 2, 0);
			listIndex++;
		} while (list[listIndex].type == commasym);
		if (list[listIndex].type != semicolonsym)
		{
			if (list[listIndex].type == identsym)
			{
				printparseerror(13);
				error = 1;
				return -1;
			}
			else
			{
				printparseerror(14);
				error = 1;
				return -1;
			}
		}
	
		listIndex++;
	}
	return numVars;
}

void procDecl(lexeme *list)
{
	int symidx;
	while (list[listIndex].type == procsym)	
	{
		listIndex++;
		
		if (list[listIndex].type != identsym)
		{
			printparseerror(4);
			error = 1;
			return;
		}
		
		symidx = multipleDeclarationCheck(list[listIndex].name);
		
		if (symidx != -1)
		{
			printparseerror(18);
			error = 1;
			return;
		}
		
		addToSymbolTable(3, list[listIndex].name, 0, level, 0, 0);
		listIndex++;
		
		if (list[listIndex].type != semicolonsym)
		{
			printparseerror(4);
			error = 1;
			return;
		}
		
		listIndex++;
		
		block(list);
		if (error == 1)
			return;
		
		if (list[listIndex].type != semicolonsym)
		{
			printparseerror(14);
			error = 1;
			return;
		}
		
		listIndex++;
		emit(2, 0, 0);
	}		
}

void statement(lexeme *list)
{
	int symIdx, jpcIdx, jmpIdx, loopIdx;
	// if current token is identifier
	if (list[listIndex].type == identsym)
	{
		// find in symbol table
		symIdx = findSymbol(list[listIndex].name, 2);
		
		// if not found
		if (symIdx == -1)
		{
			if (findSymbol(list[listIndex].name, 1) != findSymbol(list[listIndex].name, 3))
			{
				printparseerror(6);
				error = 1;
				return;
			}
			else
			{
				printparseerror(19);
				error = 1;
				return;
			}
		}
		
		// go to next token
		listIndex++;
		
		// if next token after identifier is not assignment, send assignment error
		if (list[listIndex].type != assignsym)
		{
			printparseerror(5);
			error = 1;
			return;
		}
		
		// go to next token
		listIndex++;
		expression(list, level);
		if (error == 1)
			return;
		emit(4, level - table[symIdx].level, table[symIdx].addr);
		return;
	}
	
	// if current token is begin
	if (list[listIndex].type == beginsym)
	{
		do {
			// next token
			listIndex++;
			statement(list);
			if (error == 1)
				return;
		} while (list[listIndex].type == semicolonsym);
			
		// begin is not followed by end
		if (list[listIndex].type != endsym)
		{
			// if identifier, begin, if, while, read, write, or call found instead, send semicolon error
			if (list[listIndex].type == identsym || list[listIndex].type == beginsym || list[listIndex].type == ifsym || list[listIndex].type == whilesym 
			    || list[listIndex].type == readsym || list[listIndex].type == writesym || list[listIndex].type == callsym)
			{
				printparseerror(15);
				error = 1;
				return;
			}
			
			// else, send begin must be followed by end error
			else
			{
				printparseerror(16);
				error = 1;
				return;
			}
		}
		
		// next token
		listIndex++;
		return;
	}
	
	// if current token is if
	if (list[listIndex].type == ifsym)
	{
		// next token
		listIndex++;
		condition(list);
		if (error == 1)
			return;
		jpcIdx = listIndex;
		emit(8,0,0);
		
		// if current token not then, send if must be followed by then error
		if (list[listIndex].type != thensym)
		{
			printparseerror(8);
			error = 1;
			return;
		}
				
		// next token
		listIndex++;
		statement(list,level);
		if (error == 1)
			return;
		// current token is else
		if (list[listIndex].type == elsesym)
		{
			jmpIdx = cIndex;
			emit(7,0,0);
			code[jpcIdx].m = cIndex * 3;
			
			// next token
			listIndex++;
			statement(list);
			if (error == 1)
				return;
			code[jmpIdx].m = cIndex * 3;
		}
		
		// no else statement
		else 
		{
			code[jpcIdx].m = cIndex * 3;
		}
		return;
	}
	
	// if current token is while
	if (list[listIndex].type == whilesym)
	{
		// next token
		listIndex++;
		loopIdx = cIndex;
		condition(list);
		if (error == 1)
			return;
		
		// if current token is not do, send while must be followed by do error
		if (list[listIndex].type != dosym)
		{
			printparseerror(9);
			error = 1;
			return;
		}
		
		// next token
		listIndex++;
		jpcIdx = cIndex;
		emit(8,0,0);
		statement(list);
		if (error == 1)
			return;
		emit(7,0,loopIdx * 3);
		code[jpcIdx].m = cIndex * 3;
		return;
	}
	
	// if current token is read
	if (list[listIndex].type == readsym)
	{
		// next token
		listIndex++;
		
		// if current token is not identifier, send only variables may be assigned to or read error
		if (list[listIndex].type != identsym)
		{
			printparseerror(6);
			error = 1;
			return;
		}
		
		// find symbol
		symIdx = findSymbol(list[listIndex].name, 2);
		
		// if not found
		if (symIdx == -1)
		{
			if (findSymbol(list[listIndex].name, 1) != findSymbol(list[listIndex].name, 3))
			{
				printparseerror(6);
				error = 1;
				return;
			}
			else
			{
				printparseerror(19);
				error = 1;
				return;
			}
		}
		
		// next token
		listIndex++;
		emit(9,0,2);
		emit(4, level - table[symIdx].level, table[symIdx].addr);
		return;
	}
	
	// if current token is write
	if (list[listIndex].type == writesym)
	{
		// next token
		listIndex++;
		expression(list);
		if (error == 1)
			return;
		emit(9,0,1);
		return;
	}
	
	// if current token is call
	if (list[listIndex].type == callsym)
	{
		// next token
		listIndex++;
		
		// find symbol
		symIdx = findSymbol(list[listIndex].name, 3);
		
		// if not found
		if (symIdx == -1)
		{
			if (findSymbol(list[listIndex].name, 1) != findSymbol(list[listIndex].name, 2))
			{
				printparseerror(7);
				error = 1;
				return;
			}
			else
			{
				printparseerror(19);
				error = 1;
				return;
			}
		}
		
		// next token
		listIndex++;
		emit(5, level - table[symIdx].level, table[symIdx].addr);
	}			    
}

void condition(lexeme *list)
{
	if (list[listIndex].type == oddsym)
	{
		listIndex++;
		expression(list);
		if (error == 1)
			return;
		emit(2, 0, 6);
	}
	else
	{
		expression(list);
		if (error == 1)
			return;
		if (list[listIndex].type == eqlsym)
		{
			listIndex++;
			expression(list);
			if (error == 1)
				return;
			emit(2, 0, 8);
		}
		else if (list[listIndex].type == neqsym)
		{
			listIndex++;
			expression(list);
			if (error == 1)
				return;
			emit(2, 0, 9);
		}
		else if (list[listIndex].type == lsssym)
		{
			listIndex++;
			expression(list);
			if (error == 1)
				return;
			emit(2, 0, 10);
		}
		else if (list[listIndex].type == leqsym)
		{
			listIndex++;
			expression(list);
			if (error == 1)
				return;
			emit(2, 0, 11);
		}
		else if (list[listIndex].type == gtrsym)
		{
			listIndex++;
			expression(list);
			if (error == 1)
				return;
			emit(2, 0, 12);
		}
		else if (list[listIndex].type == geqsym)
		{
			listIndex++;
			expression(list);
			if (error == 1)
				return;
			emit(2, 0, 13);
		}
		else
		{
			printparseerror(10);
			error = 1;
			return;
		}
	}
}

void expression(lexeme *list)
{
	// If token is a subtract symbol
	if (list[listIndex].type == subsym)
	{
		listIndex++;
		//Call term
		term(list);
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
				term(list);
				if (error == 1)
					return;
				emit(2, 0, 2);
			}
			// Else, go to next token, call term, and add SUB instruction to code array
			else
			{
				listIndex++;
				term(list);
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
		term(list);
		if (error == 1)
			return;
		
		// While token is an add symbol or subtract symbol
		while (list[listIndex].type == addsym || list[listIndex].type == subsym)
		{
			// If token is an add symbol, go to next token, call term, and add ADD instruction to code array
			if (list[listIndex].type == addsym)
			{
				listIndex++;
				term(list);
				if (error == 1)
					return;
				emit(2, 0, 2);
			}
			// Else, go to token, call term, and add SUB instruction to code array
			else
			{
				listIndex++;
				term(list);
				if (error == 1)
					return;
				emit(2, 0, 3);
			}
		}
		// If token is (, identifier, number, or odd, print error.
		if (list[listIndex].type == lparensym || list[listIndex].type == identsym, list[listIndex].type == numbersym || list[listIndex].type == oddsym)
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
	factor(list);
	if (error == 1)
		return;
	
	// While token is a multiplication symbol, dividing symbol, or mod symbol
	while (list[listIndex].type == multsym || list[listIndex].type == divsym || list[listIndex].type == modsym)
	{
		// If token is a multiplication symbol, go to next token, call factor, and add MUL to code array
		if (list[listIndex].type == multsym)
		{
			listIndex++;
			factor(list);
			if (error == 1)
				return;
			emit(2, 0, 4);
		}
		// If token is a dividing symbol, go to next token, call factor, and add DIV to code array
		else if (list[listIndex].type == divsym)
		{
			listIndex++;
			factor(list);
			if (error == 1)
				return;
			emit(2, 0, 5);
		}
		// Else, go to next token, call factor, and add MOD to code array
		else
		{
			listIndex++;
			factor(list);
			if (error == 1)
				return;
			emit(2, 0, 7);
		}
	}
}

void factor(lexeme *list)
{
	int symIdx_var, symIdx_const;
	if (list[listIndex].type == identsym)
	{
		symIdx_var = findSymbol(list[listIndex].name, 2);
		symIdx_const = findSymbol(list[listIndex].name, 1);
		
		if (symIdx_var == -1 && symIdx_const == -1)
			if (findSymbol(list[listIndex].name, 3) != -1)
			{
				printpareseerror(11);
				error = 1;
				return;
			}
			else
			{
				printpareseerror(19);
				error = 1;
				return;
			}
		
		if (symIdx_var == -1)
			emit(1, 0, table[symIdx_const].val);
			
		else if (symIdx_const == -1 || table[symIdx_var].level > table[symIdx_const].level)
			emit(3, level–table[symIdx_var].level, table[symIdx_var].addr);
			
		else 
			emit(1, 0 , table[symIdx_const].val);
			
		listIndex++;
	}
	
	else if (list[listIndex].type == numbersym)
	{
		// emit LIT
		emit(1, 0, list[listIndex].value);
		listIndex++;
	}
		
     	else if (list[listIndex].type == lparensym)
     	{
		listIndex++;
		expression(list);
		if (error == 1)
			return;
		
		if (list[listIndex].type != rparensym)
		{
			printpareseerror(12);
			error = 1;
			return;
		}
		
		listIndex++;	
	}
     	
     	else
     	{
		printpareseerror(11);
		error = 1;
		return;
	}
		
}

int findSymbol(char *n, int kind)
{
	int symIdx = -1;
	int symLev = -1;
	
	// linear search through symbol table
	for (int i = 0; i < tIndex; i++)
	{
		// if instance has same name, kind, and is unmarked, keep track of that instance
		if (strcmp(table[i].name, n) == 0 && table[i].kind == kind && table[i].mark == 0)
		{
			// maximize level value of a matching instance
			if (table[i].level > symLev)
			{
				symIdx = i;
				symLev = table[i].level;
			}
		}
	}
	
	// return index of symbol
	return symIdx;
}
	
void mark()
{
	// linear pass starting at end of table
	for (int i = tIndex - 1; i >= 0; i--)
	{
		// if element is unmarked
		if (table[i].mark = 0)
		{	
			// marks elements with same level as current level
			if (table[i].level == level)
			{
				table[i].mark = 1;
			}
			
			// if element has level less than current level, stop
			if (table[i].level < level)
			{
				return;
			}
		}
	}
}

int multipleDeclarationCheck(char *n)
{
	// linear search through symbol table for name given
	for (int i = 0; i < tIndex; i++)
	{
		// if instance has same name, is unmarked, and level is equal to current level, return index of instance
		if (strcmp(table[i].name, n) == 0 && table[i].mark == 0)
		{
			if (table[i].level == level)
			{
				return i;
			}
		}
	}
	
	// return -1 if nothing is found
	return -1;
}
				    


