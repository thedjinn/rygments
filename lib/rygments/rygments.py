from pygments import highlight
from pygments.lexers import get_lexer_by_name
from pygments.formatters import get_formatter_by_name

def rygmentize(code, lexer, formatter):
    lexer = get_lexer_by_name(lexer)
    formatter = get_formatter_by_name(formatter)
    return highlight(code, lexer, formatter)
