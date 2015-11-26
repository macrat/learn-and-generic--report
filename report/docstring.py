#!/usr/bin/python3

import sys
import re


if len(sys.argv) <= 1:
	print('usage: {} [FILE]'.format(sys.argv[0]))
else:
	with open(sys.argv[1], 'r') as fp:
		dat = fp.read()

	#defines = tuple('\\item[{0}] {1}'.format(m.group('name'), m.group('doc')).replace(r'_', r'\_') for m in re.finditer('#define (?P<name>[^ \t]+)[ \t]+.*?/\*(?P<doc>.+)\*/', dat))
	#if defines:
	#	print(r'\subsection{定数}')
	#	print(r'\begin{description}')
	#	print('\n'.join(defines))
	#	print(r'\end{description}')
	#	print('\n')

	for match in re.finditer(r'/\*\*.*?\n(?P<doc>.*?)\*/\n(void|int|double)\*? (?P<func>\w+?)\(', dat, re.DOTALL):
		funcname = match.group('func')
		doc = '\n'.join(x.strip('* ') for x in match.group('doc').splitlines()).strip()

		if doc and doc.splitlines()[-1].startswith('return: '):
			_return = ':'.join(doc.splitlines()[-1].split(':')[1:]).strip()
			doc = '\n'.join(doc.splitlines()[:-1])
		else:
			_return = None

		if doc and all(':' in x for x in doc.split('\n\n')[-1].splitlines()):
			description = '\n\n'.join(doc.split('\n\n')[:-1])
			params = tuple((x.split(':')[0].strip(), ':'.join(x.split(':')[1:]).strip()) for x in doc.split('\n\n')[-1].splitlines())
		else:
			description = doc
			params = None


		print(r'\subsection{' + match.group('func').replace(r'_', r'\_') + '}')
		print(description.replace(r'_', r'\_'))

		if params is not None:
			print('\n')
			print(r'\subsubsection{引数}')
			print(r'\begin{description}')
			print('\n'.join('\t\\item[' + x[0].replace(r'_', r'\_') + r'] ' + x[1].replace(r'_', r'\_') for x in params))
			print(r'\end{description}')

		if _return:
			print('\n')
			print(r'\subsubsection{戻り値}')
			print(_return.replace(r'_', r'\_'))

		print('\n')
