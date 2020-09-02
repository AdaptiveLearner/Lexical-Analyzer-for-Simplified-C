

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//#define DebugMode 1

#define key_words_states 60
#define key_words_terminals 27
#define type_states 83
#define type_terminals 27
#define operator_states 46
#define operator_terminals 25
#define number_states 7
#define number_terminals 11
#define id_states 2
#define id_terminals 37

enum terminal_list
{
    A = 1,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    Zero,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine
};

enum token_list
{
    Assign = 37,
    Mul,
    Division,
    Mod,
    Plus,
    Minus,
    LT,
    GT,
    Ampersand,
    Hat,
    OR,
    Tilda,
    SurprisedSymbol,
    Dot,
    Qmark,
    Colon,
    SemiColon,
    Comma,
    LeftParentheses,
    RightParentheses,
    LeftBracket,
    RightBracket,
    LeftBrace,
    RightBrace,
    EQ,
    MulEQ,
    DivEQ,
    ModEQ,
    PlusEQ,
    PlusPlus,
    MinusEQ,
    MinusMinus,
    Pointer,
    LShift,
    LShiftEQ,
    LTEQ,
    RShift,
    RShiftEQ,
    GTEQ,
    AmpersandEQ,
    LogAND,
    HatEQ,
    OREQ,
    LogOR,
    NEQ,
    Blank,
    UnderLine,
    Break,
    Case,
    Continue,
    Default,
    Do,
    Else,
    For,
    Goto,
    If,
    Return,
    Switch,
    Sizeof,
    While,
    Character,
    Const,
    Double,
    Enum,
    Extern,
    Float,
    Int,
    Long,
    Short,
    Signed,
    Static,
    Struct,
    Typedef,
    Union,
    Unsigned,
    Void,
    Volatile,
    ID,
    Integer,
    Real,
    Ds
};


char *tok[81] = {"assign", "mul", "division", "mod", "plus", "minus", "lt", "gt", "ampersand", "hat",
       "or", "tilda", "surprisedsymbol", "dot", "qmark", "colon", "semicolon", "comma", "leftparenthesis",
       "rightparenthesis", "leftbracket", "rightbracket", "leftbrace", "rightbrace", "eq", "muleq", "diveq", "modeq", "pluseq",
       "plusplus", "minuseq", "minusminus", "pointer", "lshift", "lshifteq", "lteq", "rshift", "rshifteq", "gteq",
       "ampersandeq", "logand", "hateq", "oreq", "logor", "neq", "blank", "underline", "break", "case",
       "continue", "default", "do", "else", "for", "goto", "if", "return", "switch", "sizeof",
       "while", "character", "const", "double", "enum", "extern", "float", "int", "long", "short",
       "signed", "static", "struct", "typedef", "union", "unsigned", "void", "volatile", "id", "integer",
       "real", "$"};


int *stream;
int **key_words, **types, **operators, **numbers, **ids;

int reservedword[][20] = {{B, R, E, A, K}, {C, A, S, E}, {C, O, N, T, I, N, U, E}, {D, E, F, A, U, L, T}, {D, O}, {E, L, S, E}, {F, O, R}, {G, O, T, O}, {I, F}, {R, E, T, U, R, N}, {S, W, I, T, C, H}, {S, I, Z, E, O, F}, {W, H, I, L, E}};
int typeword[][20] = {{C, H, A, R}, {C, O, N, S, T}, {D, O, U, B, L, E}, {E, N, U, M}, {E, X, T, E, R, N}, {F, L, O, A, T}, {I, N, T}, {L, O, N, G}, {S, H, O, R, T}, {S, I, G, N, E, D}, {S, T, A, T, I, C}, {S, T, R, U, C, T}, {T, Y, P, E, D, E, F}, {U, N, I, O, N}, {U, N, S, I, G, N, E, D}, {V, O, I, D}, {V, O, L, A, T, I, L, E}};

