/*
	The MIT License

	Copyright (c) 2008 IFMO/GameDev Studio

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/
// shell_cmd.cpp - 


#include "..\global.h"	
#include "shell.h"	


IShell	*CreateShell( void )
{
	return new EShell();
}

/*-----------------------------------------------------------------------------
	Init/Shutdown stuff :
-----------------------------------------------------------------------------*/
EShell::EShell(void)
{
}


EShell::~EShell(void)
{
	LOGF("SHUTDOWN : Shell\r\n");
	delete parser;
}


/*-----------------------------------------------------------------------------
	Command stuff :
-----------------------------------------------------------------------------*/

//
//	EShell::AddCommand
//
void EShell::AddCommand(const char *name, EShellCmd_f cmd_func, void *self)
{
//	ASSERT(IsConsoleIdent(name));

	ECmdEntry	cmd;
	cmd.name	=	name;
	cmd.func	=	cmd_func;
	cmd.self	=	self;
	
	cmd_list.push_back(cmd);
}


//
//	EShell::RemoveCommand
//
void EShell::RemoveCommand(const char *name)
{
	vector<ECmdEntry>::iterator vi;

	// NOTE : command queue should be flushed!
	FlushCmdQueue();
	
	for (vi=cmd_list.begin(); vi!=cmd_list.end(); vi++)
	{
		if (strcmp(vi->name.c_str(), name)==0)
		{
			cmd_list.erase(vi);
			return;
		}
	}
	
	SIGNAL(va("command \"%s\" does not exist", name))
}


//
//	EShell::CompleteCommand
//															
const char *EShell::CompleteCommand(const char *part)		
{				
	static	char	common[1024];
	strcpy(common, "");
	uint matched = 0;
	
	if (strlen(part)==0) return "";
	
	//	scan cmd list :
	for (uint i=0; i<cmd_list.size(); i++)
	{
		const char	*s = cmd_list[i].name.c_str();
		
		if ( _strnicmp(part, s, strlen(part)) == 0 )
		{
			LOGF("  %s\r\n", s);
			if (common[0]=='\0')	strcpy(common, s);
			
			common_parti(common, s);
			
			matched++;
		}
	}
	
	//	scan var list :
	for (uint i=0; i<var_list.size(); i++)
	{
		const char	*s = var_list[i]->Name();
		
		if ( _strnicmp(part, s, strlen(part)) == 0 )
		{
			LOGF("  %-20s = %s\r\n", s, var_list[i]->String());
			if (common[0]=='\0')	strcpy(common, s);
			
			common_parti(common, s);
			
			matched++;
		}
	}


	if (matched==0) return part;
	
	LOGF("\r\n");
	
	if (matched==1)	strcat(common, " ");
	
											
	return common;
}															
															
															

/*-----------------------------------------------------------------------------
	Command execution stuff :
	
	alpha ::= a..z|A..Z|"_";				// alphabetical chars and '_'
	digit ::= "0"|"1"|"2"|...|"9";			// digits
	ident ::= [alpha|digit];				// "12345abcd" is identifier too.
	ident ::= "\""[alpha|digit|space]"\"";			
	args  ::= ident
	space ::= "\s"|"\t"
	end   ::= ";"|"\n"
	
-----------------------------------------------------------------------------*/

void EShell::FlushCmdQueue(void)
{
	if (cmd_queue.empty()) {
		return;
	}
	
	while (!cmd_queue.empty()) {
		cmd_queue.pop();
	}
	
	LOGF("Command buffer have been flushed\r\n");
}


class EExecDepth {
	public:
		EExecDepth	( int *c )	{ counter = c; (*counter)++; }
		~EExecDepth	( void )	{ (*counter)--; }
	private:
	    int *counter;
	};


