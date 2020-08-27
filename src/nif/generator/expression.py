import re
import string

def get_value(string_value):
        int_value = 0
        parts = [int(value) for value in string_value.split(sep='.')]
        for part, offset in zip(parts, [24, 16, 8, 0]):
            int_value += part << offset
        return int_value

class Expression:

    operators = ['==', '!=', '>=', '<=', '&&', '||', '&', '|', '-', '+', '>', '<', '/', '*']

    def __init__(self, expr_str, name_filter=None):
        #if expr_str == '!(BS_Header_BS_Version <= 16)':
        #    breakpoint()
        self._code = expr_str
        left, self._op, right = self._partition(expr_str)
        self._left = self._parse(left, name_filter)
        if right:
            self._right = self._parse(right, name_filter)
        else:
            self._right = ''

    @classmethod
    def _parse(cls, expr_str, name_filter=None): # type: (str, Callable[[str], str]) -> str
        """Returns an Expression, string, or int, depending on the
        contents of <expr_str>."""
        # brackets or operators => expression
        if ("(" in expr_str) or (")" in expr_str):
            return Expression(expr_str, name_filter)
        for op in cls.operators:
            if expr_str.find(op) != -1:
                return Expression(expr_str, name_filter)

        mver = re.compile("[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+")
        iver = re.compile("[0-9]+")
        # try to convert it to an integer
        try:
            if mver.match(expr_str):
                return "0x%08X"%(get_value(expr_str))
            elif iver.match(expr_str):
                return str(int(expr_str))
        except ValueError:
            pass
        # failed, so return the string, passed through the name filter
        return name_filter(expr_str) if name_filter else expr_str

    @staticmethod
    def _scanBrackets(expr_str, fromIndex=0):
        """Looks for matching brackets.

        >>> Expression._scanBrackets('abcde')
        (-1, -1)
        >>> Expression._scanBrackets('()')
        (0, 1)
        >>> Expression._scanBrackets('(abc(def))g')
        (0, 9)
        >>> s = '  (abc(dd efy 442))xxg'
        >>> startpos, endpos = Expression._scanBrackets(s)
        >>> print s[startpos+1:endpos]
        abc(dd efy 442)
        """
        startpos = -1
        endpos = -1
        scandepth = 0
        for scanpos in range(fromIndex, len(expr_str)):
            scanchar = expr_str[scanpos]
            if scanchar == "(":
                if startpos == -1:
                    startpos = scanpos
                scandepth += 1
            elif scanchar == ")":
                scandepth -= 1
                if scandepth == 0:
                    endpos = scanpos
                    break
        else:
            if startpos != -1 or endpos != -1:
                raise ValueError("expression syntax error (non-matching brackets?)")
        return (startpos, endpos)

    @classmethod
    def _partition(cls, expr_str):
        """Partitions expr_str. See examples below.

        >>> Expression._partition('abc || efg')
        ('abc', '||', 'efg')
        >>> Expression._partition('abc||efg')
        ('abc', '||', 'efg')
        >>> Expression._partition('abcdefg')
        ('abcdefg', '', '')
        >>> Expression._partition(' abcdefg ')
        ('abcdefg', '', '')
        >>> Expression._partition(' (a | b) & c ')
        ('a | b', '&', 'c')
        >>> Expression._partition('(a | b)!=(b&c)')
        ('a | b', '!=', 'b&c')
        >>> Expression._partition('(a== b) &&(( b!=c)||d )')
        ('a== b', '&&', '( b!=c)||d')
        """
        # check for unary operators
        if expr_str.strip().startswith('!'):
            return expr_str.lstrip(' !'), '!', None
        lenstr = len(expr_str)
        # check if the left hand side starts with brackets
        # and if so, find the position of the starting bracket and the ending
        # bracket
        left_startpos, left_endpos = cls._scanBrackets(expr_str)
        if left_startpos >= 0:
            # yes, it is a bracketted expression
            # so remove brackets and whitespace,
            # and let that be the left hand side
            left_str = expr_str[left_startpos+1:left_endpos].strip()

            # the next token should be the operator
            # find the position where the operator should start
            op_startpos = left_endpos+1
            while op_startpos < lenstr and expr_str[op_startpos] == " ":
                op_startpos += 1
            if op_startpos < lenstr:
                # to avoid confusion between && and &, and || and |,
                # let's first scan for operators of two characters
                # and then for operators of one character
                for op_endpos in range(op_startpos+1, op_startpos-1, -1):
                    op_str = expr_str[op_startpos:op_endpos+1]
                    if op_str in cls.operators:
                        break
                else:
                    breakpoint()
                    raise ValueError("expression syntax error: expected operator at '%s'"%expr_str[op_startpos:])
            else:
                return cls._partition(left_str)
        else:
            # it's not... so we need to scan for the first operator
            for op_startpos, ch in enumerate(expr_str):
                if ch == ' ':
                    continue
                if ch == '(' or ch == ')':
                    breakpoint()
                    raise ValueError("expression syntax error: expected operator before '%s'"%expr_str[op_startpos:])
                # to avoid confusion between && and &, and || and |,
                # let's first scan for operators of two characters
                for op_endpos in range(op_startpos+1, op_startpos-1, -1):
                    op_str = expr_str[op_startpos:op_endpos+1]
                    if op_str in cls.operators:
                        break
                else:
                    continue
                break
            else:
                # no operator found, so we are done
                left_str = expr_str.strip()
                op_str = ''
                right_str = ''
                return left_str, op_str, right_str
            # operator found! now get the left hand side
            left_str = expr_str[:op_startpos].strip()

        return left_str, op_str, expr_str[op_endpos+1:].strip()

    def __str__(self): # type: () -> str
        """Reconstruct the expression to a string."""
        if type(self._left) == str:
            if not self._left.isdigit():
                self._left = '_'.join(self._left.split(sep=' '))
        left = str(self._left)
        if not self._op:
            return left
        if type(self._right) == str:
            if not self._right.isdigit():
                self._right = '_'.join(self._right.split(sep=' '))
        right = str(self._right)
        return left + ' ' + self._op + ' ' + right
    
    def eval(self, data=None):
        """Evaluate the expression to an integer."""

        if isinstance(self._left, Expression):
            left = self._left.eval(data)
        elif isinstance(self._left, str) and self._left.isdecimal():
            left = int(self._left)
        elif isinstance(self._left, str) and all(c in string.hexdigits for c in self._left[2:]):
            left = int(self._left[2:], 16)
        elif isinstance(self._left, str):
            left = getattr(data, self._left) if self._left != '""' else ""
        elif isinstance(self._left, str):
            left = int(self._left)
        else:
            assert isinstance(self._left, int) # debug
            left = self._left

        if not self._op:
            return left

        try:
            if self._right == '':
                right = None
            elif isinstance(self._right, Expression):
                right = self._right.eval(data)
            elif isinstance(self._right, str) and self._right.isdecimal():
                right = int(self._right)
            elif isinstance(self._right, str) and all(c in string.hexdigits for c in self._right[2:]):
                right = int(self._right[2:], 16)
            elif isinstance(self._right, str):
                right = getattr(data, self._right) if self._right != '""' else ""
            elif isinstance(self._right, str):
                right = int(self._right)
            else:
                assert isinstance(self._right, int) # debug
                right = self._right
        except ValueError:
            breakpoint()

        if self._op == '==':
            return int(left == right)
        elif self._op == '!=':
            return int(left != right)
        elif self._op == '>=':
            return int(left >= right)
        elif self._op == '<=':
            return int(left <= right)
        elif self._op == '>':
            return int(left > right)
        elif self._op == '<':
            return int(left < right)
        elif self._op == '&&':
            return int(left and right)
        elif self._op == '||':
            return int(left or right)
        elif self._op == '&':
            return left & right
        elif self._op == '|':
            return left | right
        elif self._op == '-':
            return left - right
        elif self._op == '+':
            return left + right
        elif self._op == '/':
            return left / right
        elif self._op == '*':
            return left * right
        elif self._op == '!':
            return not left
        else:
            raise NotImplementedError("expression syntax error: operator '" + self._op + "' not implemented")
