# ===========================================================================
# +++evaluater.py+++|
# __________________|
#
# Author: Sean Frischmann
# Class: Cse 305
# ===========================================================================
import sys
import bind
import primitives

def evaluate(user_input, env, active_env, isNested):
	position = 0
	isError = False
	isPrimitive = False
	isNegative = False
	isInsert = False
	if user_input == 'quit':
		sys.exit(0)
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
		if not primitives.if_func(env):
			isError = True
	elif user_input == 'bind':
		if not bind.bindFunc(user_input, env, active_env):
			isError = True
	elif user_input == ':true:' or user_input == ':false:':
		isInsert = True
	elif user_input == ':error:':
		isError = True
	elif user_input == 'add':
		temp = primitives.add_func(env)
		isPrimitive = True
	elif user_input == 'sub':
		temp = primitives.sub_func(env)
		isPrimitive = True
	elif user_input == 'div':
		temp = primitives.div_func(env)
		isPrimitive = True
	elif user_input == 'rem':
		temp = primitives.rem_func(env)
		isPrimitive = True
	elif user_input == 'mul':
		temp = primitives.mul_func(env)
		isPrimitive = True
	elif user_input == 'neg':
		temp = primitives.neg_func(env)
		env['stack'].pop(0)
		temp = str(temp)
		env['stack'].insert(0, temp)
		user_input = ''
	elif user_input == 'pop':
		topValue = env['stack'][0]
		if topValue[0] == '<':
			topValue = primitives.get_not_local(topValue)
		if topValue == ':closure:':
			env['global']['closures'].pop(0)
		env['stack'].pop(0)
		user_input = ''
	elif user_input == 'exc':
		temp = env['stack'][1]
		env['stack'].pop(1)
		env['stack'].insert(0 , temp)
		user_input = ''
	elif user_input[0].isalpha():
		user_input = bind.evalName(user_input, env, isNested)
		isInsert = True
	else:
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
