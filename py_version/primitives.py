# ===========================================================================
# +++primitives.py+++|
# ___________________|
#
# Author: Sean Frischmann
# Class: Cse 305
# ===========================================================================
import sys

def get_local(value):
	local = ''
	i = 1
	isString = False
	while i < len(value):
		if value[i] == '"':
			if isString:
				isString = False
			else:
				isString = True
		if value[i] == ',' and (not isString):
			break
		local = local + value[i]
		i += 1
	return local

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
			continue
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

def sub_func(env):
	topValue = env['stack'][0]
	bottomValue = env['stack'][1]
	if topValue[0] == '<':
		topValue = get_not_local(topValue)
	if bottomValue[0] == '<':
		bottomValue = get_not_local(bottomValue)
	return int(bottomValue) - int(topValue)

def div_func(env):
	topValue = env['stack'][0]
	bottomValue = env['stack'][1]
	if topValue[0] == '<':
		topValue = get_not_local(topValue)
	if bottomValue[0] == '<':
		bottomValue = get_not_local(bottomValue)
	temp = 0
	temp = int(bottomValue) // int(topValue)
	if int(bottomValue) < 0 and int(topValue) < 0:
		temp = temp + 1
	return temp

def rem_func(env):
	topValue = env['stack'][0]
	bottomValue = env['stack'][1]
	if topValue[0] == '<':
		topValue = get_not_local(topValue)
	if bottomValue[0] == '<':
		bottomValue = get_not_local(bottomValue)
	temp = 0
	if int(bottomValue) < 0 and int(topValue) < 0:
		temp2 = int(bottomValue) // int(topValue)
		temp2 = temp2 + 1
		temp = int(bottomValue) - (temp2*int(topValue))
	else:
		temp = int(bottomValue) % int(topValue)
	return temp

def mul_func(env):
	topValue = env['stack'][0]
	bottomValue = env['stack'][1]
	if topValue[0] == '<':
		topValue = get_not_local(topValue)
	if bottomValue[0] == '<':
		bottomValue = get_not_local(bottomValue)
	return int(bottomValue) * int(topValue)

def neg_func(env):
	topValue = env['stack'][0]
	if topValue[0] == '<':
		topValue = get_not_local(topValue)
	return int(topValue) * int(-1)

def equal_func(env):
	topValue = env['stack'][0]
	topLocal = ''
	topNot = ''
	bottomValue = env['stack'][1]
	bottomLocal = ''
	bottomNot = ''
	isBinding = False
	if topValue[0] == '<':
		topLocal = get_local(topValue)
		topNot = get_not_local(topValue)
		isBinding = True
	if bottomValue[0] == '<':
		bottomLocal = get_local(bottomValue)
		bottomNot = get_not_local(bottomValue)
		isBinding = True
	if (topValue == ':closure:' or bottomValue == ':closure:'
			or topNot == ':closure:' or bottomNot == ':closure:'):
		return False
	if isBinding:
		if (topLocal == bottomLocal and topNot == bottomNot):
			return True
		else:
			return False
	else:
		if topValue == bottomValue:
			return True
		else:
			return False

def len_func(env):
	topValue = env['stack'][0]
	if topValue[0] == '<':
		topValue = get_not_local(topValue)
	if topValue[0] == '"':
		ret = len(topValue) - 2
		ret = str(ret)
		env['stack'].insert(0,ret)
		return True
	else:
		return False

def cat_func(env):
	topValue = env['stack'][0]
	bottomValue = env['stack'][1]
	if topValue[0] == '<':
		topValue = get_not_local(topValue)
	if bottomValue[0] == '<':
		bottomValue = get_not_local(bottomValue)
	if topValue[0] == '"' and bottomValue[0] == '"':
		ret = bottomValue + topValue
		ret = ret.replace('"','')
		ret = '"'+ret+'"'
		env['stack'].pop(0)
		env['stack'].pop(0)
		env['stack'].insert(0,ret)
		return True
	else:
		return False

def less_func(env):
	topValue = env['stack'][0]
	bottomValue = env['stack'][1]
	if topValue[0] == '<':
		topValue = get_not_local(topValue)
	if bottomValue[0] == '<':
		bottomValue = get_not_local(bottomValue)
	if isinstance(int(bottomValue),int) and isinstance(int(topValue),int):
		if int(bottomValue) < int(topValue):
			env['stack'].insert(0,':true:')
		else:
			env['stack'].insert(0,':false:')
		env['stack'].pop(1)
		env['stack'].pop(1)
		return True
	else:
		return False

