%{

#include "1505018.h"

int yyparse(void);
int yylex(void);
extern FILE *yyin;
FILE *logoutfile, *errfile, *asmfile;
extern int linenoc;
int errs=0, warns=0, argid=0;
int funcdef=0;
int tempvarcount=0, labelcount=0;

void yyerror(const char *s)
{
	printf("%s\n",s);
}

string curstatementtype, startcode;
vector<string> arglist, arglist2;
vector<string> callparamlist;
vector<SymbolInfo*> paramlist;
vector<string> arrays, variables, arrayswithsize;
bool funcok=false;

int power(int base,int exp)
{
    ll power;
    power=1;
    while(exp-- > 0)
	{
		power *= base;
		if(power>(1ll<<31)) return 27;
	}
    return power;
}

ll myatoi(string s)
{
	int i=0, sz=s.size();
	ll num=0;
	if(s[0]=='-') i=1;
	ffr(i,i,sz)
	{
		if(s[i]=='E') break;
		num*=10;
		num+=(s[i]-'0');
	}
	if(i==sz)
	{
		if(num<(1ll<<31))
		{
			if(s[0]=='-') return -num; else return num;
		}
		else return 1e10;
	}
	else
	{
		int p;
		i++; if(s[i]=='+') i++;
		p=s[i]-'0';
		if(i+1<sz) return 1e10;
		else
		num*=power(10,p);
		if(num<(1ll<<31))
		{
			if(s[0]=='-') return -num; else return num;
		}
		else return 1e10;
	}
}

double myatof(string s)
{
    double val,pow, val2;
    int sign,i=0,esign,exp, sz=s.size();
    sign=(s[i]=='-')?-1:1;

    if(s[i]=='+' || s[i] == '-')
        i++;
    for(val=0.0;i<sz;i++)
    {
        if(s[i]=='.') break;
        if(s[i]=='e' || s[i]=='E') break;
        val = 10.0 * val + (s[i] - '0');
    }
    if(s[i]=='.')
        i++;
    for(val2=0.0,pow=1.0;i<sz;i++)
    {
        val2 = 10.0 * val2 + (s[i] - '0');
        pow *= 10.0;
    }
	val2/=pow;
    if(s[i]=='e' || s[i] =='E')
        i++;
    if(s[i]=='+' || s[i] =='-')
    {
        esign=s[i];
        i++;
    }
	else esign='+';
    for(exp=0;isdigit(s[i]);i++)
        exp=10.0 * exp + (s[i] - '0');
    if( esign == '-')
        return sign * (val + val2) / power(10,exp);
    else
	{
		double test=(~(1ll<<63))/(val+val2);
		if(power(10,exp)==27 || power(10,exp)>test) return -PI;
		else return sign * (val + val2) * power(10,exp);
	}
}

SymbolTable symtable(1000);

void onlylogprint(string msg)
{
	fpf(logoutfile, "%s", msg.c_str());
}

void ruleprint(string rulemsg)
{
	//fpf(logoutfile, "Line %d: %s", linenoc, rulemsg.c_str());
}

void errprint(string errmsg)
{
	fpf(logoutfile, "Error at line %d: %s\n", linenoc, errmsg.c_str());

	errs++;
}
void warningprint(string warnmsg)
{
	fpf(errfile, "Warning at line %d: %s\n", linenoc, warnmsg.c_str());
	warns++;
}

void Ccodeprint(string msg)
{
	//fpf(logoutfile, "%s\n\n", msg.c_str());
}

void asmcodeprint(string msg)
{
	fpf(asmfile, "%s", msg.c_str());
}


ostringstream ss;
string rett;
string tostring(int a)
{
	ss.str(""); ss.clear();
	ss<< a;
	rett=ss.str();
	return rett;
}
string labelmaker()
{
	return "L"+tostring(labelcount++);
}

string tempmaker()
{
	return "T"+tostring(tempvarcount++);
}

%}

%union{
SymbolInfo* sym;
}

%error-verbose
%token  INCOP DECOP COMMA SEMICOLON LPAREN RPAREN LTHIRD RTHIRD LCURL RCURL INT FLOAT VOID RETURN PRINTLN NOT ASSIGNOP IF ELSE WHILE FOR CASE DEFAULT CONTINUE DO SWITCH CHAR BREAK DOUBLE
%token <sym> ID
%token <sym> CONST_INT
%token <sym> CONST_FLOAT
%token <sym> CONST_CHAR
%token <sym> ADDOP
%token <sym> MULOP
%token <sym> BITOP
%token <sym> LOGICOP
%token <sym> RELOP

%type <sym> type_specifier expression logic_expression rel_expression simple_expression term unary_expression factor variable program unit var_declaration func_declaration func_definition parameter_list compound_statement declaration_list statements statement expression_statement argument_list arguments
%nonassoc THEN
%nonassoc ELSE

%%

start : program {ruleprint("start -> program\n");
//symtable.PrintAllScopeTable(logoutfile);
//cout << $1->show << endl;
onlylogprint("Total "+tostring(linenoc)+" lines: "+tostring(warns)+" warnings, "+tostring(errs)+" errors\n");
if(errs==0)
{
	string ans="", heretemp=tempmaker(), retaddd=tempmaker();
	variables.pb(heretemp);
	variables.pb(retaddd);
	int asmv, siz;
	siz=variables.size();
	asmcodeprint(".model small\n.stack 256\n\n.data\n\n");
	ffr(asmv,0,siz)
	{
		asmcodeprint(variables[asmv]+"\t\tdw\t\t0\n");
	}
	siz=arrays.size();
	ffr(asmv,0,siz)
	{
		asmcodeprint(arrays[asmv]+"\t\tdw\t\t"+arrayswithsize[asmv]+"\t\tdup\t\t(0)\n");
	}
	asmcodeprint(".code\n");

	ans+="\n\ndecimal_print proc\n\tpop "+retaddd+"\n\tpop "+heretemp+"\n\tpush ax\n\tpush bx\n\tpush cx\n\tpush dx\n\tpush sp\n\tpush bp\n\tpush si\n\tpush di\n";
	ans+="\tpushf\n\tcmp "+heretemp+", 0\n\tjge not_neg3\n\tmov ah, 2\n\tmov dl, \'-\'\n\tint 33\n\tneg "+heretemp+"\nnot_neg3:\n\tcmp "+heretemp+", 0\n\tjne not_z\n";
	ans+="\tmov ah, 2\n\tmov dl, '0'\n\tint 33\nnot_z:\n\tmov ax, '#'\n\tpush ax\nout_l1:\n\tcmp "+heretemp+" , 0\n";
	ans+="\tje end_out_l1\n\tmov bx, 10\n\tmov dx, 0\n\tmov ax, "+heretemp+"\n\tidiv bx\n\tpush dx\n\tmov "+heretemp+" , ax\n\tjmp out_l1\n";
	ans+="end_out_l1:\nout_l2:\n\tpop bx\n\tcmp bx, '#'\n\tje end_out_l2\n\tadd bx, '0'\n\tmov ah, 2\n";
	ans+="\tmov dl, bl\n\tint 33\n\tjmp out_l2\nend_out_l2:\n\tmov ah, 2\n\tmov dl, 13\n\tint 33\n\tmov dl, 10\n";
	ans+="\tint 33\n\n\tpopf\n\tpop di\n\tpop si\n\tpop bp\n\tpop sp\n\tpop dx\n\tpop cx\n\tpop bx\n\tpop ax\n\tpush "+retaddd+"\n\tret \ndecimal_print endp\n";

	asmcodeprint(ans);
	asmcodeprint("\n\n"+$1->asmcode);
	asmcodeprint("\nend main\n");
}
else
{
	cout << "Code contains error. Could not be compiled\n";
}


Ccodeprint($1->show);
}
	;