int operator[][20] = {{Assign}, {Assign, Assign}, {Mul}, {Mul, Assign}, {Division}, {Division, Assign}, {Mod}, {Mod, Assign}, {Plus}, {Plus, Assign}, {Plus, Plus}, {Minus}, {Minus, Assign}, {Minus, Minus}, {Minus, GT}, {LT}, {LT, LT}, {LT, LT, Assign}, {LT, Assign}, {GT}, {GT, GT}, {GT, GT, Assign}, {GT, Assign}, {Ampersand}, {Ampersand, Assign}, {Ampersand, Ampersand}, {Hat, Assign}, {OR}, {OR, Assign}, {OR, OR}, {Tilda}, {SurprisedSymbol}, {SurprisedSymbol, Assign}, {Dot}, {Qmark}, {Colon}, {SemiColon}, {Comma}, {LeftParentheses}, {RightParentheses}, {LeftBracket}, {RightBracket}, {LeftBrace}, {RightBrace}};

int key_words_final_state[] = {5, 9, 16, 23, 24, 28, 31, 35, 37, 43, 49, 54, 59};
int type_words_final_state[] = {4, 8, 14, 18, 23, 28, 31, 35, 40, 45, 50, 54, 61, 66, 72, 76, 82};
int operator_final_state[operator_states - 1];

typedef struct token_struct
{
    int type;
    int val;
} Token;

Token *token_stream;
int scout, token_number;

void printStream(int *stream, int len)
{
    printf("\n%d\n", len);
    for (int i = 0; i < len; i++)
        printf("%5d", stream[i]);
    printf("\n");
}

void printReservedWords()
{
    printf("\n\n");
    for (int i = 0; i < key_words_states; i++)
    {
        for (int j = 1; j < key_words_terminals; j++)
            printf("%3d", key_words[i][j]);
        printf("\n");
    }
}

void printTypes()
{
    printf("\n\n");
    for (int i = 0; i < type_states; i++)
    {
        for (int j = 1; j < type_terminals; j++)
            printf("%3d", types[i][j]);
        printf("\n");
    }
}

void printOperators()
{
    printf("\n\n");
    for (int i = 0; i < operator_states; i++)
    {
        for (int j = 1; j < operator_terminals; j++)
            printf("%3d", operators[i][j]);
        printf("\n");
    }
}

void printNumbers()
{
    printf("\n\n");
    for (int i = 0; i < number_states; i++)
    {
        for (int j = 0; j < number_terminals; j++)
            printf("%3d", numbers[i][j]);
        printf("\n");
    }
}

void printIDs()
{
    printf("\n\n");
    for (int i = 0; i < id_states; i++)
    {
        for (int j = 0; j < id_terminals; j++)
            printf("%3d", ids[i][j]);
        printf("\n");
    }
}

void printTokenStream()
{
    printf("\nToken List: \n");
    for (int i = 0; i < token_number; i++)
    {
        printf("%s\n", tok[token_stream[i].type - Assign]);
    }
}

void key_words_automata()
{
    int i, j;
    key_words = (int **)malloc(key_words_states * sizeof(int *));
    for (i = 0; i < key_words_states; i++)
    {
        key_words[i] = (int *)malloc(key_words_terminals * sizeof(int));
        for (j = 0; j < key_words_terminals; j++)
            key_words[i][j] = 0;
    }

    int start_state[] = {1, 6, 17, 25, 29, 32, 36, 38, 44, 55};
    int final_state[] = {5, 9, 16, 23, 24, 28, 31, 35, 37, 43, 49, 54, 59};
    int row = sizeof(reservedword) / sizeof(reservedword[0]);
    int state_cnt = 0, footprint;
#ifdef DebugMode
    printf("keys cnt = %d\n", row);
#endif    

    for (i = 0; i < row; i++)
    {
        footprint = 0;
        for (j = 0; reservedword[i][j] != 0; j++)
        {
            if (j == 0)
            {
                if (key_words[footprint][reservedword[i][j]] == 0)
                {
                    key_words[footprint][reservedword[i][j]] = state_cnt + 1;
                    footprint = state_cnt + 1;
                    state_cnt += 1;
                }
                else
                    footprint = key_words[footprint][reservedword[i][j]];
            }
            else
            {
                if (key_words[footprint][reservedword[i][j]] == 0)
                {
                    key_words[footprint][reservedword[i][j]] = state_cnt + 1;
                    footprint = state_cnt + 1;
                    state_cnt += 1;
                }
                else
                    footprint = key_words[footprint][reservedword[i][j]];
            }
        }
        /*        
        if (i == 11)
        {
            printf("3 cnt = %d\n", state_cnt);
            break;
        }
    */
    }
#ifdef DebugMode    
    printReservedWords();
#endif    
}

