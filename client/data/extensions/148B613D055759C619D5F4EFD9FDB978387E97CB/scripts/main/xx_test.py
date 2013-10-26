#winpdb
#import rpdb2; rpdb2.start_embedded_debugger("osiris")

import osiris
'''
class PyFoo(osiris.Foo):
#	def __init__(self):
#		osiris.Foo.__init__(self)
#		self.flag = False
								
	def virtual_public(self):
		osiris.LogManager.instance().log("PyFoo::virtual_public")		
		osiris.Foo.virtual_public(self)
		
	def virtual_protected(self):
		osiris.LogManager.instance().log("PyFoo::virtual_protected")		
		osiris.Foo.virtual_protected(self)
		
	def say_hello(self):
		osiris.LogManager.instance().log("foo hello")
		
def say_hello():
	osiris.LogManager.instance().log("global hello")

def main(args):
	foo = PyFoo()	
	foo.call_protected()
	foo.call_public()
	
	session = args[0]
	session.transmitHtml("<html><body>Yo!</body></html>", osiris.httpStatusOK)
'''

def main(args):
	extensions = osiris.ExtensionsSystem.instance().extensions
					
	output = ""
	for k in extensions.keys():
		extension = extensions[k]
		output += extension.id.getString()
					
	session = args[0]
	session.transmitHtml("<html><body>" + output + "</body></html>", osiris.httpStatusOK)
	