program : program unit {ruleprint("program -> program unit\n");  $$=new SymbolInfo(); $$->show=$1->show+"\n"+$2->show; Ccodeprint($$->show); $$->asmcode=$1->asmcode+$2->asmcode;}
	| unit {ruleprint("program -> unit\n");  $$=$1; Ccodeprint($$->show);}
	;

unit : var_declaration {ruleprint("unit -> var_declaration\n"); $$=$1; Ccodeprint($$->show);}
     | func_declaration {ruleprint("unit -> var_declaration\n"); $$=$1; Ccodeprint($$->show);}
     | func_definition {ruleprint("unit -> func_definition\n"); $$=$1; Ccodeprint($$->show);}
     ;

func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON
		{
			ruleprint("func_declaration -> type_specifier ID (parameter_list);\n");
			SymbolInfo* temp=symtable.Lookup($2->getName(),"FUNC");
			SymbolInfo* temp2=symtable.Lookup($2->getName(),"VAR");
			SymbolInfo* temp3=symtable.Lookup($2->getName(),"ARA");
			if(temp!=NULL) errprint("Function "+$2->getName()+" is already declared\n");
			else if(temp2!=NULL) errprint("A variable named "+$2->getName()+" exists\n");
			else if(temp3!=NULL) errprint("An array named "+$2->getName()+" exists\n");
			else
			{
				temp=new SymbolInfo($2->getName(),"ID");
				temp->setIDtype("FUNC");
				temp->setRettype($1->getDatatype());
				int i, sz=arglist.size();
				ffr(i,0,sz) temp->funcparamlist.pb(arglist[i]);
				arglist.clear();
				symtable.Insert(temp);
			}
			 $$=new SymbolInfo(); $$->show=$1->show+$2->show+"("+$4->show+");\n";
			 Ccodeprint($$->show);
		}
		| type_specifier ID LPAREN RPAREN SEMICOLON
		{
			ruleprint("func_declaration -> type_specifier ID ();\n");
			SymbolInfo* temp=symtable.Lookup($2->getName(),"FUNC");
			SymbolInfo* temp2=symtable.Lookup($2->getName(),"VAR");
			SymbolInfo* temp3=symtable.Lookup($2->getName(),"ARA");
			if(temp!=NULL) errprint("Function "+$2->getName()+" is already declared\n");
			else if(temp2!=NULL) errprint("A variable named "+$2->getName()+" exists\n");
			else if(temp3!=NULL) errprint("An array named "+$2->getName()+" exists\n");
			else
			{
				temp=new SymbolInfo($2->getName(),"ID");
				temp->setIDtype("FUNC");
				temp->setRettype($1->getDatatype());

				symtable.Insert(temp);
			}
			 $$=new SymbolInfo(); $$->show=$1->show+$2->show+"();\n";
			 Ccodeprint($$->show);
		}
		;

func_definition : type_specifier ID LPAREN parameter_list RPAREN
		{
			ruleprint("func_definition -> type_specifier ID (parameter_list) compound_statement\n");
			if(arglist.size()!=argid)
			{
				errprint("Function parameter missing\n");
				arglist.clear(); argid=0;
			}
			SymbolInfo *temp = symtable.Lookup($2->getName(), "FUNC");
			SymbolInfo* temp2=symtable.Lookup($2->getName(),"VAR");
			SymbolInfo* temp3=symtable.Lookup($2->getName(),"ARA");
			if(temp!=NULL)
			{
				if(temp->getdef())
				{
					errprint("Function "+temp->getName()+" is already defined\n");
					arglist.clear(); argid=0;
				}
				else if(temp->getRettype()!=$1->getDatatype())
				{
					errprint("Function "+temp->getName()+" has return type mismatch\n");
					arglist.clear(); argid=0;
				}
				else if(temp->funcparamlist.size()!=arglist.size())
				{
					errprint("Function "+temp->getName()+" has insufficient or too many parameter\n");
					arglist.clear(); argid=0;
				}
				else
				{
					int i, sz=arglist.size();
					ffr(i,0,sz)
					{
						if(temp->funcparamlist[i]!=arglist[i])
						{
							errprint("Function "+temp->getName()+" has argument datatype mismatch\n");
							arglist.clear(); argid=0;
						}
					}
				}
			}
			else
			{
				if(temp2!=NULL) errprint("A variable named "+$2->getName()+" exists\n");
				else if(temp3!=NULL) errprint("An array named "+$2->getName()+" exists\n");
				else {
					temp=new SymbolInfo($2->getName(),"ID");
					temp->setRettype($1->getDatatype());
					temp->setIDtype("FUNC");
					temp->setdef();
					int i, sz=arglist.size();
					ffr(i,0,sz)
					temp->funcparamlist.pb(arglist[i]);
					symtable.Insert(temp);
					funcok=true;
				}
			}
			arglist.clear(); argid=0;
		}
		compound_statement
		{
			//cout << $7->asmcode << endl;
			 string finalasm, funcname=$2->getName(), retadd=tempmaker(), popping="";
			 $$=new SymbolInfo();
			 if(funcok==true)
			 {
				 if($2->getName()!="main")
				 {
					 int mot=$2->funcparamlist.size(), mk, scopeno=symtable.cur->id+1; //disaster
					 for(mk=mot-1; mk>=0; mk--)
					 {
						 popping+="\tpop "+arglist2[mk]+tostring(scopeno)+"\n";
					 }
					 finalasm=funcname+" proc\n\tpop "+retadd+"\n"+popping+"\tpush ax\n\tpush bx\n\tpush cx\n\tpush dx\n\tpush sp\n\tpush bp\n\tpush si\n\tpush di\n\tpushf\n\tpush "+retadd+"\n";
					 finalasm+=$7->asmcode+"\tret\n"+funcname+" endp\n\n\n";
				 }
				 else
				 {
					 finalasm="main proc\n\tmov dx, @data\n\tmov ds, dx\n"+$7->asmcode+"\tmov ah, 4ch\n\tint 33\nmain endp\n";
				 }
				 $$->asmcode=finalasm;
				 funcok=false;
		 	}
			 $$->show=$1->show+$2->show+"("+$4->show+")\n"+$7->show;
			 Ccodeprint($$->show);
		}
		| type_specifier ID LPAREN RPAREN
		{
			ruleprint("func_definition -> type_specifier ID () compound_statement\n");
			SymbolInfo *temp = symtable.Lookup($2->getName(), "FUNC");
			SymbolInfo* temp2=symtable.Lookup($2->getName(),"VAR");
			SymbolInfo* temp3=symtable.Lookup($2->getName(),"ARA");
			if(temp!=NULL)
			{
				if(temp->getdef())
				{
					errprint("Function "+temp->getName()+" is already defined\n");
				}
				else if(temp->getRettype()!=$1->getDatatype())
				{
					errprint("Function "+temp->getName()+" has return type mismatch\n");
				}
			}
			else
			{
				if(temp2!=NULL) errprint("A variable named "+$2->getName()+" exists\n");
				else if(temp3!=NULL) errprint("An array named "+$2->getName()+" exists\n");
				else {
					temp=new SymbolInfo($2->getName(),"ID");
					temp->setRettype($1->getDatatype());
					temp->setIDtype("FUNC");
					temp->setdef();
					funcok=true;
					symtable.Insert(temp);
				}

			}
		} compound_statement
		{
			 $$=new SymbolInfo();

			 string finalasm, funcname=$2->getName(), retadd=tempmaker();
			 $$=new SymbolInfo();
			 if(funcok==true)
			 {
				 if($2->getName()!="main")
				 {
					 finalasm=funcname+" proc\n\tpop "+retadd+"\n"+"\tpush ax\n\tpush bx\n\tpush cx\n\tpush dx\n\tpush sp\n\tpush bp\n\tpush si\n\tpush di\n\tpushf\n\tpush "+retadd+"\n";
					 finalasm+=$6->asmcode+"\tret\n"+funcname+" endp\n\n\n";
				 }
				 else
				 {
					 finalasm="main proc\n\tmov dx, @data\n\tmov ds, dx\n"+$6->asmcode+"\tmov ah, 4ch\n\tint 33\nmain endp\n";
				 }
				 $$->asmcode=finalasm;
				 funcok=false;
		 	}
			 $$->show=$1->show+$2->show+"()\n"+$6->show;
			 Ccodeprint($$->show);
		}
 		;