void type_automata()
{
    int i, j;
    types = (int **)malloc(type_states * sizeof(int *));
    for (i = 0; i < type_states; i++)
    {
        types[i] = (int *)malloc(type_terminals * sizeof(int));
        for (j = 0; j < type_terminals; j++)
            types[i][j] = 0;
    }

    int start_state[] = {1, 9, 15, 24, 29, 32, 36, 55, 62, 74};
    int final_state[] = {4, 8, 14, 18, 23, 28, 31, 35, 40, 45, 50, 54, 61, 66, 72, 76, 82};
    int row = sizeof(typeword) / sizeof(typeword[0]);
    int state_cnt = 0, footprint;
#ifdef DebugMode
    printf("type cnt = %d\n", row);
#endif    
    for (i = 0; i < row; i++)
    {
        footprint = 0;
        for (j = 0; typeword[i][j] != 0; j++)
        {
            if (j == 0)
            {
                if (types[footprint][typeword[i][j]] == 0)
                {
                    types[footprint][typeword[i][j]] = state_cnt + 1;
                    footprint = state_cnt + 1;
                    state_cnt += 1;
                }
                else
                    footprint = types[footprint][typeword[i][j]];
            }
            else
            {
                if (types[footprint][typeword[i][j]] == 0)
                {
                    types[footprint][typeword[i][j]] = state_cnt + 1;
                    footprint = state_cnt + 1;
                    state_cnt += 1;
                }
                else
                    footprint = types[footprint][typeword[i][j]];
            }
        }
        /*        
        if (i == 11)
        {
            printf("3 cnt = %d\n", state_cnt);
            break;
        }
    */
    }
#ifdef DebugMode
    printTypes();
#endif    
}

void operator_automata()
{
    int i, j;
    operators = (int **)malloc(operator_states * sizeof(int *));
    for (i = 0; i < operator_states; i++)
    {
        operators[i] = (int *)malloc(operator_terminals * sizeof(int));
        for (j = 0; j < operator_terminals; j++)
            operators[i][j] = 0;
    }

    j = 0;
    for (i = 0; i < operator_states; i++)
        if (i != 27)                        // A Hat
            operator_final_state[j++] = i;

    int row = sizeof(operator) / sizeof(operator[0]);
    int state_cnt = 0, footprint;
#ifdef DebugMode
    printf("operators cnt = %d\n", row);
#endif
    for (i = 0; i < row; i++)
    {
        footprint = 0;
        for (j = 0; operator[i][j] != 0; j++)
        {
            if (j == 0)
            {
                if (operators[footprint][operator[i][j]-Nine] == 0)
                {
                    operators[footprint][operator[i][j]-Nine] = state_cnt + 1;
                    footprint = state_cnt + 1;
                    state_cnt += 1;
                }
                else
                    footprint = operators[footprint][operator[i][j]-Nine];
            }
            else
            {
                if (operators[footprint][operator[i][j]-Nine] == 0)
                {
                    operators[footprint][operator[i][j]-Nine] = state_cnt + 1;
                    footprint = state_cnt + 1;
                    state_cnt += 1;
                }
                else
                    footprint = operators[footprint][operator[i][j]-Nine];
            }
        }
    }
#ifdef DebugMode        
    printOperators();
#endif    
}

void number_automata()
{
    int i, j;

    numbers = (int **)malloc(number_states * sizeof(int *));
    for (i = 0; i < number_states; i++)
    {
        numbers[i] = (int *)malloc(number_terminals * sizeof(int));
        for (j = 0; j < number_terminals; j++)
            numbers[i][j] = 0;
    }

    numbers[0][0] = 4;
    for (j = 1; j <= 9; j++)
        numbers[0][j] = 1;

    for (j = 0; j <= 9; j++)
        numbers[1][j] = 1;
    numbers[1][10] = 2;

    for (j = 0; j <= 9; j++)
    {
        numbers[2][j] = 3;
        numbers[3][j] = 3;
    }

    numbers[4][10] = 5;

    for (j = 0; j <= 9; j++)
    {
        numbers[5][j] = 6;
        numbers[6][j] = 6;
    }
#ifdef DebugMode    
    printNumbers();
#endif
}