void EShell::CallCommand(EArgs args)
{
	ASSERT(args.Argc() >= 1);

	EExecDepth depth(&execution_depth);
	
	//	Check recursive call :	
	if (execution_depth>5) {
		RAISE_EXCEPTION(va("Recursive call of command \"%s\"", args.Argv(0)));
	}

	//	Dereference variables :	
	EArgs new_args;
	for (int i=0; i<args.Argc(); i++)
	{
		new_args.NewArg();
		
		string arg = args.Argv(i);
		
		if (i==0 && strcmp(arg.c_str(), "endwith")!=0) {	// add prefix :
			string t = arg;
			arg = with_prefix;
			arg += t;
		}
		
		if (i>=1 && arg[0]=='$') {
			string var_name = &arg[1];
			IPxVar var = FindVar(var_name.c_str());
			if (!var) {
				RAISE_EXCEPTION(va("Can not refer to var \"%s\"", var_name.c_str()));
			}
			arg = var->String();
		}
		
		for (uint j=0; j<arg.length(); j++) {
			new_args.PushChar(arg[j]);
		}
	}

	//LOGF("CALL : [%s] [%s] [%s] ... \r\n", new_args.Argv(0), new_args.Argv(1), new_args.Argv(2));

	//	execute command :
	for (uint i=0; i<cmd_list.size(); i++)
	{
		if (strcmp(cmd_list[i].name.c_str(), new_args.Argv(0))==0)
		{
			EQueuedCmd	queued_cmd;
			queued_cmd.func	=	cmd_list[i].func;
			queued_cmd.self	=	cmd_list[i].self;
			queued_cmd.args	=	new_args;
			
			cmd_queue.push(queued_cmd);
			
			if (immediate_mode) {
				ExecuteEnqueuedCommands();
			}
			
			return;
		}
	}
						 
	//	try var functions :
	IPxVar var	= FindVar(new_args.Argv(0));
	if (var) 
	{
		EQueuedCmd	queued_cmd;
		queued_cmd.func	=	(EShellCmd_f)VarCmd_f;
		queued_cmd.self	=	this;	// "self"
		queued_cmd.args	=	new_args;
		
		cmd_queue.push(queued_cmd);

		if (immediate_mode) {
			ExecuteEnqueuedCommands();
		}

		return;
	}

	RAISE_EXCEPTION(va("unknown command : %s", new_args.Argv(0)));
}


void EShell::ExecuteString(const char *cmd_text)
{
	try {
		parser->ParseText(cmd_text);
	} catch (exception &e)
	{
		LOG_ERROR("Shell : %s\r\n", e.what());
	}
}


void EShell::ExecuteEnqueuedCommands(void)
{
	defer_cmds = false;

	const uint MAX_COMMANDS = 10000;	
	uint counter = 0;

	try 
	{
		while (!cmd_queue.empty())
		{
			counter++;
			
			if (counter>MAX_COMMANDS) {
				FlushCmdQueue();
				RAISE_EXCEPTION(va("Can not execute more than %d commands per frame", MAX_COMMANDS));
			}
		
			EQueuedCmd	queued_cmd = cmd_queue.front();
			cmd_queue.pop();

			queued_cmd.func(queued_cmd.self, queued_cmd.args.Argc(), queued_cmd.args.Argv());
			
			if (defer_cmds) {
				break; // wait for next ExecuteEnqueuedCommands()
			}
		}
			
	} catch (exception &e)
	{
		//FlushCmdQueue();
		
		LOG_ERROR("Shell : %s\r\n", e.what());
	}
}


//
//	EShell::ExecuteDelayedCommand
//
void EShell::ExecuteDelayedCommand(int delta_time)
{
	uint num=0;
	
	vector<EDelayedCmd> temp;
	
	for (uint i=0; i<delayed_cmds.size(); i++) {
	
		EDelayedCmd *cmd = &delayed_cmds[i];
		
		cmd->timeout-=delta_time;

		if (cmd->timeout<=0) {
			ExecuteString(va("%s\n", cmd->command.c_str()));
			continue;
		} else {
			temp.push_back(delayed_cmds[i]);
		}
	}
	
	delayed_cmds = temp;
}