parameter_list  : parameter_list COMMA type_specifier ID
		{
			ruleprint("parameter_list -> parameter_list, type_specifier ID\n");
			arglist.pb(curstatementtype);
			arglist2.pb($4->getName());
			argid++;

			$4->setIDtype("VAR");
			$4->setDatatype(curstatementtype);

			paramlist.pb($4);  $$=new SymbolInfo(); $$->show=$1->show+", "+$3->show+$4->show;
			Ccodeprint($$->show);
		}
		| parameter_list COMMA type_specifier
		{
			ruleprint("parameter_list -> parameter_list, type_specifier\n");
			arglist.pb(curstatementtype);  $$=new SymbolInfo(); $$->show=$1->show+", "+$3->show;
			Ccodeprint($$->show);
		}
 		| type_specifier ID
		{
			ruleprint("parameter_list -> type_specifier ID\n");
			arglist.pb(curstatementtype);
			arglist2.pb($2->getName());
			argid++;
			$2->setDatatype(curstatementtype);
			$2->setIDtype("VAR");
			paramlist.pb($2);  $$=new SymbolInfo(); $$->show=$1->show+$2->show;
			Ccodeprint($$->show);
		}
		| type_specifier
		{
			ruleprint("parameter_list -> type_specifier\n");
			arglist.pb(curstatementtype);  $$=new SymbolInfo(); $$->show=$1->show;
			Ccodeprint($$->show);
		}
 		;

compound_statement : LCURL
		{
			symtable.EnterScope();
			int i, sz=paramlist.size();
			ffr(i,0,sz) symtable.Insert(paramlist[i]);
		}
		statements {
			//symtable.PrintAllScopeTable(logoutfile);
		}
		RCURL {symtable.ExitScope(); paramlist.clear(); ruleprint("compound_statement -> {statements}\n");  $$=new SymbolInfo(); $$->show="{\n"+$3->show+"}\n"; $$->asmcode=$3->asmcode; $$->asmname=$3->asmname; //cout << $3->asmcode << endl;
	}
 		| LCURL RCURL
		{
			ruleprint("compound_statement -> {}\n");  $$=new SymbolInfo(); $$->show="{}\n"; Ccodeprint($$->show); $$->asmcode=""; $$->asmname="";
		}
 		;

var_declaration : type_specifier declaration_list SEMICOLON {ruleprint("var_declaration -> type_specifier declaration_list;\n");  $$=new SymbolInfo(); $$->show=$1->show+$2->show+";\n"; Ccodeprint($$->show); }
		| type_specifier declaration_list error { errprint("; missing\n");}
 		 ;

type_specifier	: INT
		{
			ruleprint("type_specifier -> INT\n");
			SymbolInfo* temp=new SymbolInfo("INT");
			$$=temp;
			curstatementtype="INT";
			 $$->show="int ";
			 Ccodeprint($$->show);
		}
 		| FLOAT
		{
			ruleprint("type_specifier -> FLOAT\n");
			SymbolInfo* temp=new SymbolInfo("FLOAT");
			$$=temp;
			curstatementtype="FLOAT";
			 $$->show="float "; Ccodeprint($$->show);
		}
 		| VOID
		{
			ruleprint("type_specifier -> VOID\n");
			SymbolInfo* temp=new SymbolInfo("VOID");
			$$=temp;
			curstatementtype="VOID";
			 $$->show="void "; Ccodeprint($$->show);
		}
 		;

