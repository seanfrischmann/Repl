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
import primitives

def print_list(stack):
	for i, v in enumerate(stack):
		print (v)

def load(inputFile):
	buf = ''
	with open(inputFile) as fileIn:
		while True:
			contents = fileIn.read(1)
			if not contents:
				fileIn.close()
				break;
			if contents == '\n':
				if buf[len(buf)-1] != ' ':
					buf = buf + ' '
			else:
				buf = buf + contents
	if buf[len(buf)-1] == ' ':
		buf = buf[0:len(buf)-1]
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
	env['inactive'] = list()
	mode = 'default'
	buf = ''
	isSpace = False
	isNested = False
	apply_count = 0
	temp_buf = []
	temp_position = []
	active_env = 0
	closure_count = 0
	user_input = ''
	while True:
		position=0
		if apply_count > 0:
			if isNested:
				isNested = False
				env['inactive'] = list()
			if len(temp_buf) != 0:
				buf = temp_buf[0]
				temp_buf.pop(0)
				position = temp_position[0]
				temp_position.pop(0)
				position += 1
			apply_count += -1
			active_env += -1
			if len(env['active']) != 0:
				env['active'].pop(0)
		elif mode == 'default':
			buf = input('repl> ')
		else:
			buf = input(' ')
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
					topValue = env['stack'][0]
					if topValue[0] == '<':
						topValue = primitives.get_not_local(topValue)
					if topValue == ':closure:':
						temp_buf.insert(0, buf)
						temp_position.insert(0, position)
						closureValue = env['global']['closures'][0]
						env['active'].insert(0,{})
						env['active'][0]['bindings'] = dict()
						env['active'][0]['binded closures'] = dict()
						if isinstance(closureValue, dict):
							isNested = True
							linked_env = []
							for val in closureValue.values():
								linked_env = linked_env + val
							env['inactive'] = linked_env
							env['inactive'].insert(0,env['active'][0])
							for val in closureValue.keys():
								closureValue = val
						buf = closureValue
						env['global']['closures'].pop(0)
						env['stack'].pop(0)
						active_env += 1
					else:
						buf = ':error:' + buf[position+1:len(buf)]
					user_input = ''
					apply_count += 1
					isSpace = False
					position = 0
					continue
				elif user_input == 'load':
					if len(env['stack']) > 0 and (env['stack'][0])[0] == '"':
						try:
							inputFile = env['stack'][0]
							inputFile = inputFile.replace('"','')
							if position == len(buf)-1:
								buf = buf[position+1:len(buf)]
							else:
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
					evaluater.evaluate(user_input,env, active_env, isNested)
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
						closure.createClosure(user_input, env, active_env)
						if isMore:
							position = position+1
					else:
						buf = user_input + buf[position+1:len(buf)]
						position = -1
					mode = 'default'
					user_input = ''
			position = position+1
		if mode == 'default' and (apply_count == 0):
			print_list(env['stack'])


repl()
