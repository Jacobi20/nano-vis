% ``graphicon'' LaTeX class.
%
% ------------ identification --------------
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{graphicon}

% ------------ declaration of options --------------

\ProcessOptions
\PassOptionsToClass{twocolumn}{article}
\PassOptionsToClass{9pt}{article}
\PassOptionsToClass{letter}{article}

% ------------ package loading --------------

\LoadClass{article}

% ------------ main code --------------

% text area
\setlength{\textheight}{9.250in}
\setlength{\textwidth}{7.000in}
\setlength{\voffset}{-1.000in}
\setlength{\hoffset}{-0.250in}
\setlength{\columnsep}{0.330in}

% paragraphs
\setlength{\parindent}{0in}
\setlength{\parskip}{4pt}

% Letter Size
\setlength{\paperheight}{11.000in}
\setlength{\paperwidth}{8.500in}

%\setlength{\oddsidemargin}{-0.000in}
%\setlength{\evensidemargin}{-0.000in}
%\setlength{\topmargin}{-0.750in}
%\setlength{\headheight}{0.200in}
%\setlength{\headsep}{0.250in}
%\setlength{\footskip}{0.500in}

\flushbottom

% no page numbers - they are added in production

\pagestyle{empty}

% Font sizes

\renewcommand\normalsize{%
    \@setfontsize\normalsize\@ixpt\@xpt
    \abovedisplayskip 9\p@ \@plus2\p@ \@minus4\p@
    \abovedisplayshortskip \z@ \@plus3\p@
    \belowdisplayshortskip 6\p@ \@plus3\p@ \@minus3\p@
    \belowdisplayskip \abovedisplayskip
    \let\@listi\@listI}

\renewcommand\small{%
    \@setfontsize\small\@viipt\@ixpt
    \abovedisplayskip 8.5\p@ \@plus3\p@ \@minus4\p@
    \abovedisplayshortskip \z@ \@plus2\p@
    \belowdisplayshortskip 4\p@ \@plus2\p@ \@minus2\p@
    \def\@listi{\leftmargin\leftmargini
                \topsep 4\p@ \@plus2\p@ \@minus2\p@
                \parsep 2\p@ \@plus\p@ \@minus\p@
                \itemsep \parsep}%
    \belowdisplayskip \abovedisplayskip}

\renewcommand\footnotesize{%
    \@setfontsize\footnotesize\@viiipt{9.5}%
    \abovedisplayskip 6\p@ \@plus2\p@ \@minus4\p@
    \abovedisplayshortskip \z@ \@plus\p@
    \belowdisplayshortskip 3\p@ \@plus\p@ \@minus2\p@
    \def\@listi{\leftmargin\leftmargini
                \topsep 3\p@ \@plus\p@ \@minus\p@
                \parsep 2\p@ \@plus\p@ \@minus\p@
                \itemsep \parsep}%
    \belowdisplayskip \abovedisplayskip}

\renewcommand\scriptsize{\@setfontsize\scriptsize\@viipt\@viiipt}

\renewcommand\tiny{\@setfontsize\tiny\@vpt\@vipt}

\renewcommand\large{\@setfontsize\large\@xpt\@xiipt}

\renewcommand\Large{\@setfontsize\Large\@xiipt{14}}

\renewcommand\LARGE{\@setfontsize\LARGE\@xivpt{18}}

\renewcommand\huge{\@setfontsize\huge\@xviipt{22}}

\renewcommand\Huge{\@setfontsize\Huge\@xxpt{25}}
    \selectfont

% Section commands

\renewcommand\section{\@startsection {section}{1}{\z@}%
                {-1.0ex}%
                {1.0ex}%
                {\reset@font\large\sffamily\bfseries\uppercase}}
\renewcommand\subsection{\@startsection{subsection}{2}{\z@}%
                {-1.0ex}%
                {1.0ex}%
                {\reset@font\large\sffamily\bfseries}}