void id_automata()
{
    int i, j;

    ids = (int **)malloc(id_states * sizeof(int *));
    for (i = 0; i < id_states; i++)
    {
        ids[i] = (int *)malloc(id_terminals * sizeof(int));
        for (j = 0; j < id_terminals; j++)
            ids[i][j] = 0;
    }

    for (j = 0; j <= 26; j++)
        ids[0][j] = 1;
    for (j = 0; j < id_terminals; j++)
        ids[1][j] = 1;
#ifdef DebugMode        
    printIDs();
#endif    
}

int is_termination(int state, int option)
{
    int i, length;
    if (option == 0)
    {
        length = sizeof(key_words_final_state) / sizeof(key_words_final_state[0]);
        for (i = 0; i < length; i++)
        {
            if (key_words_final_state[i] == state)
                return 1;
        }
        return 0;
    }
    else if (option == 1)
    {
        length = sizeof(type_words_final_state) / sizeof(type_words_final_state[0]);
        for (i = 0; i < length; i++)
        {
            if (type_words_final_state[i] == state)
                return 1;
        }
        return 0;
    }
    else if (option == 2)
    {
        length = sizeof(operator_final_state) / sizeof(operator_final_state[0]);
        for (i = 0; i < length; i++)
        {
            if (operator_final_state[i] == state)
                return 1;
        }
        return 0;
    }
}

int run_key_words_automata(int position, int len)
{
    int i, path = 0, newposition = position;
    //printf("Run: %d\n", newposition);
    for (i = position; i < len; i++)
    {
        if (key_words[path][stream[i]] != 0 && stream[i] >= A && stream[i] <= Z)
        {
            path = key_words[path][stream[i]];
#ifdef DebugMode            
            printf("path = %d %d\n", path, stream[i]);
#endif            
        }
        else
        {
            break;
        }
        newposition++;
    }
    if ((is_termination(path, 0) == 1) && !(stream[i] >= A && stream[i] <= Z)) // next character must be blank NOTE!!!
    {
        switch (path)
        {
        case 5:
            token_stream[token_number].type = Break;
            break;
        case 9:
            token_stream[token_number].type = Case;
            break;
        case 16:
            token_stream[token_number].type = Continue;
            break;
        case 23:
            token_stream[token_number].type = Default;
            break;
        case 24:
            token_stream[token_number].type = Do;
            break;
        case 28:
            token_stream[token_number].type = Else;
            break;
        case 31:
            token_stream[token_number].type = For;
            break;
        case 35:
            token_stream[token_number].type = Goto;
            break;
        case 37:
            token_stream[token_number].type = If;
            break;
        case 43:
            token_stream[token_number].type = Return;
            break;
        case 49:
            token_stream[token_number].type = Switch;
            break;
        case 54:
            token_stream[token_number].type = Sizeof;
            break;
        case 59:
            token_stream[token_number].type = While;
            break;

        default:
            break;
        }
        token_number++;
#ifdef DebugMode        
        printf("Key Words : successful!\n");
#endif        
        return newposition;
    }
    else
    {
#ifdef DebugMode        
        printf("fail\n");
#endif        
        return 0;
    }
}

