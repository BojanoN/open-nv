import xml.dom
import xml.dom.minidom

if __name__ == '__main__':
	document = xml.dom.minidom.parse('./nif.xml')
	tokens = document.getElementsByTagName('token')
	for token in tokens:
		replacer_name = token.getAttribute('name')
		targets = token.getAttribute('attrs').split()
		replacements = document.getElementsByTagName(replacer_name)
		replacement_map = dict()
		for replacement in replacements:
			replacement_map[replacement.getAttribute('token')] = replacement.getAttribute('string')

		fields = document.getElementsByTagName('field')
		fields.extend(document.getElementsByTagName('compound'))
		fields.extend(document.getElementsByTagName('niobject'))
		fields.extend(document.getElementsByTagName('enum'))
		fields.extend(document.getElementsByTagName('bitfield'))
		fields.extend(document.getElementsByTagName('bitflags'))
		fields.extend(document.getElementsByTagName('enum'))
		for field in fields:
			for target in targets:
				if field.hasAttribute(target):
					value = field.getAttribute(target)
					for old, new in replacement_map.items():
						value = value.replace(old, new)
					field.setAttribute(target, value)
	with open('./new_nif.xml', 'w', encoding='utf-8') as f:
		document.documentElement.writexml(f, addindent="\t")