declaration_list : declaration_list COMMA ID
		{
			 $$=new SymbolInfo(); $$->show=$1->show+", "+$3->show;
			ruleprint("declaration_list -> declaration_list, ID\n"); Ccodeprint($$->show);
			if(curstatementtype=="VOID") {errprint("Variable type can't be void\n"); ;}
			else
			{
				string nname=$3->getName();
				SymbolInfo* temp;
				temp=symtable.Lookup(nname,"VAR");
				if(temp!=NULL) {errprint("Variable "+nname+" is already declared\n"); ;}
				else
				{
					temp=symtable.Lookup(nname,"ARA");
					if(temp!=NULL)
					{errprint(nname+" is already declared as array\n"); ;}
					else
					{
						temp=symtable.Lookup(nname,"FUNC");
						if(temp!=NULL)
						{errprint("A function "+nname+" is already declared in the same scope\n");}
						else
						{
							temp=new SymbolInfo(nname, $3->getType());
							temp->setIDtype("VAR");
							temp->setDatatype(curstatementtype);
							symtable.Insert(temp);
							string unk=$3->show+tostring(symtable.cur->id);
							variables.pb(unk);
						}
					}
				}
			}
		}
 		  | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD
		  {
			   $$=new SymbolInfo(); $$->show=$1->show+", "+$3->show+"["+$5->show+"]";
			  ruleprint("declaration_list -> declaration_list, ID[CONST_INT]\n"); Ccodeprint($$->show);
			  if(curstatementtype=="VOID") {errprint("Array type can't be void\n"); ;}
  			else
  			{
  				string nname=$3->getName();
  				SymbolInfo* temp=symtable.Lookup(nname, "ARA");
  				if(temp!=NULL) {errprint("Array "+nname+" is already declared\n"); ;}
  				else
  				{
					temp=symtable.Lookup(nname,"VAR");
					if(temp!=NULL)
					{errprint(nname+" is already declared as variable\n"); }
					else
					{
						ll size=myatoi($5->getName());
						if(size>1e7) errprint("Too large array, cannot declare\n");
						else
						{
							temp=symtable.Lookup(nname,"FUNC");
							if(temp!=NULL)
							{errprint("A function "+nname+" is already declared in the same scope\n");}
							else
							{
								int i;
			  					temp=new SymbolInfo(nname, $3->getType());
			  					temp->setIDtype("ARA");

								temp->setarrlen((int)size);
			  					temp->setDatatype(curstatementtype);
								string unk=$3->show+tostring(symtable.cur->id);
								arrays.pb(unk);
								arrayswithsize.pb(tostring((int)size));
			  					symtable.Insert(temp);
							}
						}
					}
  				}
  			}
		  }
		  | declaration_list COMMA ID LTHIRD CONST_FLOAT RTHIRD
		  {
			  errprint("Array size cannot be float\n");
			   $$=new SymbolInfo(); $$->show=$1->show+", "+$3->show+"["+$5->show+"]"; Ccodeprint($$->show);
		  }
 		  | ID
		  {
			   $$=new SymbolInfo(); $$->show=$1->show;
			  ruleprint("declaration_list -> ID\n"); Ccodeprint($$->show);
			  if(curstatementtype=="VOID") {errprint("Variable type can't be void\n"); }
			  else
			  {
				    string nname=$1->getName();
    				SymbolInfo* temp=symtable.Lookup(nname, "ARA");
    				if(temp!=NULL) {errprint(nname+" is already declared as array\n"); }
    				else
    				{
	  					temp=symtable.Lookup(nname,"VAR");
	  					if(temp!=NULL)
	  					{errprint("Variable "+nname+" is already declared\n"); }
	  					else
	  					{
							temp=symtable.Lookup(nname,"FUNC");
							if(temp!=NULL)
							{errprint("A function "+nname+" is already declared in the same scope\n");}
							else {
		  	  					temp=new SymbolInfo(nname, "ID");
		  	  					temp->setIDtype("VAR");
		  	  					temp->setDatatype(curstatementtype);
		  	  					symtable.Insert(temp);
								string unk=$1->show+tostring(symtable.cur->id);
								variables.pb(unk);
							}
	  					}
    				}
			  }
		  }

		  | ID LTHIRD expression RTHIRD
		  {
			   $$=new SymbolInfo(); $$->show=$1->show+"["+$3->show+"]";
			  ruleprint("declaration_list -> ID[expression]\n"); Ccodeprint($$->show);
			  if(curstatementtype=="VOID") {errprint("Array type can't be void\n"); }
			  else if($3->getDatatype()!="INT") {errprint("Array size non integer\n");}
			  else
			  {
				    string nname=$1->getName();
    				SymbolInfo* temp=symtable.Lookup(nname, "VAR");
    				if(temp!=NULL) {errprint(nname+" is already declared as variable\n"); }
    				else
    				{
	  					temp=symtable.Lookup(nname,"ARA");
	  					if(temp!=NULL)
	  					{errprint("Array "+nname+" is already declared\n"); }
						else
	  					{
							ll size=myatoi($3->getName());

							if(size>1e7) errprint("Too large array, cannot declare\n");
							else
							{
								temp=symtable.Lookup(nname,"FUNC");
								if(temp!=NULL)
								{errprint("A function "+nname+" is already declared in the same scope\n");}
								else {
									temp=new SymbolInfo(nname, $1->getType());
			  	  					temp->setIDtype("ARA");
			  	  					temp->setDatatype(curstatementtype);
									temp->setarrlen((int)size);
									string unk=$1->show+tostring(symtable.cur->id);
									arrays.pb(unk);
									arrayswithsize.pb(tostring((int)size));
			  	  					symtable.Insert(temp);
								}
							}
	  					}
    				}
			  }
		  }
 		  ;

statements : statement {ruleprint("statements -> statement\n");   $$=new SymbolInfo(); $$->show=$1->show; $$->asmcode=$1->asmcode; Ccodeprint($$->show);   }
	   | statements statement {ruleprint("statements -> statements statement\n");  $$=new SymbolInfo(); $$->show=$1->show+$2->show; $$->asmcode=$1->asmcode+$2->asmcode; Ccodeprint($$->show); //asmcodeprint($$->asmcode+"\n\none gone\n\n");
   }
	   ;

