# ===========================================================================
# +++primitives.py+++|
# ___________________|
#
# Author: Sean Frischmann
# Class: Cse 305
# ===========================================================================
import sys

def get_not_local(value):
	ret = ''
	i = 1
	isBuild = False
	isString = False
	while i < len(value):
		if value[i] == '>' and isString == False:
			break
		if value[i] == '"':
			if isString == True:
				isString = False
			else:
				isString = True
		if value[i] == ',' and isBuild == False and isString == False:
			i += 1
			isBuild = True
		if isBuild:
			ret = ret + value[i]
		i += 1
	return ret

def if_func(env):
	ret = True
	if len(env['stack']) >= 3:
		topValue = env['stack'][0]
		if env['stack'][0][0] == '<':
			topValue = get_not_local(env['stack'][0])
		if topValue == ':true:':
			env['stack'].pop(1)
			env['stack'].pop(0)
		elif topValue == ':false:':
			env['stack'].pop(2)
			env['stack'].pop(0)
		else:
			ret = False
	else:
		ret = False
	return ret

def add_func(env):
	topValue = env['stack'][0]
	bottomValue = env['stack'][1]
	if topValue[0] == '<':
		topValue = get_not_local(topValue)
	if bottomValue[0] == '<':
		bottomValue = get_not_local(bottomValue)
	return int(topValue) + int(bottomValue)

