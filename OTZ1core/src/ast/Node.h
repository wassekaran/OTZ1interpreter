#pragma once

typedef enum NodeType
{
	NODE_TERNARY_EXP, NODE_BOOL_EXP, NODE_REL_EXP, NODE_BINARY_EXP, NODE_BINARY_TERM, NODE_VALUE_EXP, NODE_ARRAY_EXP,
	NODE_ARRAY_ELEMENT_EXP
};

#define TERNARY_EXP(x) ((TernaryExp*)x)
#define BOOL_EXP(x) ((BoolExp*)x)
#define REL_EXP(x) ((RelExp*)x)
#define BINARY_EXP(x) ((BinaryExp*)x)
#define BINARY_TERM(x) ((BinaryTerm*)x)
#define VALUE_EXP(x) ((ValueExp*)x)
#define ARRAY_EXP(x) ((ArrayExp*)x)
#define ARRAY_ELEMENT_EXP(x) ((ArrayElementExp*)x)

struct Node
{
	NodeType type;
};