statement : var_declaration {ruleprint("statement -> var_declaration\n");  $$=new SymbolInfo(); $$->show=$1->show; $$->asmcode=$1->asmcode; $$->asmname=$1->asmname; Ccodeprint($$->show);}
	  | expression_statement {ruleprint("statement -> expression_statement\n");  $$=new SymbolInfo(); $$->show=$1->show; Ccodeprint($$->show); $$->asmcode=$1->asmcode; $$->asmname=$1->asmname; //cout << $$->asmcode << endl << "helloworld\n";
  }
	  | compound_statement {ruleprint("statement -> compound_statement\n"); $$=new SymbolInfo(); $$->show=$1->show; Ccodeprint($$->show); $$->asmcode=$1->asmcode; $$->asmname=$1->asmname; }
	  | FOR LPAREN expression_statement expression_statement expression RPAREN statement {ruleprint("statement -> FOR (expression_statement expression_statement expression) statement\n");  $$=new SymbolInfo(); $$->show="for ("+$3->show+$4->show+$5->show+")\n"+$7->show+"\n"; Ccodeprint($$->show);

$$->asmcode=$3->asmcode;
string loopstart=labelmaker(), xit=labelmaker();
$$->asmcode+=loopstart+":\n";
$$->asmcode+=$7->asmcode+$5->asmcode;
$$->asmcode+=$4->asmcode;
if($4->asmname==";")
{
	errprint("Code has infinite loop\n");
}
$$->asmcode+="\tmov ax, "+$4->asmname;
$$->asmcode+="\n\tcmp ax, 0\n\tje "+xit+"\n\tjmp "+loopstart+"\n"+xit+":\n";
$$->asmname="forloop";
//cout << $$->asmcode << endl;

   }
   | FOR LPAREN expression_statement expression_statement RPAREN statement {ruleprint("statement -> FOR (expression_statement expression_statement expression) statement\n");  $$=new SymbolInfo(); $$->show="for ("+$3->show+$4->show+")\n"+$6->show+"\n"; Ccodeprint($$->show);

$$->asmcode=$3->asmcode;
string loopstart=labelmaker(), xit=labelmaker();
$$->asmcode+=loopstart+":\n";
$$->asmcode+=$6->asmcode;
$$->asmcode+=$4->asmcode;
if($4->asmname==";")
{
	errprint("Code has infinite loop\n");
}
$$->asmcode+="\tmov ax, "+$4->asmname;
$$->asmcode+="\n\tcmp ax, 0\n\tje "+xit+"\n\tjmp "+loopstart+"\n"+xit+":\n";
$$->asmname="forloop";
//cout << $$->asmcode << endl;

}
	  | IF LPAREN expression RPAREN statement %prec THEN {ruleprint("statement -> IF (expression) statement\n");  $$=new SymbolInfo(); $$->show="if("+$3->show+")\n"+$5->show+"\n"; Ccodeprint($$->show);
string xit=labelmaker();
$$->asmcode=$3->asmcode+"\mov ax, "+$3->asmname+"\n\tcmp ax, 0\n\tjmp "+xit+"\n"+$5->asmcode+""+xit+":\n";
$$->asmname="if";
   }
	  | IF LPAREN expression RPAREN statement ELSE statement {ruleprint("statement -> IF(expression) statement ELSE statement\n");  $$=new SymbolInfo(); $$->show="if("+$3->show+")\n"+$5->show+"\nelse\n"+$7->show+"\n"; Ccodeprint($$->show);
string iffalse=labelmaker(), xit=labelmaker();
$$->asmcode=$3->asmcode+"\mov ax, "+$3->asmname+"\n\tcmp ax, 0\n\tjmp "+iffalse+"\n"+$5->asmcode+"\tj"+xit+"\n"+iffalse+":\n"+$7->asmcode+""+xit+":\n";
$$->asmname="ifelse";
   }
	  | WHILE LPAREN expression RPAREN statement {ruleprint("WHILE (expression) statement\n");  $$=new SymbolInfo(); $$->show="while ("+$3->show+")\n"+$5->show+"\n"; Ccodeprint($$->show);
string loopstart=labelmaker(), xit=labelmaker();
$$->asmcode=loopstart+":\n";
$$->asmcode+=$3->asmcode;
$$->asmcode+="\tmov ax, "+$3->asmname;
$$->asmcode+="\n\tcmp ax, 0\n\tje "+xit+"\n";
$$->asmcode+=$5->asmcode+"\tjmp "+loopstart+"\n"+xit+":\n";
if($3->asmname==";")
{
	errprint("Code has infinite loop\n");
}
$$->asmname="whileloop";
   }
	  | PRINTLN LPAREN expression RPAREN SEMICOLON {ruleprint("PRINTLN (ID);\n");
	   $$=new SymbolInfo(); $$->show="println("+$3->show+");\n";
	  //SymbolInfo* temp=symtable.Lookup($3->getName(), "VAR");
	  if($3->getDatatype()=="") {errprint("Variable not fount in println\n");}
	  else {
		  $$->asmcode=$3->asmcode+"\tpush "+$3->asmname+"\n\tcall decimal_print\n";
	  }
	  Ccodeprint($$->show);

  }
	  | PRINTLN LPAREN expression RPAREN error {errprint("; missing\n");}
	  | RETURN expression SEMICOLON {ruleprint("statement -> RETURN expression;\n");  $$=new SymbolInfo(); $$->show="return "+$2->show+";\n"; Ccodeprint($$->show);
$$->asmcode=$2->asmcode+"\tmov dx, "+$2->asmname+"\n\tret\n";

   }
	  | RETURN expression error {errprint("; missing\n");}
	  ;

expression_statement 	: SEMICOLON {ruleprint("expression_statement -> SEMICOLON\n");  $$=new SymbolInfo(); $$->show=";\n"; $$->asmcode=""; Ccodeprint($$->show); $$->asmname=";";}
			| expression SEMICOLON {ruleprint("expression_statement -> expression SEMICOLON\n");  $$=new SymbolInfo(); $$->show=$1->show+";\n"; Ccodeprint($$->show); $$->asmcode=$1->asmcode; $$->asmname=$1->asmname; }
			| expression error {errprint("; missing\n");}
			;

