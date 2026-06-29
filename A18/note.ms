.TL
A parser for the C programming language
.AU
A. Rabusov
.DT Jun, 26, 2026
.AB
This notes provides insites on my parser implementation for the C programming
language.
.AE
.NH
Grammar
.LP
While one could take an existing parser, or generate one using
.CW "yacc",
I would like to focus on clarity of the implementation, rather on it's
efficiency or other metrics. A from-scratch implementation suits best to this
goal.
.PP
I follow the grammar description in [1] and parsing techniques from [2].
We start our exericse with
.I "primary"
and
.I "lvalue"
nonterminals and substitute the remaining with terminals. I'll repeat the
grammar here:
.DS
\fIprimary:
	\0identifier
	\0constant
	\0string
	\0\fC( \fIexpression \fC)
	\0\fIprimary \fC( \fIexpression-list\dopt\u \fC)
	\0\fIprimary \fC[ \fIexpression \fC]\fI
	\0\fIlvalue \fC. \fIidentifier
	\0primary \fC-> \fIidentifier

\fIlvalue:
	\0identifier
	\0primary \fC[ \fIexpression \fC]
	\0\fIlvalue \fC. \fIidentifier
	\0primary \fC-> \fIidentifier
	\0\fC* \fIexpression
	\0\fC( \fIlvalue \fC)
.DE
.PP
\fRWe could shorten notatin and use capitalized letters for nonterminals. The
same grammar would be reformulated like this:
.DS
\fI P \[->]	i | c | s | \fC(\fI e \fC)\fI
\fI	| P \fC(\fI EL\dopt\u \fC)\fI
\fI	| P \fC[\fI e \fC]\fI
\fI	| L \fC.\fI i
\fI	| P \fC->\fI i
.DE
.DS
\fI L\[->] i \fI 
\fI	| P \fC[\fI e \fC]\fI
\fI	| L \fC.\fI i
\fI	| P \fC->\fI i
\fI	| \fC * \fI e
\fI	| \fC(\fI L \fC )
.DE
\fR
where \fI i, c, s, e \fR are essentialy terminals alongside with operators and
delimeters, \fIlvalue\fR is denoted as \fIL\fR, and \fIprimary\fR is denoted as
\fIP\fR.
.PP
The problem with this grammar is left recursion (rule No. 3 for \fIlvalue\fR
and rules 5, 6, and 8 for \fIprimary\fR), to eliminate it we'll use the
technique from [2], Sec. 2.5, p.50.
.NH
Predictive parser
.LP
.IP [1]
Brian W. Kernighan and Dennis M. Ritchie: The C Programming Language;
PRENTICE-HALL, INC., Englewood Cliffs, New Jersey (1978), (APPENDIX A, 18, pp.
214-219)
.IP [2]
Alfred V. Aho, Ravi Sethi, and Jeffrey D. Ullman: Compilers: Principles,
Techniques, and Tools; ADDISON-WESLEY PUBLISHING COMPANY (1986), (Chapter 2, 4)