int run_type_words_automata(int position, int len)
{
    int i, path = 0, newposition = position;
    for (i = position; i < len; i++)
    {
        if (types[path][stream[i]] != 0 && stream[i] >= A && stream[i] <= Z)
        {
            path = types[path][stream[i]];
#ifdef DebugMode            
            printf("path = %d %d\n", path, stream[i]);
#endif            
        }
        else
        {
            break;
        }
        newposition++;
    }
    if ((is_termination(path, 1) == 1) && (stream[i] == Blank))
    {
        switch (path)
        {
        case 4:
            token_stream[token_number].type = Character;
            break;
        case 8:
            token_stream[token_number].type = Const;
            break;
        case 14:
            token_stream[token_number].type = Double;
            break;
        case 18:
            token_stream[token_number].type = Enum;
            break;
        case 23:
            token_stream[token_number].type = Extern;
            break;
        case 28:
            token_stream[token_number].type = Float;
            break;
        case 31:
            token_stream[token_number].type = Int;
            break;
        case 35:
            token_stream[token_number].type = Long;
            break;
        case 40:
            token_stream[token_number].type = Short;
            break;
        case 45:
            token_stream[token_number].type = Signed;
            break;
        case 50:
            token_stream[token_number].type = Static;
            break;
        case 54:
            token_stream[token_number].type = Struct;
            break;
        case 61:
            token_stream[token_number].type = Typedef;
            break;
        case 66:
            token_stream[token_number].type = Union;
            break;
        case 72:
            token_stream[token_number].type = Unsigned;
            break;
        case 76:
            token_stream[token_number].type = Void;
            break;
        case 82:
            token_stream[token_number].type = Volatile;
            break;
        default:
            break;
        }
        token_number++;
#ifdef DebugMode        
        printf("Type: successful!\n");
#endif        
        return newposition;
    }
    else
    {
#ifdef DebugMode        
        printf("fail\n");
#endif        
        return 0;
    }
}

int run_operator_automata(int position, int len)
{
    int i, path = 0, newposition = position;
    for (i = position; i < len; i++)
    {
        if (operators[path][stream[i] - Nine] != 0 && stream[i] >= Assign && stream[i] <= RightBrace)
        {
#ifdef DebugMode            
            printf("Here: %d %d\n", stream[i], stream[i] - Nine);
#endif            
            path = operators[path][stream[i] - Nine];
#ifdef DebugMode            
            printf("%d\n", path);
#endif            
        }
        else
        {
            break;
        }
        newposition++;
    }

    if ((is_termination(path, 2) == 1))
    {
        switch (path)
        {
        case 1:
            token_stream[token_number].type = Assign;
            break;
        case 2:
            token_stream[token_number].type = EQ;
            break;
        case 3:
            token_stream[token_number].type = Mul;
            break;
        case 4:
            token_stream[token_number].type = MulEQ;
            break;
        case 5:
            token_stream[token_number].type = Division;
            break;
        case 6:
            token_stream[token_number].type = DivEQ;
            break;
        case 7:
            token_stream[token_number].type = Mod;
            break;
        case 8:
            token_stream[token_number].type = ModEQ;
            break;
        case 9:
            token_stream[token_number].type = Plus;
            break;
        case 10:
            token_stream[token_number].type = PlusEQ;
            break;
        case 11:
            token_stream[token_number].type = PlusPlus;
            break;
        case 12:
            token_stream[token_number].type = Minus;
            break;
        case 13:
            token_stream[token_number].type = MinusEQ;
            break;
        case 14:
            token_stream[token_number].type = MinusMinus;
            break;
        case 15:
            token_stream[token_number].type = Pointer;
            break;
        case 16:
            token_stream[token_number].type = LT;
            break;
        case 17:
            token_stream[token_number].type = LShift;
            break;
        case 18:
            token_stream[token_number].type = LShiftEQ;
            break;
        case 19:
            token_stream[token_number].type = LTEQ;
            break;
        case 20:
            token_stream[token_number].type = GT;
            break;
        case 21:
            token_stream[token_number].type = RShift;
            break;
        case 22:
            token_stream[token_number].type = RShiftEQ;
            break;
        case 23:
            token_stream[token_number].type = GTEQ;
            break;
        case 24:
            token_stream[token_number].type = Ampersand;
            break;
        case 25:
            token_stream[token_number].type = AmpersandEQ;
            break;
        case 26:
            token_stream[token_number].type = LogAND;
            break;
        case 28:
            token_stream[token_number].type = HatEQ;
            break;
        case 29:
            token_stream[token_number].type = OR;
            break;
        case 30:
            token_stream[token_number].type = OREQ;
            break;
        case 31:
            token_stream[token_number].type = LogOR;
            break;
        case 32:
            token_stream[token_number].type = Tilda;
            break;
        case 33:
            token_stream[token_number].type = SurprisedSymbol;
            break;
        case 34:
            token_stream[token_number].type = NEQ;
            break;
        case 35:
            token_stream[token_number].type = Dot;
            break;
        case 36:
            token_stream[token_number].type = Qmark;
            break;
        case 37:
            token_stream[token_number].type = Colon;
            break;
        case 38:
            token_stream[token_number].type = SemiColon;
            break;
        case 39:
            token_stream[token_number].type = Comma;
            break;
        case 40:
            token_stream[token_number].type = LeftParentheses;
            break;
        case 41:
            token_stream[token_number].type = RightParentheses;
            break;
        case 42:
            token_stream[token_number].type = LeftBracket;
            break;
        case 43:
            token_stream[token_number].type = RightBracket;
            break;
        case 44:
            token_stream[token_number].type = LeftBrace;
            break;
        case 45:
            token_stream[token_number].type = RightBrace;
            break;
        default:
            break;
        }
        token_number++;
#ifdef DebugMode        
        printf("Operator: successful!\n");
#endif        
        return newposition;
    }
    else
    {
#ifdef DebugMode        
        printf("fail\n");
#endif        
        return 0;
    }
}

