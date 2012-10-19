#!/usr/bin/python

import sys
import re
from subprocess import Popen, PIPE

pat_header = re.compile('^cu v([^,]+), ([^,]*), ([0-9]{4}-[0-9]{2}-[0-9]{2})$')

class Section(object):
    def __init__(self, version, author, date):
        self.version = str(version)
        self.author  = str(author)
        self.date    = str(date)
        self.changelog = ''
    
    def addLine(self, line):
        line = str(line)
        if len(line) > 0:
            line = '  ' + line

        self.changelog += line + '\n'

    def pr(self):
        print(self._formatHeader())
        print(self.changelog)
        print(self._formatFooter())

    def _formatHeader(self):
        h = 'cunittest (' + self.version + ') UNRELEASED; urgency=low'
        return h
    def _formatFooter(self):
        env = { 'LANG' : 'en_US.UTF-8' }
        date_cmd = ['date', '-d' + self.date, '+%a, %d %b %Y %H:%M:%S %z' ]
        pipe = Popen(date_cmd, env = env, stdout = PIPE)
        sin = pipe.communicate()[0]
        date = str(sin.strip())

        h = ' -- ' + self.author + '  ' + date
        return h

def main():
    sec = None
    for line in sys.stdin:
        line = line.strip()

        match = pat_header.match(line)
        if match is not None:
            if sec is not None:
                sec.pr()

            version = match.group(1)
            author  = match.group(2)
            date    = match.group(3)

            sec = Section(version, author, date)
        else:
            if sec is not None:
                sec.addLine(line)

    if sec is not None:
        sec.pr()

if __name__ == '__main__':
    main()
