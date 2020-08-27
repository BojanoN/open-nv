def replace_tokens(document):
    """
        Replaces all tokens in the document. Should be
        called after loading the document.

        Parameters:
            document (xml.Document) -- The document.
        
        Returns:
            document (xml.Document) -- The document with all tokens replaced.
    """
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
    return document