int run_number_automata(int position, int len)
{
    int is_termination_state[] = {0, 1, 0, 1, 1, 0, 1};
    int i, path = 0, flag = 1, newposition = position;
    for (i = position; i < len; i++)
    {
        if ((numbers[path][stream[i] - Zero] != 0) && (stream[i] >= Zero && stream[i] <= Nine))
        {
            path = numbers[path][stream[i] - Zero];
#ifdef DebugMode            
            printf("1 here : %d\n", path);
#endif            
        }
        else if ((numbers[path][10] != 0) && (stream[i] == Dot))
        {
            path = numbers[path][10];
#ifdef DebugMode            
            printf("2 here : %d\n", path);
#endif
        }
        else
        {
            break;
        }
        newposition++;
    }

    if (is_termination_state[path] == 1)
    {
        switch (path)
        {
        case 1:
            token_stream[token_number].type = Integer;
            break;
        case 3:
            token_stream[token_number].type = Real;
            break;
        case 4:
            token_stream[token_number].type = Integer;
            break;
        case 6:
            token_stream[token_number].type = Real;
            break;
        default:
            break;
        }
        token_number++;
#ifdef DebugMode        
        printf("Number: successful!\n");
#endif        
    }
    else
    {
#ifdef DebugMode        
        printf("fail\n");
#endif
    }
    return newposition;
}

int run_id_automata(int position, int len)
{
    int i, path = 0, newposition = position;
    for (i = position; i < len; i++)
    {
        if ((ids[path][stream[i]] != 0) && (stream[i] >= A && stream[i] <= Z))
        {
            path = ids[path][stream[i]];
#ifdef DebugMode             
            printf("1 here : %d\n", path);
#endif            
        }
        else if ((ids[path][stream[i]] != 0) && (stream[i] >= Zero && stream[i] <= Nine))
        {
            path = ids[path][stream[i]];
        
#ifdef DebugMode   
            printf("2 here : %d\n", path);         
#endif             
        }
        else if ((ids[path][stream[i] - UnderLine] != 0) && stream[i] == UnderLine)
        {
            path = ids[path][stream[i] - UnderLine];
#ifdef DebugMode
            printf("3 here : %d\n", path);
#endif            
        }
        else
        {
            break;
        }
        newposition++;
    }
    if (path == 1)
    {
        token_stream[token_number++].type = ID;
#ifdef DebugMode        
        printf("ID: successful!\n");
#endif        
        return newposition;
    }
    else
    {
#ifdef DebugMode        
        printf("fail\n");
#endif        
        return 0;
    }
}

