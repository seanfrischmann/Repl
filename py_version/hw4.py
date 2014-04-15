# ===========================================================================
# +++hw4.py+++|
# ____________|
#
# Author: Sean Frischmann
# Class: Cse 305
# ===========================================================================
import sys
import evaluater
import closure

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
	print (
		'\nInterpreter\n'
		'Author: Sean Frischmann\n'
		'Class: Cse 305\n'
	)
	env = dict()
	env['stack'] = list()
	env['global'] = dict()
	env['active'] = list()
	env['global']['bindings'] = dict()
	env['global']['closures'] = list()
	env['global']['binded closures'] = dict()
	env['inactive links'] = dict()
	mode = 'default'
	buf = ''
	isSpace = False
	active_env = 0
	closure_count = 0
	user_input = ''
	while True:
		if mode == 'default':
			buf = raw_input('repl> ')
		else:
			buf = raw_input(' ')
		position=0
		while position < len(buf):
			if buf[position] == '"':
				if mode == 'closure':
					mode = 'closure-string'
				elif mode == 'closure-string':
					mode = 'closure'
				elif mode == 'string':
					mode = 'default'
				else:
					mode = 'string'
			if buf[position] == '{':
				closure_count = closure_count + 1
				mode = 'closure'
			if (buf[position] == ' ') and (mode == 'default'):
				isSpace = True
			else:
				user_input = user_input + buf[position]
			if isSpace or position == len(buf)-1:
				if user_input == 'apply':
					if env['stack'][0] == ':closure:':
						buf = env['global']['closures'][0] + buf[position+1:len(buf)]
						env['global']['closures'].pop(0)
						env['stack'].pop(0)
						active_env += 1
						env['active'].insert(0,{})
						env['active'][0]['bindings'] = dict()
						env['active'][0]['binded closures'] = dict()
					else:
						buf = ':error:' + buf[position+1:len(buf)]
					user_input = ''
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
							user_input = ''
							position = 0
							continue
						except:
							user_input = ':false:'
							continue
				elif mode == 'string':
					user_input = user_input + '\n'
				elif mode == 'default':
					evaluater.evaluate(user_input,env, active_env)
					user_input = ''
					isSpace = False
			if buf[position] == '}' and mode == 'closure':
				closure_count = closure_count - 1
				if closure_count == 0:
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
						closure.createClosure(user_input, env)
						if isMore:
							position = position+1
					else:
						buf = user_input + buf[position+1:len(buf)]
						position = -1
					mode = 'default'
					user_input = ''
			position = position+1
		if mode == 'default':
			print_list(env['stack'])


repl()
