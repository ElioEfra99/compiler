%{
    #include <stdio.h>        /* Para entrada y salida */
    #include <stdlib.h>       /* For malloc here and in symbol table */
    #include <string.h>       /* For strcmp in symbol table */
    #include "ST.h"           /* Tabla de Símbolos */
    #include "SM.h"           /* Stack Machine */
    #include "CG.h"           /* Generador de gódigo */
    #define  YYDEBUG 1       
    int  errors;              /* Error Count                                 */

    struct  lbs              /* Etiquetas para datos, if y while               */
    {
        int for_goto;
        int for_jmp_false;
    };
    
    struct lbs * newlblrec()
    {
        return (struct lbs *) malloc(sizeof(struct lbs));
    }

    install ( char *sym_name )
    {
        symrec *s;
        s = getsym (sym_name);
        if (s == 0)
            s = putsym (sym_name);
        else { errors++;
            printf( "%s is already defined\n", sym_name );
        }
    }

    context_check( enum code_ops operation, char *sym_name )
    { symrec *identifier;
        identifier = getsym( sym_name );
        if ( identifier == 0 )
            { errors++;
              printf( "%s", sym_name );
              printf( "%s\n", " is an undeclared identifier"  );
            }
        else gen_code( operation, identifier->offset );
    }
%}
%union semrec  /* Semantic records */
{
    int intval;
    char *id;
    struct lbs *lbs;
}
%start program
%token <intval> NUMBER  /* Entero simple */ 
%token <id> identifier  /* Identificador simple */ 
%token <lbls> IF WHILE  /* parcheo posterior de etiquetas */ 
%token SKIP THEN ELSE FI DO END
%token INTEGER READ WRITE LET IN
%token ASSGNOP
// PRECEDENCIA DE OPERADORES
%left ’-’ ’+’
%left ’*’ ’/’
%right ’^’
// REGLAS GRAMATICALES para la gramática simple
%%
program : LET
            declarations
          IN           { gen_code( DATA, data_location() - 1 );          }
            commands
          END          { gen_code( HALT, 0 ); YYACCEPT;                  }
;
declarations : /* empty */
    | INTEGER id_seq IDENTIFIER ’.’ { install( $3 );                      }
;
id_seq : /* empty */
    | id_seq IDENTIFIER ’,’  { install( $2 );                             }
;
commands : /* empty */
    | commands command ’;’
;
command : SKIP
    | READ IDENTIFIER   { context_check( READ_INT, $2 );                  }
    | WRITE exp         { gen_code( WRITE_INT, 0 );                       }
    | IDENTIFIER ASSGNOP exp { context_check( STORE, $1 );                }
    | IF exp            { $1 = (struct lbs *) newlblrec();
                          $1->for_jmp_false = reserve_loc();              }
      THEN commands     { $1->for_goto = reserve_loc();                   }
      ELSE              { back_patch( $1->for_jmp_false,
                                      JMP_FALSE,
                                      gen_label() );                       }
            commands
      FI                { back_patch( $1->for_goto, GOTO, gen_label() );  }
    | WHILE             { $1 = (struct lbs *) newlblrec();
                          $1->for_goto = gen_label();                     }
            exp            { $1->for_jmp_false = reserve_loc();              }
      DO
        commands
      END               { gen_code( GOTO, $1->for_goto );
                          back_patch( $1->for_jmp_false,
                                      JMP_FALSE,
                                      gen_label() );                       }
;
exp : NUMBER           { gen_code( LD_INT, $1 );                         }
   | IDENTIFIER        { context_check( LD_VAR,  $1 );                   }
   | exp ’<’ exp       { gen_code( LT,   0 );                            }
   | exp ’=’ exp       { gen_code( EQ,   0 );                            }
   | exp ’>’ exp       { gen_code( GT,   0 );                            }
   | exp ’+’ exp       { gen_code( ADD,  0 );                            }
   | exp ’-’ exp       { gen_code( SUB,  0 );                            }
   | exp ’*’ exp       { gen_code( MULT, 0 );                            }
   | exp ’/’ exp       { gen_code( DIV,  0 );                            }
   | exp ’^’ exp       { gen_code( PWR,  0 );                            }
   | ’(’ exp ’)’
;
%%       
// MAIN
main( int argc, char *argv[] )
{ extern FILE *yyin;
  ++argv; --argc;
  yyin = fopen( argv[0], "r" );
  /*yydebug = 1;*/
  errors = 0;
  yyparse ();
  printf ( "Parseo Completado\n" );
  if ( errors == 0 )
  { print_code ();
    fetch_execute_cycle();
  }
}    
yyerror( char *s )             
{
    errors++;
    printf("%s\n", s);
}