//
//	EShell::ExecuteConfig
//
void EShell::ExecuteConfig(const char *file_name, bool immediate_exec)
{
	static uint execution_depth = 0;
	
	try {
		IPxFileSystem	fs =	Linker()->GetFileSystem();
		
		immediate_mode	=	immediate_exec;
	
		LOGF("execing : %s\r\n", file_name);
	
		IPxFile	f	=	fs->FileOpen(file_name, FS_OPEN_READ);

		string	buffer;
		buffer.resize(f->Size()+1, '\0');
		f->Read(&buffer[0], f->Size());
		
		ExecuteString(buffer.c_str());
		
	} catch (exception &e)
	{
		LOG_ERROR("couldn`t exec \"%s\"\r\n", file_name );
		LOG_ERROR("%s\r\n", e.what());
	}
	immediate_mode	=	false;
	with_prefix = "";
}


//
//	EShell::WriteConfig
//
void EShell::WriteConfig(const char *file_name)
{
	try {
		LOGF("writing cfg : %s\r\n", file_name);
	
		IPxFileSystem	fs	=	Linker()->GetFileSystem();
		IPxFile	f	=	fs->FileOpen(file_name, FS_OPEN_WRITE);

		//	write header :
		f->Printf("// config : %s\r\n", file_name);
		f->Printf("\r\n");

		f->Printf("// variables : \r\n");

		//	write variables :
		for (uint i=0; i<var_list.size(); i++)
		{	
			EVar *var = var_list[i].To<EVar>();
			if (var->Flags() & SHLF_NOSAVE) continue;

			if (var->Flags() & SHLF_EXECUTABLE) {
				f->Printf("setx %-24s \"%s\"\r\n", var->Name(), parser->AddEscapeSequences(var->String()).c_str());			
			} else {
				f->Printf("set %-24s \"%s\"\r\n", var->Name(), parser->AddEscapeSequences(var->String()).c_str());			
			}
		}
		
	} catch (exception &e)
	{
		LOG_ERROR("couldn`t write \"%s\"\r\n", file_name );
		LOG_ERROR("%s\r\n", e.what());
	}
}


/*-----------------------------------------------------------------------------
	Shell Commands 
-----------------------------------------------------------------------------*/

void EShell::Delay_f(EShell *self, int argc, const char **argv)
{
	// IConsole	*con	=	self->con;
	if (argc!=3) {
		LOGF("usage : delay <time in ms> <command>\r\n");
		return;
	}
	EDelayedCmd cmd;
	cmd.timeout = atoi(argv[1]);
	cmd.command = argv[2];
	
	self->delayed_cmds.push_back(cmd);
}


void EShell::Exec_f(EShell *self, int argc, const char **argv)
{
	// IConsole	*con	=	self->con;
	if (argc!=2) {
		LOGF("usage : exec <config name>\r\n");
		return;
	}
	self->ExecuteConfig(argv[1], false);
}


void EShell::WriteCfg_f(EShell *self, int argc, const char **argv)
{
	// IConsole	*con	=	self->con;
	if (argc!=2) {
		LOGF("usage : writeCfg <config name>\r\n");
		return;
	}
	self->WriteConfig(argv[1]);
}


bool IsNamespace(const char *name);

void EShell::ListCmds_f(EShell *self, int argc, const char **argv)
{
	// IConsole	*con	=	self->con;
	uint num = (uint)self->cmd_list.size();
	
	LOGF("\r\n");
	for (uint i=0; i<num; i++)
	{
		if ( !IsNamespace(self->cmd_list[i].name.c_str()) ) {
			LOGF("  %s\r\n", self->cmd_list[i].name.c_str());
		}
	}
	LOGF("%d cmds\r\n", num);
}


