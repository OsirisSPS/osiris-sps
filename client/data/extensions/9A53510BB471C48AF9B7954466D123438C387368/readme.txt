------------------------------------------------------
Before add a service:
- Enable "HtmlGeneric".
- Open a topic in the "Osiris Official - Tester - Html Snipper", paste each syntax you want to support, check that works.

To add a service,
- Use this service: http://www.clodo.it/projects/regextest/
- Adjust the HTML to remove any carriage return.
- Paste in "Sample text" the HTML
- Paste in "Regular Expression" the same HTML
- Click "Quote"
- Copy the "Output" overriding "Regular Expression"
- Click "Test Match". If success, all it's ok.

- Now, Insert in the regular expression more flexibility of params.

Look the file "macros.py", you can find some macros.
Use it in the regular expression "not quoted" form, for example    \<img width\="[0-9*]"

Optional:
If you enable "Debug Mode" for a snippet (look "snippets.by", it's done by adding "debug" attribute), you can find some logs in Osiris log.
The system will suggest you a "regular expression macro edition", use i

Utils regex:
[0-9]* 								- Number only. Use this in value attributes
[a-zA-Z]* 						- String only. Use this in value attributes
[0-9a-zA-Z]* 					- Any alphanumeric ascii word
[0-9a-zA-Z\s\-_]*		 	- Any alphanumeric ascii string
[^<>]*								- Anything plain text, not html tag.
[^"'<>]*							- Anything plain text until a " is reached. Use this in the final part of an url, normally after ?.
[^"'<>\(\)*"] 				- Used in "style" attributes
['\"] 								- Match ' or ", opening and closing attribute.

- Use ofter "Test Match" to check if isn't mistakes.
- Click "Quote For String", and copy the "Output" in the Python Class inside "snippets.py".
- Look about "self.items.append(" and append/register your class.

(old) To debug old service:
- Copy the string with the regex in "Regular Expression"
- Click "De-Quote from String"
- Copy the "Output" to "Regular Expression"
- Follow the guide above

To debug old service:
- Activate the snippet "Debug mode"
- Copy the regex used from Osiris log to the web service.


Notes:
- HTML are altered (macro replace) before matching regex. Set "debug = true" in the snippets class to view useful informations in Osiris log.

------------------------------------------------------

Code notes:

- Normally we add the HTML to the DOM after confirmation. This don't work with some html, generally because their are in listening on window.onLoad events.
  For that, exists a "reload" boolean: if "false", html are injected in DOM. If "true", a validation value are sended in http/post, to be used during the next load.

