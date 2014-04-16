# ===========================================================================
# +++bind.py+++|
# ____________|
#
# Author: Sean Frischmann
# Class: Cse 305
# ===========================================================================
import primitives


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
		not_local = primitives.get_not_local(env['stack'][0])
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


def evalName(user_input, env, isNested):
	local = user_input
	not_local = None
	current_env = ''
	if isNested:
		current_env = 'inactive'
	else:
		current_env = 'active'
	i = 1
	env_len = len(env[current_env])
	if (env_len > 0) and ((user_input) in env[current_env][0]['bindings']):
		local = env[current_env][0]['bindings'][user_input]
		not_local = env[current_env][0]['bindings'][user_input]
		if not_local == ':closure:':
			env['global']['closures'].insert(0,env[current_env][i]['binded closures'][user_input])
		i = env_len
	while i < env_len:
		if (user_input) in env[current_env][i]['bindings']:
			not_local = env[current_env][i]['bindings'][user_input]
			if not_local == ':closure:':
				env['global']['closures'].insert(0,env[current_env][i]['binded closures'][user_input])
		i += 1
	if not_local  == None:
		if (user_input) in env['global']['bindings']:
			not_local = env['global']['bindings'][user_input]
			if env_len == 0:
				local = env['global']['bindings'][user_input]
			if not_local == ':closure:':
				env['global']['closures'].insert(0,env['global']['binded closures'][user_input])
		else:
			not_local = user_input
	ret = '<' + local + ',' + not_local + '>'
	return ret
