import osiris
import weakref				
				
def main(args):
	session = args[0]
	session.transmitHtml("<html><body>Yo!</body></html>", osiris.httpStatusOK)