variable : ID
		{
			ruleprint("variable -> ID\n");
			SymbolInfo* temp=symtable.Lookupwithparent($1->getName(),"VAR");
			SymbolInfo* niu= new SymbolInfo();
			if(temp==NULL) {errprint($1->getName()+" wasn't declared in this scope\n");}
			else
			{
				SymbolInfo* temp2=new SymbolInfo();
				temp2->copyfrom(temp,temp2);
			   $$=temp2;
			}
			$$->show=$1->show;
			string unk=$1->show+tostring(symtable.cur->id);

			$$->asmname=unk; $$->isarray=false;
			$$->asmcode="";
			Ccodeprint($$->show);
		}
	 | ID LTHIRD expression RTHIRD
	 {
		 ruleprint("variable -> ID [expression]\n");
		 SymbolInfo* temp=symtable.Lookupwithparent($1->getName(),"ARA");
		 if(temp->getDatatype()=="" || $3->getDatatype()=="") {  $$=new SymbolInfo(); $$->show=$1->show+"["+$3->show+"]"; }
		 else {

		 if(temp==NULL && symtable.Lookupwithparent($1->getName(),"VAR")){errprint("Using index on non array\n");  $$=new SymbolInfo(); $$->show=$1->show+"["+$3->show+"]"; }
		 else if(temp==NULL) {errprint($1->getName()+" wasn't declared in this scope\n");  $$=new SymbolInfo(); $$->show=$1->show+"["+$3->show+"]"; }
		 else
		 {
			 if($3->getDatatype()!="INT") {errprint("Array index isn't integer\n");  $$=new SymbolInfo(); $$->show=$1->show+"["+$3->show+"]"; }
			 else if($3->ints[0]>=temp->getarrlen()) {errprint("Array "+temp->getName()+" index out of bound\n");  $$=new SymbolInfo(); $$->show=$1->show+"["+$3->show+"]"; }
			 else
			 {
				 temp->setarrind($3->ints[0]);
				$$=new SymbolInfo();
				$$->copyfrom(temp,$$);
				 $$->show=$1->show+"["+$3->show+"]";
				 $$->asmcode=$3->asmcode+"\tmov bx, "+$3->asmname+"\n\tadd bx, bx\n";
				 $$->asmname=$1->show+tostring(symtable.cur->id);
				 $$->isarray=true;
			 }
		 }
	 	}
		Ccodeprint($$->show);
	 }
	 ;

 expression : logic_expression
 		{
			ruleprint("expression -> logic_expression\n");
			$$=$1;
			Ccodeprint($$->show);
		}
	   | variable ASSIGNOP logic_expression
	   {
		   ruleprint("expression -> variable = logic_expression\n");
		   string dtype=$1->getDatatype();
		   if($3->getDatatype()=="VOID") {
			   errprint("Trying to get value from void function\n");
			   $$=new SymbolInfo();
			   $$->show=$1->show+"="+$3->show;
		   }
		   else if($3->getDatatype()=="" || dtype=="")
		   {
			   $$=new SymbolInfo();
			   $$->show=$1->show+"="+$3->show;
		   }
		   else if(dtype=="INT")
		   {
			   if($1->getIDtype()=="VAR")
			   {
				   if($3->getDatatype()=="FLOAT")
				   {
					   warningprint("Assigning non integer value in an integer\n");
				   }
				   $$=$1;
			   }
			   else if($1->getIDtype()=="FUNC") {errprint("Cannot assign values to a function\n"); $$=new SymbolInfo();}
			   else
			   {
				   if($3->getDatatype()=="FLOAT")
				   {
					   warningprint("Assigning non integer value to an integer\n");
				   }
				   $$=$1;
			   }
			   $$->asmcode=$3->asmcode+$1->asmcode+"\tmov ax, "+$3->asmname+"\n";
			   if($1->isarray)
			   {
				   $$->asmcode+="\tmov "+$1->asmname+" [bx], ax\n";
			   }
			   else
			   {
				   $$->asmcode+="\tmov "+$1->asmname+", ax\n";
			   }
			   string tm=tempmaker(); variables.pb(tm);
			   $$->asmcode+="\tmov "+tm+", ax\n";
			   $$->asmname=tm;

		   }
		   else if(dtype=="FLOAT")
		   {
			   if($1->getIDtype()=="VAR")
			   {
				   if($3->getDatatype()=="INT")
				   {
					   warningprint("Assigning integer value to a float\n");
				   }
				   $$=$1;
			   }
			   else if($1->getIDtype()=="FUNC") {errprint("Cannot assign values to function\n"); $$=new SymbolInfo(); }
			   else
			   {
				   if($3->getDatatype()=="INT")
				   {
					   warningprint("Assigning integer value in a float\n");
				   }
				   $$=$1;
			   }
		   }
		   else
		   {
			   $$=new SymbolInfo();

		   }
		   $$->show=$1->show+"="+$3->show;
		   Ccodeprint($$->show);
	   }
	   ;

logic_expression : rel_expression
		{
			ruleprint("logic_expression -> rel_expression\n"); $$=$1; Ccodeprint($$->show);
		}
		 | rel_expression LOGICOP rel_expression
		 {
			 ruleprint("logic_expression -> rel_expression LOGICOP rel_expression\n");
			 if($1->getDatatype()=="VOID" || $3->getDatatype()=="VOID") {
				 errprint("Trying to get value from void function\n");
				 $$=new SymbolInfo();
  			   $$->show=$1->show+$2->getName()+$3->show;
			 }
			 else if($1->getDatatype()=="" || $3->getDatatype()=="")
			 {
				 $$=new SymbolInfo();
  			   $$->show=$1->show+$2->getName()+$3->show;
			 }
			 else
			 {
			 SymbolInfo* temp=new SymbolInfo("INT");
			 temp->setIDtype("VAR");
			 string lop=$2->getName();
			 string tm=tempmaker(); variables.pb(tm); $$->asmcode=$1->asmcode+$3->asmcode;
			 $$=temp;
			 $$->show=$1->show+" "+lop+" "+$3->show; $$->asmname=tm;
			 if(lop=="&&")
			 {
				 string fail=labelmaker(), xit=labelmaker();
				 $$->asmcode="\tmov ax, "+$1->asmname+"\n\tcmp ax, 0\n\tjz "+fail+"\n\tmov ax, "+$3->asmname+"\n\tcmp ax, 0\n\tjz "+fail+"\n\tmov "+tm+", 1\n\tjmp "+xit+"\n"+fail+":\n\tmov "+tm+", 0\n"+xit+":\n";
			 }
			 else if(lop=="||")
			 {
				 string success=labelmaker(), xit=labelmaker();
				 $$->asmcode="\tmov ax, "+$1->asmname+"\n\tcmp ax, 1\n\tjz "+success+"\n\tmov ax, "+$3->asmname+"\n\tcmp ax, 1\n\tjz "+success+"\n\tmov "+tm+", 0\n\tjmp "+xit+"\n"+success+":\n\tmov "+tm+", 1\n"+xit+":\n";
			 }
		  }
			 Ccodeprint($$->show);
		 }
		 ;

rel_expression	: simple_expression
		{
			ruleprint("rel_expression -> simple_expression\n"); $$=$1; Ccodeprint($$->show);
		}
		| simple_expression RELOP simple_expression
		{
			ruleprint("rel_expression -> simple_expression RELOP simple_expression\n");
			SymbolInfo* temp=new SymbolInfo("INT");
			temp->setIDtype("VAR");
			string relop=$2->getName(), op1=$1->getDatatype(), op2=$3->getDatatype();
			if(op1=="VOID" || op2=="VOID") {
				errprint("Trying to get value from void function\n");
				$$=new SymbolInfo();
			  $$->show=$1->show+relop+$3->show;
			}
			else if(op1=="" || op2=="")
			{
				$$=$2;
				$$=new SymbolInfo();
			  $$->show=$1->show+relop+$3->show;
			}
			else if(op1!=op2) {
				errprint("Operands mismatch on "+relop+" operator\n");
				$$=new SymbolInfo();
			  $$->show=$1->show+relop+$3->show;
			}
			else
			{
				string relst;
				if(relop=="==")
				{
					relst="\tje ";
				}
				else if(relop=="!=")
				{
					relst="\tjne ";
				}
				else if(relop==">=")
				{
					relst="\tjge ";
				}
				else if(relop==">")
				{
					relst="\tjg ";
				}
				else if(relop=="<=")
				{
					relst="\tjle ";
				}
				else if(relop=="<")
				{
					relst="\tjl ";
				}
				$$=temp;
			  $$->show=$1->show+" "+relop+" "+$3->show;
			  string lbtrue=labelmaker(), lbexit=labelmaker();
			  string tm=tempmaker(); variables.pb(tm); $$->asmcode=$1->asmcode+$3->asmcode;
			  $$->asmcode+="\tmov ax, "+$1->asmname+"\n\tcmp ax, "+$3->asmname+"\n"+ relst+lbtrue+"\n\tmov "+tm+", 0\n\tjmp "+lbexit+"\n"+lbtrue+":\n\tmov "+tm+", 1\n"+lbexit+":\n"; $$->asmname=tm;

			}
			Ccodeprint($$->show);
		}
		;

