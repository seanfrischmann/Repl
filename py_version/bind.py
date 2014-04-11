# ===========================================================================
# +++bind.py+++|
# ____________|
#
# Author: Sean Frischmann
# Class: Cse 305
# ===========================================================================


def bindFunc(user_input, env):
	if (env['stack'][1])[0].isalpha(): 
		if not env['bindings'].has_key(env['stack'][1]):
			if env['stack'][0] == ':closure:':
				(env['binded closures'])[env['stack'][1]] = env['closures'][0]
				(env['bindings'])[env['stack'][1]] = ':closure:'
			else:
				(env['bindings'])[env['stack'][1]] = env['stack'][0]
			env['stack'].pop(1)
			return True
		else:
			return False
	else:
		return False
