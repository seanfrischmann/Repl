# ===========================================================================
# +++bind.py+++|
# ____________|
#
# Author: Sean Frischmann
# Class: Cse 305
# ===========================================================================


def bindFunc(user_input, env, active_env):
	local = ''
	not_local = ''
	if (env['stack'][1])[0].isalpha(): 
		local = env['stack'][1]
	elif (env['stack'][1])[0] == '<': 
		i = 1
		while i < len(env['stack'][1]):
			if env['stack'][1][i] == ',':
				break
			local = local + env['stack'][1][i]
			i += 1
	else:
		return False
	if (env['stack'][0])[0] == '<': 
		start_building = False
		i = 1
		while i < len(env['stack'][1]):
			if env['stack'][0][i] == '>':
				break
			if env['stack'][0][i] == ',':
				start_building = True
				i += 1
			if start_building:
				not_local = not_local + env['stack'][0][i]
			i += 1
	else:
		not_local = env['stack'][0]
	if active_env == 0:
		if (local) in env['global']['bindings']:
			return False
		else:
			if not_local == ':closure:':
				(env['global']['binded closures'])[local] = env['global']['closures'][0]
				(env['global']['bindings'])[local] = ':closure:'
			else:
				(env['global']['bindings'])[local] = not_local
			env['stack'].pop(1)
			return True
	else:
		if (local) in env['active'][0]['bindings']:
			return False
		else:
			if not_local == ':closure:':
				(env['active'][0]['binded closures'])[local] = env['global']['closures'][0]
				(env['active'][0]['bindings'])[local] = ':closure:'
			else:
				(env['active'][0]['bindings'])[local] = not_local
			env['stack'].pop(1)
			return True


def evalName(user_input, env, active_env):
	local = user_input
	not_local = None
	i = 1
	if (active_env > 0) and ((user_input) in env['active'][0]['bindings']):
		local = env['active'][0]['bindings'][user_input]
		not_local = env['active'][0]['bindings'][user_input]
		if not_local == ':closure:':
			env['global']['closures'].insert(0,env['active'][i]['binded closures'][user_input])
		i = active_env
	while i < active_env:
		if (user_input) in env['active'][i]['bindings']:
			not_local = env['active'][i]['bindings'][user_input]
			if not_local == ':closure:':
				env['global']['closures'].insert(0,env['active'][i]['binded closures'][user_input])
		i += 1
	if not_local  == None:
		if (user_input) in env['global']['bindings']:
			not_local = env['global']['bindings'][user_input]
			if active_env == 0:
				local = env['global']['bindings'][user_input]
			if not_local == ':closure:':
				env['global']['closures'].insert(0,env['global']['binded closures'][user_input])
		else:
			not_local = user_input
	ret = '<' + local + ',' + not_local + '>'
	return ret