simple_expression : term
		{
			ruleprint("simple_expression -> term\n"); $$=$1; Ccodeprint($$->show);
		}
		  | simple_expression ADDOP term
		  {
			  ruleprint("simple_expression -> simple_expression ADDOP term\n");
			  if($1->getDatatype()=="VOID" || $3->getDatatype()=="VOID") {
				  errprint("Trying to get value from void function\n");
				  $$=new SymbolInfo();
   			   $$->show=$1->show+$2->getName()+$3->show;
			  }
			  else if($1->getDatatype()=="" || $3->getDatatype()=="")
 			 {
				 $$=new SymbolInfo();
  			   $$->show=$1->show+$2->getName()+$3->show;
 			 }
 			 else {
			  string addop=$2->getName();
			  SymbolInfo* temp=new SymbolInfo("INT");
			  temp->setIDtype("VAR");
			  $$=temp;
			$$->show=$1->show+$2->getName()+$3->show;
			if(addop=="+")
			{string tm=tempmaker(); variables.pb(tm); $$->asmcode=$1->asmcode+$3->asmcode+"\tmov ax, "+$1->asmname+"\n\tadd ax, "+$3->asmname+"\n\tmov "+tm+", ax\n"; $$->asmname=tm;}
			else
			{string tm=tempmaker(); variables.pb(tm); $$->asmcode=$1->asmcode+$3->asmcode+"\tmov ax, "+$1->asmname+"\n\tsub ax, "+$3->asmname+"\n\tmov "+tm+", ax\n"; $$->asmname=tm;}

			Ccodeprint($$->show);
		 }

		  }
		  ;

term :	unary_expression
		{
			ruleprint("term -> unary_expression\n");
			$$=$1;
			Ccodeprint($$->show);
		}
     |  term MULOP unary_expression
	 {
		 ruleprint("term -> term MULOP unary_expression\n");
		 string mulop=$2->getName();
		 if($1->getDatatype()=="VOID" || $3->getDatatype()=="VOID") {
			 errprint("Trying to get value from void function\n");
			 $$=new SymbolInfo();
		   $$->show=$1->show+$2->getName()+$3->show;
		 }
		 else if($1->getDatatype()=="" || $3->getDatatype()=="")
		 {
			 $$=new SymbolInfo();
		   $$->show=$1->show+$2->getName()+$3->show;
		 }
		 else {
		 if(mulop=="%")
		 {
			 if($1->getDatatype()!="INT" || $3->getDatatype()!="INT"){
				 errprint("Both operands of \% operator must be integers\n");
				 $$=new SymbolInfo();
  			   $$->show=$1->show+$2->getName()+$3->show;
			 }
			 else
			{
				SymbolInfo* temp=new SymbolInfo("INT");
				temp->setIDtype("VAR");
				if($1->getIDtype()=="VAR" && $3->getIDtype()=="VAR") temp->ints[0]= ($1->ints[0]) % ($3->ints[0]);
				 else if($1->getIDtype()=="ARA" && $3->getIDtype()=="VAR") temp->ints[0]= ($1->ints[$1->getarrind()]) % ($3->ints[0]);
				 else if($1->getIDtype()=="VAR" && $3->getIDtype()=="ARA") temp->ints[0]= ($1->ints[0]) % ($3->ints[$3->getarrind()]);
				 else if($1->getIDtype()=="ARA" && $3->getIDtype()=="ARA") temp->ints[0]= ($1->ints[$1->getarrind()]) % ($3->ints[$3->getarrind()]);
				 temp->setDatatype("INT");
				 $$=temp;
  			   $$->show=$1->show+$2->getName()+$3->show;
			   string tm=tempmaker(); variables.pb(tm);
			   $$->asmcode=$1->asmcode+$3->asmcode;
			   $$->asmcode ="\tmov ax, "+$1->asmname + "\n\tmov bx, "+$3->asmname+"\n";
			   $$->asmcode+="\tdiv bx\n";
			   $$->asmcode+="\tmov "+tm+", dx\n";
			   //cout << $$->asmcode << endl;
			   $$->asmname=tm;
			 }
		 }
		 else
		 {
			 SymbolInfo* temp=new SymbolInfo("INT");
			 temp->setIDtype("VAR");

		 $$=temp;
	   $$->show=$1->show+$2->getName()+$3->show;
	   $$->asmcode=$1->asmcode+$3->asmcode;
	   $$->asmcode ="\tmov ax, "+$1->asmname + "\n\tmov bx, "+$3->asmname+"\n";
	   string tm=tempmaker(); variables.pb(tm);
	   if(mulop=="*")
	   {
		   $$->asmcode+="\tmul bx\n";
		   $$->asmcode+="\tmov "+tm+", ax\n";
	   }

	   else if(mulop=="/")
	   {
		   $$->asmcode+="\tdiv bx\n";
		   $$->asmcode+="\tmov "+tm+", ax\n";
	   }
	   //cout << $$->asmcode << endl;
	   $$->asmname=tm;
		}
	}
	   Ccodeprint($$->show);
	 }
     ;

unary_expression : ADDOP unary_expression
		{
			ruleprint("unary_expression -> ADDOP unary_expression\n");
			if($2->getDatatype()=="VOID") {
				errprint("Trying to get value from void function\n"); $$=new SymbolInfo();
			  $$->show=$1->getName()+$2->show;
		  }
			else if($2->getDatatype()=="")
			{
				$$=new SymbolInfo();
			  $$->show=$1->getName()+$2->show;
			}
			else {
			if($1->getName()=="-")
			{
			}
			$$=$2;
		  $$->show=$1->getName()+$2->show;

		  if($1->getName()=="+") {
		  $$->asmcode=$2->asmcode; $$->asmname=$2->asmname; }
		  else { string tm=tempmaker(); variables.pb(tm);
		  $$->asmcode=$2->asmcode+"\tmov ax, "+$2->asmname+"\n\tneg ax\n\tmov "+tm+", ax\n"; $$->asmname=tm;}

		  //cout << $$->asmcode << endl;
	  }
		  Ccodeprint($$->show);
		}
		 | NOT unary_expression
		 {
			 ruleprint("unary_expression -> NOT unary_expression\n");
			 if($2->getDatatype()=="VOID") { errprint("Trying to get value from void function\n"); $$=new SymbolInfo();
		   $$->show="!"+$2->show; }
			 else if($2->getDatatype()=="")
			 {
				 $$=new SymbolInfo();
  			   $$->show="!"+$2->show;
			 }
			 else {
			 SymbolInfo *temp=new SymbolInfo("INT");
			 temp->setIDtype("VAR");
			 int val;
			   temp->ints[0]=val;
			   string tm=tempmaker(); variables.pb(tm);
			   $$=temp;
			 $$->show="!"+$2->show; $$->asmcode=$2->asmcode+"\tmov ax, "+$2->asmname+"\n\tnot ax\n\tmov "+tm+", ax\n"; $$->asmname=tm; }
			 Ccodeprint($$->show);
		 }
		 | factor
		 {
			 ruleprint("unary_expression -> factor\n"); $$=$1; Ccodeprint($$->show);
		 }
		 ;

