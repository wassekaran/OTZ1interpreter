#pragma once

typedef enum NodeType
{
	NODE_TERNARY_EXP, NODE_BOOL_EXP, NODE_REL_EXP, NODE_BINARY_EXP, NODE_BINARY_TERM, NODE_VALUE_EXP, NODE_ARRAY_EXP,
	NODE_ARRAY_ELEMENT_EXP, NODE_VAR_EXP, NODE_FUNCCALL_EXP,
	NODE_FUNCDEF_STMT, NODE_VARDEF_STMT, NODE_VARASS_STMT, NODE_FUNCCALL_STMT, NODE_IF_STMT, NODE_PRINT_STMT, NODE_RETURN_STMT
};

#define TERNARY_EXP(x) ((TernaryExp*)x)
#define BOOL_EXP(x) ((BoolExp*)x)
#define REL_EXP(x) ((RelExp*)x)
#define BINARY_EXP(x) ((BinaryExp*)x)
#define BINARY_TERM(x) ((BinaryTerm*)x)
#define VALUE_EXP(x) ((ValueExp*)x)
#define ARRAY_EXP(x) ((ArrayExp*)x)
#define ARRAY_ELEMENT_EXP(x) ((ArrayElementExp*)x)
#define VAR_EXP(x) ((VarExp*)x)
#define FUNC_CALL_EXP(x) ((FuncCallExp*)x)

#define FUNCDEF_STMT(x) ((FuncDefStmt*)x)
#define VARDEF_STMT(x) ((VarDefStmt*)x)
#define VARASS_STMT(x) ((VarAssStmt*)x)
#define FUNCCALL_STMT(x) ((FuncCallStmt*)x)
#define IF_STMT(x) ((IfStmt*)x)
#define PRINT_STMT(x) ((PrintStmt*)x)
#define RETURN_STMT(x) ((ReturnStmt*)x)

struct Node
{
	NodeType type;
};