void analysis(int len)
{
    token_stream = (Token *)malloc(len * sizeof(Token));
    scout = 0, token_number = 0;
    int tmp;

    while (1)
    {
        while (stream[scout] == Blank)
        {
            scout++;
        }

        if (stream[scout] == Ds)
        {
            token_stream[token_number++].type = Ds;
            break;
        }
        else
        {
            if (stream[scout] >= Zero && stream[scout] <= Nine)
            {
                scout = run_number_automata(scout, len);
            }

            else if (stream[scout] >= A && stream[scout] <= Z)
            {
                //printf("enter: %d\n", stream[scout]);
                if ((tmp = run_key_words_automata(scout, len)) != 0)
                    scout = tmp;
                //printf("scout = %d %d\n", scout, stream[scout]);
                else if ((tmp = run_type_words_automata(scout, len)) != 0)
                    scout = tmp;
                else if ((tmp = run_id_automata(scout, len)) != 0)
                    scout = tmp;
            }

            else
            {
#ifdef DebugMode                
                printf("enter: %d %d\n", scout, stream[scout]);
#endif                
                scout = run_operator_automata(scout, len);
#ifdef DebugMode                
                printf("scout = %d %d\n", scout, stream[scout]);
#endif                
            }
        }
    }
    printTokenStream(); 
}

static inline void convert_to_number(char *s)
{
    int accept = 1;
    int len = strlen(s);
    stream = (int *)malloc(len * sizeof(int));
#ifdef DebugMode    
    printf("len = %d\n", len);
#endif    
    for (int i = 0; i < len; i++)
    {
        if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'))
        {
            stream[i] = tolower((int)s[i]) - 96;
#ifdef DebugMode            
            printf("Here Alphabet: %d\n", stream[i]);
#endif            
        }
        else if (s[i] >= '0' && s[i] <= '9')
        {
            stream[i] = (int)s[i] - 21;
#ifdef DebugMode            
            printf("Here Digit: %d\n", stream[i]);
#endif            
        }
        else if (s[i] == '=')
            stream[i] = Assign;
        else if (s[i] == '>')
            stream[i] = GT;
        else if (s[i] == '<')
            stream[i] = LT;
        else if (s[i] == '+')
            stream[i] = Plus;
        else if (s[i] == '-')
            stream[i] = Minus;
        else if (s[i] == '*')
            stream[i] = Mul;
        else if (s[i] == '/')
            stream[i] = Division;
        else if (s[i] == '%')
            stream[i] = Mod;
        else if (s[i] == '&')
            stream[i] = Ampersand;
        else if (s[i] == '^')
            stream[i] = Hat;
        else if (s[i] == '|')
            stream[i] = OR;
        else if (s[i] == '~')
            stream[i] = Tilda;
        else if (s[i] == ' ')
            stream[i] = Blank;
        else if (s[i] == '(')
            stream[i] = LeftParentheses;
        else if (s[i] == ')')
            stream[i] = RightParentheses;
        else if (s[i] == '[')
            stream[i] = LeftBracket;
        else if (s[i] == ']')
            stream[i] = RightBracket;
        else if (s[i] == '{')
            stream[i] = LeftBrace;
        else if (s[i] == '}')
            stream[i] = RightBrace;
        else if (s[i] == '.')
            stream[i] = Dot;
        else if (s[i] == '_')
            stream[i] = UnderLine;
        else if (s[i] == '?')
            stream[i] = Qmark;
        else if (s[i] == ':')
            stream[i] = Colon;
        else if (s[i] == ';')
            stream[i] = SemiColon;
        else if (s[i] == '!')
            stream[i] = SurprisedSymbol;
        else if (s[i] == ',')
            stream[i] = Comma;
        else if (s[i] == '$')
            stream[i] = Ds;
        else
        {
            printf("unrecognized character: %c\n", s[i]);
            accept = 0;
            break;
        }
    }

    if (accept == 1)
    {
#ifdef DebugMode        
        printStream(stream, len);
#endif        
        analysis(len);
    }
}

int main()
{
    char *s[] = {"if(a == 0)$", 
                 "i=2*k+3;$", 
                 "double pi=3.1416;$",
                 "for(i=0; i<10; i++) {k++;}$",
                 "i=6; t=2; while(i>=0) {i-=1; t=2*t+1;}$"};

    key_words_automata();
    type_automata();
    operator_automata();

    number_automata();
    id_automata();

    int i = 0;
    while(s[i] != NULL)
    {
        printf("\n******************************************\nInput String [%d]: %s\n", i, s[i]);
        convert_to_number(s[i]);
        i++;
    }

    return 0;
}