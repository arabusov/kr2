.char \[*a] \f[S]\[*a]\fP
.char \[*b] \f[S]\[*b]\fP
.char \[*g] \f[S]\[*g]\fP
.char \[*e] \f[S]\[*e]\fP
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
efficiency or other metrics. A from-scratch implementation suits best for this
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
	\0\fC(\fIexpression \fC)
	\0\fIprimary \fC(\fIexpression-list\dopt\u \fC)
	\0\fIprimary \fC[\fIexpression \fC]\fI
	\0\fIlvalue \fC.\fIidentifier
	\0primary \fC->\fIidentifier\fR
.DE
.DS
\fIlvalue:
	\0identifier
	\0primary \fC[ \fIexpression \fC]
	\0\fIlvalue \fC. \fIidentifier
	\0primary \fC-> \fIidentifier
	\0\fC* \fIexpression
	\0\fC( \fIlvalue \fC)\fR
.DE
.PP
We could shorten this notatin by using capitalized italics for nonterminals
and monospace or lowercase italics for terminals, reformulating the same
grammar will be in the following manner:
.2C
.DS
\fIP \[->]		i  | c | s | \fC(\fIe \fC)\fI
	|	P \fC(\fIEL\dopt\u \fC)\fI
	|	P \fC[\fIe \fC]\fI
	|	L \fC. \fIi
	|	P \fC-> \fIi
.DE
.DS
\fIL\[->]		i  | \fC*\fIe
	|	P \fC[\fIe \fC]\fI
	|	L \fC. \fIi
	|	P \fC-> \fIi
	|	\fC(\fIL \fC)\fR
.DE
.1C
where \fIi, c, s, e \fR are essentialy terminals alongside with operators and
delimeters, \fIlvalue\fR is denoted as \fIL\fR, and \fIprimary\fR is denoted as
\fIP\fR.
.PP
This grammar contains immediate left recursion (rule No. 3 for \fIlvalue\fR
and rules 5, 6, and 8 for \fIprimary\fR) which causes an infinite recursion in a
straightforward implementation of the predictive parser.  To eliminate the
immediate recursion we could use a substitution from [2], Sec. 2.5, p. 50:
.DS
\fIA \[->] \[*g] R
R \[->] \[*a] R | \[*b] R | \[*e]\fR,
.DE
where \fIA\fR is a non-terminal with the productions
.DS
\fIA \[->] A\[*a] | A\[*b]\fR.
.DE
.PP
We eliminate left recursion using Algorithm 4.1 from [2], Sec.  4.3, p. 177\u*\d.
.FS *
It seems there is a couple of typos on the p. 177. The line 8 should read
\*[Q]Grammar \fIG\fR with no cycles or \[*e] production,\*[U] and the line 16
should read \*[Q]the production \fIA\di\u\[->]\[*d]\d1\u\[*g]  |
\[*d]\d2\u\[*g]  | ...  | \[*d]\dk\u\[*g].\*[U]
.FE
First, we order the non-terminals with being \fIL\fR the first and \fIP\fR the
second. The \fIL\fR production need to undergo the elimination of the immediate
left recursion:
.DS
\fIL	\[->]	i RL | \fC*\fIe RL
	|	P \fC[\fIe \fC] \fIRL
	|	P \fC-> \fIi RL
	|	\fC(\fIL \fC)\fI RL\fR
\fIRL	\[->]	\fC. \fIi RL | \[*e]
.DE
.PP
Then, we need to eliminate the left recursion for \fIP\fR.  First, we replace
the only production of \fIP\fR containing \fIL\fR by expanding \fIL\fR:
.DS
\fIP	\[->]	i  | c | s | \fC(\fIe \fC)\fI
	|	P \fC(\fIEL\dopt\u \fC)\fI
	|	P \fC[\fIe \fC]\fI
	|	i \fC\. \fIi | \fC*\fIe \fC\. \fIi
	|	P \fC[\fIe \fC]\fI \fC\. \fIi
	|	L \fC\. \fIi \fC\. \fIi
	|	P \fC-> \fIi \fC. \fIi
	|	\fC(\fIL \fC)\fI \fC. \fIi\fR
	|	P \fC-> \fIi

.DE
.PP
We extend the \fIP\fR non-terminal productions with two additional non-terminals
and their productions. We use the \fIRP\fR symbol for the \*[Q]rest\*[U] of the
\fIP\fR and \fIRL\fR for the \*[Q]rest\*[U] of \fIL\fR. Additionally, we add an
extra non-terminal \fIPt\fR just to shorten the notation as its productions are
terminals only.
.2C
.DS
\fIPt \[->]	i | c | s | \fC(\fIe \fC)\fI
\fIPR \[->]	\fC(\fIEL\dopt\u \fC)\fI
\fI	| \fC[\fIe \fC]\fI
\fI	| P \fC->\fIi
\fI	| L \fC.\fIi
.DE
.DS
\fIP \[->]	i | c | s | \fC(\fIe \fC)\fI
\fI	| P \fC(\fIEL\dopt\u \fC)\fI
\fI	| P \fC[\fIe \fC]\fI
\fI	| L \fC.\fIi
\fI	| P \fC->\fIi
.DE
.1C


.NH
Literature
.LP
.IP [1]
Brian W. Kernighan and Dennis M. Ritchie: The C Programming Language;
PRENTICE-HALL, INC., Englewood Cliffs, New Jersey (1978), (APPENDIX A, 18, pp.
214-219)
.IP [2]
Alfred V. Aho, Ravi Sethi, and Jeffrey D. Ullman: Compilers: Principles,
Techniques, and Tools; ADDISON-WESLEY PUBLISHING COMPANY (1986), (Chapter 2, 4)
