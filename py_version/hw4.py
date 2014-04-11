# ===========================================================================
# +++hw4.py+++|
# ____________|
#
# Author: Sean Frischmann
# Class: Cse 305
# ===========================================================================
import sys
import parser

def print_list(stack):
	for i, v in enumerate(stack):
		print v

def load(inputFile):
	buf = ''
	with open(inputFile) as fileIn:
		while True:
			contents = fileIn.read(1)
			if not contents:
				fileIn.close()
				break;
			if contents != '\n':
				buf = buf + contents
	return buf

def repl():
	print('\nInterpreter\n'
			'Author: Sean Frischmann\n'
			'Class: Cse 305\n'
			'Incomplete Features:\n'
			'    functions and apply\n')
	env = dict()
	env['stack'] = list()
	env['bindings'] = dict()
	env['closures'] = list()
	env['binded closures'] = dict()
	isPrompt = True
	isSpace = False
	isString = False
	isClosure = False
	isFile = False
	user_input = ''
	buf = ''
	while True:
		if isPrompt:
			buf = raw_input('repl> ')
		elif not isFile:
			buf = raw_input(' ')
		elif isFile:
			isFile = False
			isPrompt = True
		position=0
		while position < len(buf):
			if buf[position] == '"':
				if isString == True:
					isPrompt = True
					isString = False
				else:
					isString = True
					isPrompt = False
			if buf[position] == '{':
					isPrompt = False
					isClosure = True
			if (buf[position] == ' ') and (not isString) and (not isClosure):
				isSpace = True
			else:
				user_input = user_input + buf[position]
			if isSpace or position == len(buf)-1:
				if user_input == 'apply':
					if env['stack'][0] == ':closure:':
						buf = env['closures'][0] + buf[position+1:len(buf)]
						env['closures'].pop(0)
						env['stack'].pop(0)
					else:
						buf = ':error:' + buf[position+1:len(buf)]
					user_input = ""
					position = 0
					continue
				elif user_input == 'load':
					if len(env['stack']) > 0 and (env['stack'][0])[0] == '"':
						try:
							inputFile = env['stack'][0]
							inputFile = inputFile.replace('"','')
							buf = buf[position:len(buf)]
							buf = load(inputFile)+' '+':true:'+buf
							env['stack'].pop(0)
							isFile = True
							isPrompt = False
							user_input = ''
							break;
						except:
							user_input = ':error:'
							continue
				elif (not isString) and (not isClosure):
					parser.parse(user_input,env)
					user_input = ''
					isSpace = False
			if buf[position] == '}' and (isClosure):
				isFrontValid = True
				isEndValid = True
				isMore = True
				if user_input[1] != ' ':
					isFrontValid = False
					temp = ':error:'+user_input[2:len(user_input)]
					user_input = temp
				if user_input[len(user_input)-2] != ' ':
					isEndValid = False
					if isFrontValid:
						temp = ':error:'+user_input[1:len(user_input)]
						user_input = temp
				if (position+1) < len(buf):
					if buf[position+1] != ' ':
						if isFrontValid:
							isFrontValid = False
							temp = ':error:'+user_input[1:len(user_input)]
							user_input = temp
					else:
						isMore = True
				if isFrontValid and isEndValid:
					user_input = user_input.replace('{','')
					user_input = user_input.replace('}','')
					user_input = user_input[1:len(user_input)-1]
					env['closures'].insert(0,user_input)
					env['stack'].insert(0,':closure:')
					if isMore:
						position = position+1
				else:
					buf = user_input + buf[position+1:len(buf)]
					position = -1
				isPrompt = True
				isClosure = False
				user_input = ''
			position = position+1
		if isPrompt:
			print "STACK:"
			print_list(env['stack'])
			print "BINDINGS:"
			print_list(env['bindings'])
			print "CLOSURES:"
			print_list(env['closures'])


repl()
