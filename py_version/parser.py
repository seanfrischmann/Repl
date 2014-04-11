# ===========================================================================
# +++parser.py+++|
# _______________|
#
# Author: Sean Frischmann
# Class: Cse 305
# ===========================================================================
import sys
import bind

def parse(user_input, env):
	position = 0
	while position < len(user_input):
		if user_input[position] == '\n':
			user_input[position].replace('\n','')
		position = position+1
	isError = False
	isPrimitive = False
	isNegative = False
	isInsert = False
	if user_input == 'quit':
		sys.exit(0)
	try:
		temp = None
		if user_input[0] == '-':
			t = 1
			temp = ''
			while t < len(user_input):
				temp = temp + user_input[t]
				t = t+1
			if temp.isdigit():
				isNegative = True
		if user_input.isdigit() or isNegative:
			isInsert = True
			isNegative = False
		elif user_input[0] == '"':
			isInsert = True
		elif user_input == 'if':
			if len(env['stack']) >= 3 and (env['stack'][0] == ':true:' or env['stack'][0] == ':false:'):
				if env['stack'][0] == ':true:':
					env['stack'].pop(1)
				else:
					env['stack'].pop(2)
				env['stack'].pop(0)
			else:
				isError = True
		elif user_input == 'bind':
			if not bind.bindFunc(user_input, env):
				isError = True
		elif user_input == ':true:' or user_input == ':false:':
			isInsert = True
		elif user_input == ':error:':
			isError = True
		elif user_input == 'add':
			temp = int(env['stack'][1]) + int(env['stack'][0])
			isPrimitive = True
		elif user_input == 'sub':
			temp = int(env['stack'][1]) - int(env['stack'][0])
			isPrimitive = True
		elif user_input == 'div':
			if int(env['stack'][1]) < 0 and int(env['stack'][0]) < 0:
				temp = int(env['stack'][1]) / int(env['stack'][0])
				temp = temp + 1
			else:
				temp = int(env['stack'][1]) / int(env['stack'][0])
			isPrimitive = True
		elif user_input == 'rem':
			if int(env['stack'][1]) < 0 and int(env['stack'][0]) < 0:
				temp2 = int(env['stack'][1]) / int(env['stack'][0])
				temp2 = temp2 + 1
				temp = int(env['stack'][1]) - (temp2*int(env['stack'][0]))
			else:
				temp = int(env['stack'][1]) % int(env['stack'][0])
			isPrimitive = True
		elif user_input == 'mul':
			temp = int(env['stack'][1]) * int(env['stack'][0])
			isPrimitive = True
		elif user_input == 'neg':
			temp = int(env['stack'][0]) * int(-1)
			env['stack'].pop(0)
			temp = str(temp)
			env['stack'].insert(0, temp)
			user_input = ''
		elif user_input == 'pop':
			if env['stack'][0] == ':closure:':
				env['closures'].pop(0)
			env['stack'].pop(0)
			user_input = ''
		elif user_input == 'exc':
			temp = env['stack'][1]
			env['stack'].pop(1)
			env['stack'].insert(0 , temp)
			user_input = ''
		elif user_input[0].isalpha():
			while (env['bindings']).has_key(user_input):
				temp = env['bindings'][user_input]
				if temp == ':closure:':
					env['closures'].insert(0,env['binded closures'][user_input])
				user_input = temp
			isInsert = True
		else:
			isError = True
	except:
		isError = True
	if isPrimitive:
		env['stack'].pop(0)
		env['stack'].pop(0)
		temp = str(temp)
		env['stack'].insert(0, temp)
		user_input = ''
	if isInsert:
		env['stack'].insert(0, user_input)
		user_input = ''
	if isError:
		env['stack'].insert(0, ':error:')
