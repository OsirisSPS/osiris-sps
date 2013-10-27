#winpdb
#import rpdb2; rpdb2.start_embedded_debugger("osiris")

# Questo script va invocato dalla versione console altrimenti lancia delle eccezioni

import sys
import osiris
import epydoc.cli

def main(args):
	session = args[0]
	
	#sys.argv.append("--debug")
	sys.argv.append("--html")	
	sys.argv.append("osiris")
	sys.argv.append("osirislib")			
	
	sys.argv.append("--graph")
	sys.argv.append("all")
	sys.argv.append("--dotpath")
	sys.argv.append("D:/store/utils/graphviz/bin/dot.exe")
	
	sys.argv.append("--url")
	sys.argv.append("http://www.osiris-sps.org")	
	
	sys.argv.append("-o")
	sys.argv.append("../../../share/plugins/AF9A4C281070FDB0F34CF417CDB168AB38C8A388/doc")
	
	epydoc.cli.cli()
		
	session.redirect("/python");