void EShell::TestArgs_f(EShell *self, int argc, const char **argv)
{
	// IConsole	*con	=	self->con;
	for (int i=0; i<argc; i++)
	{
		LOGF("[%s] ", argv[i]);
	}
	LOGF("\r\n");
	LOGF("%d args\r\n", argc);
}


void EShell::Wait_f(EShell *self, int argc, const char **argv)
{
	// IConsole *con = self->con;
	if (self->immediate_mode) {
		LOG_WARNING("Command \"wait\" does not work in immediate mode");
	}
	self->defer_cmds = true;
}


//void EShell::ImmediateMode_f(EShell *self, int argc, const char **argv)
//{
//	// IConsole *con = self->con;
//	if (argc!=2) {
//		if (self->immediate_mode) {
//			LOGF("Immediate execution mode ON\r\n");
//		} else {
//			LOGF("Immediate execution mode OFF\r\n");
//		}
//		LOGF("usage : immediateMode <0|1>\r\n");
//		LOGF("   0 - disable immediate mode\r\n");
//		LOGF("   1 - enable immediate mode\r\n");
//	}
//	
//	if (atoi(argv[1])==0) {
//		self->immediate_mode	=	false;
//	}
//	if (atoi(argv[1])==1) {
//		self->immediate_mode	=	true;
//	}
//}


void EShell::Echo_f(EShell *self, int argc, const char **argv)
{
	// IConsole *con = self->con;
	for (int i=1; i<argc; i++) {
		LOGF("%s ", argv[i]);
	}
	LOGF("\r\n");
}


void EShell::Math_f(EShell *self, int argc, const char **argv)
{
	// IConsole *con = self->con;
	if (argc!=4) {
		LOGF("usage : %s <dst> <op1> <op2>\r\n", argv[0]);
	}
	
	const char *op = argv[0];
	
	IPxVar	var = self->FindVar(argv[1]);
	float	a = (float)atof(argv[2]);
	float	b = (float)atof(argv[3]);
	
	if (!var) {
		RAISE_EXCEPTION(va("Dst var \"%s\" does not exist", argv[1]));
	}
	
	if (strcmp(op, "add")==0) {	
		var->SetValue(va("%f", a + b));	return;
	}
	if (strcmp(op, "sub")==0) {	
		var->SetValue(va("%f", a - b));	return;
	}
	if (strcmp(op, "mul")==0) {	
		var->SetValue(va("%f", a * b));	return;
	}
	if (strcmp(op, "div")==0) {	
		var->SetValue(va("%f", a / b));	return;
	}
	
	bool cmd_result = false;
	
	if (strcmp(op, "cmpEq")==0) {	
		if (a==b) cmd_result = true;
	}
	if (strcmp(op, "cmpLEq")==0) {	
		if (a>=b) cmd_result = true;
	}
	if (strcmp(op, "cmpGEq")==0) {	
		if (a<=b) cmd_result = true;
	}
	if (strcmp(op, "cmpGrtr")==0) {	
		if (a>b) cmd_result = true;
	}
	if (strcmp(op, "cmpLess")==0) {	
		if (a<b) cmd_result = true;
	}
	
	if (cmd_result) {
		var->SetValue("1");
	} else {
		var->SetValue("0");
	}
}


void EShell::CondCall_f(EShell *self, int argc, const char **argv)
{	
	// IConsole *con = self->con;
	if (argc!=3) {
		LOGF("usage : condCall <condition> <command>\r\n");
		return;
	}
	
	int cond = atoi(argv[1]);
	
	if (cond) {
		self->ExecuteString(va("%s\n", argv[2]));
	}
}


void EShell::With_f(EShell *self, int argc, const char **argv)
{
	// IConsole *con = self->con;
	if (argc!=2) {
		LOGF("usage : with <object name>\r\n");
		return;
	}
	self->with_prefix	=	va("%s.", argv[1]);
}


void EShell::EndWith_f(EShell *self, int argc, const char **argv)
{
	self->with_prefix	=	"";
}








