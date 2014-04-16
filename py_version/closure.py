# ===========================================================================
# +++closure.py+++|
# ________________|
#
# Author: Sean Frischmann
# Class: Cse 305
# ===========================================================================
import sys

def createClosure(user_input, env, active_env):
	user_input = user_input[2:len(user_input)-2]
	if active_env > 0:
		temp = env['active'].copy()
		user_input = {user_input: temp}
	env['global']['closures'].insert(0,user_input)
	env['stack'].insert(0,':closure:')