factor	: variable
	{
		ruleprint("factor -> variable\n");
		$$=$1; Ccodeprint($$->show);
		string tm=tempmaker(); variables.pb(tm);
		if($1->isarray)
		{

			$$->asmcode=$1->asmcode+"\tmov ax, "+$1->asmname+"[bx]\n";
			$$->asmcode+="\tmov "+tm+", ax\n";
			$$->asmname=tm;
		}
	}
	| ID LPAREN argument_list RPAREN
	{
		ruleprint("factor -> ID (argument_list)\n");
		SymbolInfo *temp9=symtable.Lookupwithparent($1->getName(),"FUNC");
		SymbolInfo *temp=new SymbolInfo();
		temp->copyfrom(temp9,temp);
		if(temp==NULL)  {
			errprint("Function "+$1->getName()+" wasn't declared\n"); $$=new SymbolInfo(); $$->show=$1->show+"("+$3->show+")";
		}

		else
		{
			int i=0, sz=callparamlist.size();
			if(sz!=temp->funcparamlist.size())
			{
				errprint("Function argument too many or insufficient\n");
				 $$=new SymbolInfo(); $$->show=$1->show+"("+$3->show+")";
			}
			else
			{
				ffr(i,0,sz)
				{
					if(callparamlist[i]!=temp->funcparamlist[i]) {errprint("Function argument type mismatch\n"); break;}
				}
				if(i==sz)
				{
					temp=new SymbolInfo(temp->getRettype());
					if(temp->getDatatype() == "INT")temp->ints[0] = 0;
					else if(temp->getDatatype() == "FLOAT")temp->floats[0] = 0;
					$$=temp;
					$$->show=$1->show+"("+$3->show+")";
					string tm=tempmaker();
					$$->asmcode=$3->asmcode+"\tcall "+$1->getName()+"\n\tmov "+tm+", dx\n\tpopf \n\tpop di\n\tpop si\n\tpop bp\n\tpop sp\n\tpop dx\n\tpop cx\n\tpop bx\n\tpop ax\n";
					$$->asmname=tm;
				}
				else
				{
					 $$=new SymbolInfo(); $$->show=$1->show+"("+$3->show+")";
				}
			}
			callparamlist.clear();
		}
		Ccodeprint($$->show);
	}
	| LPAREN expression RPAREN
	{
		ruleprint("factor -> (expression)\n"); $$=$2; $$->show="("+$2->show+")"; Ccodeprint($$->show); $$->asmcode=$2->asmcode; $$->asmname=$2->asmname;
	}
	| CONST_INT
	{
		ruleprint("factor -> CONST_INT\n");
		ll num=myatoi($1->getName());
		if(num==1e10) {errprint("Too large number, cannot fit into integer\n"); $$=new SymbolInfo();  }
		else
		{
			$1->setIDtype("VAR");
			$1->setDatatype("INT");
			$1->ints[0]=(int) num;
			$$=$1;
		}
		$$->show=$1->getName();
		$$->asmname=$$->show;
		Ccodeprint($$->show);
	}
	| CONST_FLOAT
	{
		ruleprint("factor -> CONST_FLOAT\n");
		double num=myatof($1->getName());

		if(num==-PI) { errprint("Too large number, cannot fit into float\n"); $$=$1; $$->show=$1->getName();}
		else
		{
			$1->setIDtype("VAR");
			$1->setDatatype("FLOAT");
			$1->floats[0]= num;
			$$=$1;
			$$->show=$1->getName();
		}
		Ccodeprint($$->show);
	}
	| variable INCOP
	{
		ruleprint("factor -> variable INCOP\n");
		if($1->getDatatype()=="FLOAT")
		{
			warningprint("Using increment operator on non integer variable\n");
		}
		$$=$1;
		$$->show+="++";
		string tm=tempmaker(); variables.pb(tm); $$->asmcode=$1->asmcode+"\tmov ax, "+$1->asmname+"\n\tmov "+tm+", ax\n\tinc ax\n\tmov "+$1->asmname+", ax\n"; $$->asmname=tm;
		Ccodeprint($$->show);
	}
	| variable DECOP
	{
		ruleprint("factor -> variable DECOP\n");
		if($1->getDatatype()=="FLOAT")
		{
			warningprint("Using decrement operator on non integer variable\n");
		}
		$$=$1;
		$$->show+="--";
		string tm=tempmaker(); variables.pb(tm); $$->asmcode=$1->asmcode+"\tmov ax, "+$1->asmname+"\n\tmov "+tm+", ax\n\tdec ax\n\tmov "+$1->asmname+", ax\n"; $$->asmname=tm;
		Ccodeprint($$->show);
	}
	;

argument_list : arguments {ruleprint("argument_list -> arguments\n");
$$=$1; Ccodeprint($$->show);
				}
			  | {$$=new SymbolInfo(); $$->show=""; Ccodeprint($$->show);}
			  ;

arguments : arguments COMMA logic_expression {ruleprint("arguments -> arguments, logic_expression\n"); callparamlist.pb($3->getDatatype());

$$=new SymbolInfo();
$$->show=$1->show+", "+$3->show;
$$->asmcode=$1->asmcode+$3->asmcode+"\tpush "+$3->asmname+"\n";
Ccodeprint($$->show);
}
	      | logic_expression {ruleprint("arguments -> logic_expression\n");
		  callparamlist.pb($1->getDatatype());

		  $$=new SymbolInfo();
		$$->show=$1->show; Ccodeprint($$->show);
		$$->asmcode=$1->asmcode+"\tpush "+$1->asmname+"\n";
	  }
	      ;
%%

int main(int argc, char* argv[])
{
	/*yydebug=1;*/
	symtable.EnterScope();
	///*
	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL)
	{
		printf("Cannot Open Input File.\n");
		exit(1);
	}
	/*
	logoutfile=fopen("logfile.txt", "w");
	errfile=fopen("errfile.txt", "w");
	asmfile=fopen("code.asm", "w"); //works for 3
	*/

	logoutfile=fopen("log.txt", "w");
	asmfile=fopen("code.asm", "w");

	yyin=fin;
	//*/

	yyparse();

	return 0;
}
