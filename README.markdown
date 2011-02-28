# Rygments

Rygments is a Ruby wrapper for the awesome Python based Pygments syntax
highlighter.

Rygments differs from other implementations in that it embeds a full
Python interpreter for the Pygments Python files as a Ruby C extension.
This enables Rygments to reach a level of performance that is equal to
invoking Pygments from a native Python application, and without the overhead of forking
a new process. This difference in performance really pays off when
you are highlighting lots of smaller code fragments, e.g. for blog
posts or documentation generators such as Rocco.

Compared to Albino (another popular Pygments Ruby wrapper) Rygments is over 30% faster when using it to highlight all files from the Ruby 1.9.2 source code.

# Installation

## Pygments

First, you need to make sure Pygments and the Python development headers are installed. How this can be done
depends on your platform.

### Debian and Ubuntu Linux

    sudo apt-get install python-dev python-pygments

### OSX (Homebrew)

    brew install pip && pip install pygments

The Python development headers are supplied with XCode.

### OSX (Distutils)

    sudo easy_install Pygments

The Python development headers are supplied with XCode.

## Rygments

After installing Pygments you can install the Rygments gem via Rubygems:

    gem install rygments

and you're good to go!

# Usage

Usage is easy. First you have to require the Rygments gem:

    require "rygments"

Rygments then gives you two class methods that enable you to highlight code from wherever you want.

To highlight source code from a string use:

    Rygments.highlight_string(source_code, lexer, formatter)

and to highlight source code from a file use:

    Rygments.highlight_file(filename, lexer, formatter)

Both methods return a string containing the highlighted code.

The _lexer_ is a lowercase string specifying the programming language you want
to highlight for, e.g. `ruby` or `sass`.

The _formatter_ is a lowercase string specifying the kind of output you want, 
e.g. `html` or `latex`.

A full list of lexers and formatters is available on the Pygments
website.

# Compatibility

Rygments has been tested and is known to work on Ruby Enterprise Edition 1.8.7-2011.01 and Ruby 1.9.2.

It has been tested on the following platforms:

 * OSX Snow Leopard (10.6.6)
 * Ubuntu Linux (8.04 and 10.10)

If you used it with success on a different Ruby implementation or platform then please let me know so I can extend this list.

# License

Copyright (c) 2011 Emil Loer <http://emilloer.com>

Permission  is  hereby granted, free of charge, to any person obtaining a copy of  this  software  and  associated  documentation files  (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is  furnished to do so, subject to the following conditions:

The  above  copyright  notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF  ANY  KIND, EXPRESS  OR  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE  AND  NON-INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER  IN  AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN  THE SOFTWARE.