\renewcommand\subsubsection{\@startsection{subsubsection}{3}{\z@}%
                {-1.0ex}%
                {1.0ex}%
                {\reset@font\large\sffamily\itshape}}
\renewcommand\paragraph{\@startsection{paragraph}{4}{\z@}%
                {2.05ex}%
                {-1em}%
                {\reset@font\normalsize\sffamily\bfseries}}
\renewcommand\subparagraph{\@startsection{subparagraph}{5}{\parindent}%
                {3.25ex \@plus1ex \@minus .2ex}%
                {-1em}%
                {\reset@font\normalsize\sffamily\bfseries}}
\def\thesection{\arabic{section}.} 
\def\thesubsection{\arabic{section}.\arabic{subsection}} 
               
\newcommand{\graphicon@empty}{}

\newcommand{\graphicon@affiliation}{}
\newcommand{\affiliation}[1]{\renewcommand{\graphicon@affiliation}{#1}}

\newcommand{\graphicon@format}{}
\newcommand{\format}[1]{\renewcommand{\graphicon@format}{#1}}

\newcommand{\graphicon@teaser}{}
\newcommand{\teaser}[1]{\renewcommand{\graphicon@teaser}{#1}}

\newlength{\titlespace}
\setlength{\titlespace}{0.25in}

\newlength{\teaserspace}
\setlength{\teaserspace}{0.25in}

    \renewcommand{\@maketitle}{%
        \begin{center}%
            {\LARGE\sffamily\bfseries \@title \par}%
            \vspace{1\baselineskip}%
            \large
            \begin{tabular}[t]{c}%
                \@author
            \end{tabular}\par%
            \ifx\graphicon@affiliation\graphicon@empty \else%
                \vspace{0.5\baselineskip}%
                \graphicon@affiliation\par%
            \fi%
            \vspace{\titlespace}%
            \ifx\graphicon@teaser\graphicon@empty \else%
                \begingroup%
                    \def\@captype{figure}%
                    \graphicon@teaser%
                \endgroup\par%
                \vspace{\teaserspace}%
            \fi%
        \end{center} \par%
    }

\let\graphicon@origmaketitle\maketitle
\let\graphicon@origand\and
\renewcommand{\maketitle}{%
    \let\graphicon@title\@title%
    \let\graphicon@author\@author%
    \graphicon@origmaketitle%
    \thispagestyle{empty}%
}

\newtoks\graphicon@abs

\newcommand{\keywordlist}{%
    \vspace{0.5\baselineskip}%
    \par\noindent \textit{\textbf{Keywords: } \graphicon@keywords .}%
}
\newenvironment{CRcatlist}{%
    \vspace{0.5\baselineskip}%
    \par\noindent \textbf{CR Categories: }%
}{}

\newcommand{\CRcat}[4]{%
    #1 [#2]%
    \ifx#3\graphicon@empty \else : #3\fi%
    \ifx#4\graphicon@empty \else ---#4\fi%
}

% leave a 1.5 inch space at the bottom of the left column
% on the first page for the copyright block.

\renewcommand{\footnoterule}{%
    \kern-3pt
    \hrule width 0.45\columnwidth
    \kern 2.6pt}

\newcommand{\graphicon@keywords}{}
\newcommand{\keywords}[1]{\renewcommand{\graphicon@keywords}{#1}}

\def\fnum@figure{{\bf Figure\ \thefigure}}
\def\fnum@table{{\bf Table \thetable}}

\def\thebibliography#1{\section{REFERENCES}\list
 {[\arabic{enumi}]}{\settowidth\labelwidth{[#1]}\leftmargin\labelwidth
 \advance\leftmargin\labelsep
 \usecounter{enumi}}
 \def\newblock{\hskip .11em plus .33em minus .07em}
 \sloppy\clubpenalty4000\widowpenalty4000
 \sfcode`\.=1000\relax}
\let\endthebibliography=\endlist

\endinput

% End of file 'acmsiggraph.cls'.

