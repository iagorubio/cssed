// Scintilla source code edit control
/** @file LexCSS.cxx
 ** Lexer for Cascade Style Sheets
 ** Written by Jakub Vrána
 ** *******************************
 ** Modified by Iago Rubio for cssed
 ** Tue Dec 16 13:32:20 2003
 ** *******************************
 **
 **/
// Copyright 1998-2002 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>

#include "Platform.h"

#include "PropSet.h"
#include "Accessor.h"
#include "StyleContext.h"
#include "KeyWords.h"
#include "Scintilla.h"
#include "SciLexer.h"

static inline bool
IsAWordChar (const unsigned int ch)
{
	return (isalnum (ch) || ch == '-' || ch == '_' || ch >= 161);
}

inline bool
IsCssOperator (const char ch)
{
	if (!isalnum (ch)
	    && (ch == '{' || ch == '}' || ch == ':' || ch == '#'|| ch == ',' || ch == ';'
		|| ch == '.' || ch == '!' || ch == '@'))
		return true;
	return false;
}
// added
static inline bool
IsValidFunctionParamChar (const unsigned int ch)
{
	return (isalnum (ch) || ch == ',' || ch == '?' || ch == '\t'|| ch == '-'|| ch == '_'
		|| ch == ' ' || ch == '.' || ch == '(' || ch == ')' || ch == ':'|| ch == '%'
		|| ch == '&'|| ch == '='|| ch == '/' || ch == '\'' || ch == '\"' || ch >= 161);
}

inline bool
IsValidHexaChar( const char ch )
{
	if( ch >=0 && ch <='f' )
			return true;
	return false;
}

inline bool
IsValidHexaString( char* ch )
{
	char *p;
	char *end;
	unsigned int len;
	
	len = strlen (ch);
	p = ch;
	end = ch + len;
	
	if( len != 3 && len != 6 )
		return false;
	
	while( p <= end )
	{
		if( !IsValidHexaChar( *p ))
				return false;
		p++;		
	}
	return true;	
}

inline bool
IsNum (char *ch)
{
	char *p;
	unsigned int len;
	unsigned int i = 0;
	unsigned int cc = 0;

	len = strlen (ch);
	p = ch;

	while (i < len)
	{
		if (!isdigit (*p))
		{
			if (*p == '.')
				cc++;
			else
				return false;
		}
		i++;
		p++;
		if (cc > 1)
			return false;
	}
	return true;
}

static void
ColouriseCssDoc (unsigned int startPos, int length, int initStyle,
		 WordList * keywordlists[], Accessor & styler)
{
	WordList & validProperties = *keywordlists[0];	
	WordList & pseudoClasses = *keywordlists[1];
	WordList & validValues = *keywordlists[2];
	WordList & commonFunctions = *keywordlists[3];
	WordList & validUnits = *keywordlists[4];
	WordList & namedColors = *keywordlists[5];
	StyleContext sc (startPos, length, initStyle, styler);

	int lastState = -1;	
	int lastStateC = -1;
	int op = ' ';		
	

	for (; sc.More (); sc.Forward ())
	{			

		if (sc.state == SCE_CSS_COMMENT && sc.Match ('*', '/'))
		{
			if (lastStateC == -1)
			{
				unsigned int i = startPos;
				for (; i > 0; i--)
				{
					if ((lastStateC =
					     styler.StyleAt (i - 1)) !=
					    SCE_CSS_COMMENT)
					{
						if (lastStateC ==
						    SCE_CSS_OPERATOR)
						{
							op = styler.
								SafeGetCharAt
								(i - 1);
							while (--i)
							{
								lastState =
									styler.
									StyleAt
									(i -
									 1);
								if (lastState
								    !=
								    SCE_CSS_OPERATOR
								    &&
								    lastState
								    !=
								    SCE_CSS_COMMENT)
									break;
							}
							if (i == 0)
								lastState =
									SCE_CSS_DEFAULT;
						}
						break;
					}
				}
				if (i == 0)
					lastStateC = SCE_CSS_DEFAULT;
			}
			sc.Forward ();
			sc.ForwardSetState (lastStateC);
		}

		if (sc.state == SCE_CSS_FUNCTION)
		{
			if (sc.ch == ')')
			{
				if (IsCssOperator (sc.chNext))
				{
					sc.Forward ();
					sc.SetState (SCE_CSS_OPERATOR);
					sc.ForwardSetState
						(SCE_CSS_IDENTIFIER);
				}
				else if (sc.chNext == '\'')
				{
					sc.ForwardSetState
						(SCE_CSS_SINGLESTRING);
				}
				else if (sc.chNext == '\"')
				{
					sc.ForwardSetState
						(SCE_CSS_DOUBLESTRING);
				}
				else
				{
					sc.ForwardSetState (SCE_CSS_VALUE);
				}
			}
			else
			{
				if (!IsValidFunctionParamChar (sc.ch))
				{
					sc.SetState (SCE_CSS_VALUE);
				}
			}
			continue;
		}
		
		if( sc.state == SCE_CSS_ATTR_MATCH && sc.ch == ']' )
				sc.ForwardSetState( SCE_CSS_TAG );
		else if( sc.state == SCE_CSS_ATTR_MATCH && sc.ch == '\n' && sc.chPrev != '\\' )
				sc.ChangeState( SCE_CSS_UNKNOWN_PSEUDOCLASS );
		else if( sc.state == SCE_CSS_ATTR_MATCH && sc.ch != ']')
				continue;
		
		if( sc.state == SCE_CSS_LANGUAGE && sc.ch == ')'){
			sc.ForwardSetState( SCE_CSS_TAG );
		}		
		if (sc.state == SCE_CSS_COMMENT)
			continue;

		if (sc.state == SCE_CSS_DOUBLESTRING
		    || sc.state == SCE_CSS_SINGLESTRING)
		{
			if (sc.ch !=
			    (sc.state == SCE_CSS_DOUBLESTRING ? '\"' : '\''))
				continue;
			unsigned int i = sc.currentPos;
			while (i && styler[i - 1] == '\\')
				i--;
			if ((sc.currentPos - i) % 2 == 1)
				continue;
			sc.ForwardSetState (SCE_CSS_VALUE);
		}
		
		if( sc.state == SCE_CSS_DEFAULT && sc.ch == ','){
			sc.ForwardSetState (SCE_CSS_DEFAULT);
		}
		
		if (sc.state == SCE_CSS_OPERATOR)
		{
			if (op == ' ')
			{
				unsigned int i = startPos;
				op = styler.SafeGetCharAt (i - 1);
				while (--i)
				{
					lastState = styler.StyleAt (i - 1);
					if (lastState != SCE_CSS_OPERATOR
					    && lastState != SCE_CSS_COMMENT)
						break;
				}
			}
			switch (op)
			{
			case '@':
				if( lastState == SCE_CSS_ATTR_MATCH ){
					sc.ChangeState( SCE_CSS_ATTR_MATCH );
				}
				if (lastState == SCE_CSS_DEFAULT) {
					sc.SetState (SCE_CSS_DIRECTIVE);
				}
				break;
			case '{':
				if( lastState == SCE_CSS_ATTR_MATCH ){
					sc.ChangeState( SCE_CSS_ATTR_MATCH );
				}
				if (lastState == SCE_CSS_DIRECTIVE)
				{
					sc.SetState (SCE_CSS_DEFAULT);
				}
				else if ( 
							lastState == SCE_CSS_TAG || 
							lastState == SCE_CSS_CLASS || 
							lastState ==SCE_CSS_UNKNOWN_PSEUDOCLASS ||
							lastState ==SCE_CSS_PSEUDOCLASS || 
							lastState == SCE_CSS_CLASS || 	
							lastState == SCE_CSS_ID ||
							lastState == SCE_CSS_DEFAULT
						)
				{
					sc.SetState (SCE_CSS_IDENTIFIER);
				}
				break;
			case '}':
				if( lastState == SCE_CSS_ATTR_MATCH ){
					sc.ChangeState( SCE_CSS_ATTR_MATCH );
				}
				sc.SetState (SCE_CSS_DEFAULT);
				break;
			case ':':
				if( lastState == SCE_CSS_ATTR_MATCH ){
					sc.ChangeState( SCE_CSS_ATTR_MATCH );
				}
				if (lastState == SCE_CSS_TAG
				    || lastState == SCE_CSS_PSEUDOCLASS
				    || lastState == SCE_CSS_DEFAULT
				    || lastState == SCE_CSS_CLASS
				    || lastState == SCE_CSS_ID
				    || lastState ==
				    SCE_CSS_UNKNOWN_PSEUDOCLASS)
				{
					sc.SetState (SCE_CSS_PSEUDOCLASS);
				}
				else if (lastState == SCE_CSS_IDENTIFIER
					 || lastState ==
					 SCE_CSS_UNKNOWN_IDENTIFIER)
				{
					sc.SetState (SCE_CSS_VALUE);
				}
				break;
			case '.':
				if( lastState == SCE_CSS_CLASS ){
					sc.SetState( SCE_CSS_CLASS );
				}
				else if( lastState == SCE_CSS_ATTR_MATCH )
				{
					sc.ChangeState( SCE_CSS_ATTR_MATCH );
				}
				else if (lastState == SCE_CSS_TAG
				    || lastState == SCE_CSS_DEFAULT)
				{
					sc.ChangeState (SCE_CSS_CLASS);
				}
				else if (lastState == SCE_CSS_FUNCTION)
				{
					sc.ChangeState (SCE_CSS_FUNCTION);
				}
				else if (lastState == SCE_CSS_COLOR)
				{
					sc.ChangeState (SCE_CSS_COLOR);
				}
				else if (lastState == SCE_CSS_NUMBER)
				{
					sc.ChangeState (SCE_CSS_NUMBER);
				}else if( lastState == SCE_CSS_VALUE ){
					sc.ChangeState (SCE_CSS_NUMBER);
				}
				break;
			case '#':
				if( lastState == SCE_CSS_ATTR_MATCH ){
					sc.ChangeState( SCE_CSS_ATTR_MATCH );
				}
				if (lastState == SCE_CSS_TAG
				    || lastState == SCE_CSS_DEFAULT)
				{
					sc.SetState (SCE_CSS_ID);
				}
				else if(lastState == SCE_CSS_VALUE)
				{
					if( IsValidHexaChar( sc.ch ) ){
						sc.ChangeState (SCE_CSS_HEXACOLOR);
					}else{
						sc.ChangeState (SCE_CSS_VALUE);
					}
				}else{
					sc.ChangeState (SCE_CSS_VALUE);
				}

				break;
			case ',':
				if( lastState == SCE_CSS_ATTR_MATCH ){
					sc.ChangeState( SCE_CSS_ATTR_MATCH );
				}
				if (lastState == SCE_CSS_TAG
					|| lastState == SCE_CSS_PSEUDOCLASS
					|| lastState == SCE_CSS_ID
					|| lastState == SCE_CSS_UNKNOWN_PSEUDOCLASS
					|| lastState == SCE_CSS_CLASS
				)
				{
					sc.SetState (SCE_CSS_DEFAULT);
				}	
				else if (lastState == SCE_CSS_VALUE
					 || lastState == SCE_CSS_VALID_VALUE
					 || lastState == SCE_CSS_IMPORTANT
					 || lastState == SCE_CSS_FUNCTION
					 || lastState == SCE_CSS_NUMBER
					 || lastState == SCE_CSS_UNIT)
				{
					sc.SetState (SCE_CSS_VALUE);
				}
				break;
			case ')':
				if( lastState == SCE_CSS_ATTR_MATCH ){
					sc.ChangeState( SCE_CSS_ATTR_MATCH );
				}
				if (lastState == SCE_CSS_FUNCTION)
				{
					sc.ForwardSetState (SCE_CSS_DEFAULT);
				}
				else
				{
					sc.SetState (SCE_CSS_VALUE);
				}
				break;
			case ';':
				if( lastState == SCE_CSS_ATTR_MATCH ){
					sc.ChangeState( SCE_CSS_ATTR_MATCH );
				}
				if (lastState == SCE_CSS_DIRECTIVE)
				{
					sc.SetState (SCE_CSS_DEFAULT);
				}	
				else if (lastState == SCE_CSS_VALUE
					 || lastState == SCE_CSS_VALID_VALUE
					 || lastState == SCE_CSS_IMPORTANT
					 || lastState == SCE_CSS_FUNCTION
					 || lastState == SCE_CSS_NUMBER
					 || lastState == SCE_CSS_UNIT
					 || lastState == SCE_CSS_COLOR
					 || lastState == SCE_CSS_HEXACOLOR
					)
				{
					sc.SetState (SCE_CSS_IDENTIFIER);
				}
				break;
			case '!':
				if( lastState == SCE_CSS_ATTR_MATCH ){
					sc.ChangeState( SCE_CSS_ATTR_MATCH );
				}
				if (lastState == SCE_CSS_VALUE)
				{
					sc.SetState (SCE_CSS_IMPORTANT);
				}
				break;
			}
		}
		if (IsAWordChar (sc.ch) || sc.ch == '*') // allow globbing pattern matching
		{
			if (sc.state == SCE_CSS_DEFAULT)
				sc.SetState (SCE_CSS_TAG);	
		}
		if( sc.state == SCE_CSS_TAG && sc.ch == '['){
			sc.SetState (SCE_CSS_ATTR_MATCH);
		}
		
		
		if(sc.ch==' ' &&
		   (sc.state == SCE_CSS_ID	
			|| sc.state == SCE_CSS_CLASS
			|| sc.state == SCE_CSS_PSEUDOCLASS
			|| sc.state == SCE_CSS_TAG)
		  ){
			  sc.SetState(SCE_CSS_DEFAULT);
		  }
		  
		
		if (IsAWordChar (sc.chPrev)
		    && (sc.state == SCE_CSS_IDENTIFIER
			|| sc.state == SCE_CSS_UNKNOWN_IDENTIFIER
			|| sc.state == SCE_CSS_PSEUDOCLASS
			|| sc.state == SCE_CSS_UNKNOWN_PSEUDOCLASS
			|| sc.state == SCE_CSS_IMPORTANT
			|| sc.state == SCE_CSS_VALUE
			|| sc.state == SCE_CSS_VALID_VALUE
			|| sc.state == SCE_CSS_FUNCTION
			|| sc.state == SCE_CSS_NUMBER
			|| sc.state == SCE_CSS_UNIT
			|| sc.state == SCE_CSS_COLOR
			|| sc.state == SCE_CSS_HEXACOLOR 
			|| sc.state == SCE_CSS_DIRECTIVE
			)
		)
		{
			char s[100];
			sc.GetCurrentLowered (s, sizeof (s));
			char *s2 = s;
			while (*s2 && !IsAWordChar (*s2))
				s2++;

			switch (sc.state)
			{
			case SCE_CSS_DIRECTIVE:		
				if( strcmp(s2,"page") ==0 || strcmp(s2, "font-face") == 0){
					sc.ForwardSetState( SCE_CSS_TAG );
				}
				break;				
			case SCE_CSS_VALUE:				
				if (validValues.InList (s2))
				{					
					if( sc.ch == ' ' || sc.ch == '\t' || sc.ch == '\n' ){
						sc.ChangeState (SCE_CSS_VALID_VALUE);
						sc.SetState (SCE_CSS_VALUE);
					}else if( sc.ch == ';'){
						sc.ChangeState (SCE_CSS_VALID_VALUE);
						sc.SetState (SCE_CSS_OPERATOR);	
						sc.ForwardSetState( SCE_CSS_IDENTIFIER );				
					}else if( sc.ch == ',' ){
						sc.ChangeState (SCE_CSS_VALID_VALUE);
						sc.SetState (SCE_CSS_OPERATOR);	
						sc.ForwardSetState( SCE_CSS_VALUE );						
					}
				}
				else if (commonFunctions.InList (s2))
				{
					sc.ChangeState (SCE_CSS_FUNCTION);
				}
				else if ( IsNum (s2) )
				{
					sc.ChangeState (SCE_CSS_NUMBER);
				}
				else if (namedColors.InList (s2))
				{
					if (sc.ch == ' ')
					{
						sc.ChangeState (SCE_CSS_COLOR);
						sc.ForwardSetState	(SCE_CSS_VALUE);
					}
					else if (IsCssOperator (sc.ch))
					{
						sc.ChangeState (SCE_CSS_COLOR);
						sc.SetState	(SCE_CSS_OPERATOR);
						if (sc.ch == ';')
							sc.ForwardSetState(SCE_CSS_IDENTIFIER);
						else
							sc.ChangeState(SCE_CSS_VALUE);
					}
				}
				else if (validUnits.InList (s2))
				{
					sc.ChangeState (SCE_CSS_UNIT);
					if (sc.ch == ' ')
						sc.ForwardSetState	(SCE_CSS_VALUE);
				}
				break;
			case SCE_CSS_NUMBER:
				if( sc.ch == ' ' || sc.ch == '\t' || '\n'){
					if( sc.chNext == '%' ){
						sc.ForwardSetState( SCE_CSS_UNIT );
					}else{
						sc.SetState( SCE_CSS_VALUE );
					}
				}
				else if( sc.ch == ';' )
				{
					sc.SetState( SCE_CSS_OPERATOR );
					sc.ForwardSetState(SCE_CSS_IDENTIFIER);
				}
				else if( sc.ch == '%' )
				{
					sc.SetState (SCE_CSS_UNIT);
					sc.ForwardSetState ( SCE_CSS_VALUE );
				}
				else if (isalpha( sc.ch ))
				{
					sc.ChangeState (SCE_CSS_VALUE);
				}

				break;
			case SCE_CSS_UNIT:
					  if( !validUnits.InList(s2) ){
						  if( namedColors.InList(s2) ){
							  sc.ChangeState ( SCE_CSS_COLOR );	
							  if( sc.ch == ' ' ){
								  sc.SetState(SCE_CSS_VALUE);
							  }							 														  
						  }else{
					  		sc.ChangeState(SCE_CSS_VALUE);
						  }
					  }
				  break;  
			case SCE_CSS_COLOR:
				if( sc.ch == ' ' || sc.ch == '\t' || '\n'){
					sc.SetState( SCE_CSS_VALUE );
				}
				else if( sc.ch == ';' )
				{
					sc.SetState( SCE_CSS_OPERATOR );
					sc.ForwardSetState(SCE_CSS_IDENTIFIER);
				}
				break;			
			case SCE_CSS_VALID_VALUE:
				if (commonFunctions.InList (s2))
					sc.SetState (SCE_CSS_FUNCTION);
				else if (!validValues.InList (s2))
					sc.SetState (SCE_CSS_VALUE);
				break;
			case SCE_CSS_HEXACOLOR:
				if( sc.ch == ' '|| sc.ch == '\n' || sc.ch == '\t'){
					if( !IsValidHexaString( s2 ) )
						sc.ChangeState( SCE_CSS_VALUE );
					else
						sc.SetState( SCE_CSS_VALUE );
				}else if( sc.ch == ';' ){
					if( !IsValidHexaString( s2 ) )
						sc.ChangeState( SCE_CSS_VALUE );
					else
						sc.SetState( SCE_CSS_VALUE );
				}
				
				break;
			case SCE_CSS_IDENTIFIER:
				if( sc.ch == '"' || sc.ch == '\'' )
					sc.ChangeState( SCE_CSS_UNKNOWN_IDENTIFIER );

				if (!validProperties.InList (s2))
					sc.ChangeState	(SCE_CSS_UNKNOWN_IDENTIFIER);
				break;
			case SCE_CSS_UNKNOWN_IDENTIFIER:
				if (validProperties.InList (s2))
					sc.ChangeState (SCE_CSS_IDENTIFIER);
				break;
			case SCE_CSS_PSEUDOCLASS:
				if( strcmp("lang",s2)==0){
					sc.ChangeState	(SCE_CSS_LANGUAGE);
				}
				else if( sc.ch==' ' ){
					sc.SetState (SCE_CSS_DEFAULT);
				}
				else if (!pseudoClasses.InList (s2))
				{
					sc.ChangeState	(SCE_CSS_UNKNOWN_PSEUDOCLASS);
				}
				break;
			case SCE_CSS_UNKNOWN_PSEUDOCLASS:
				if( strcmp("lang",s2)==0){
					sc.ChangeState	(SCE_CSS_LANGUAGE);
				}
				else if (pseudoClasses.InList (s2))
					sc.ChangeState (SCE_CSS_PSEUDOCLASS);
				break;
			case SCE_CSS_IMPORTANT:
				if( sc.ch == ' ' || sc.ch == '\t' || sc.ch == '\n' ){
					if (strcmp (s2, "important") != 0)
						sc.ChangeState( SCE_CSS_VALUE );
					else
						sc.SetState( SCE_CSS_VALUE );
				}
				else if (sc.ch == ';' ) 	
				{
					if (strcmp (s2, "important") != 0)
						sc.ChangeState( SCE_CSS_VALUE );
					else
						sc.SetState( SCE_CSS_VALUE );
				}
				break;
			}
		}
		if( sc.state == SCE_CSS_VALUE && isdigit(sc.ch) )
				sc.ChangeState( SCE_CSS_NUMBER);
		
		if( sc.state == SCE_CSS_IDENTIFIER && (sc.ch == '"' || sc.ch == '\'') )
			sc.ChangeState( SCE_CSS_UNKNOWN_IDENTIFIER );			

		if (sc.Match ('/', '*'))
		{
			lastStateC = sc.state;
			sc.SetState (SCE_CSS_COMMENT);
			sc.Forward ();

		}
		else if ((sc.state == SCE_CSS_VALUE
			  	|| sc.state == SCE_CSS_VALID_VALUE
				|| sc.state == SCE_CSS_FUNCTION
	 			|| sc.state == SCE_CSS_NUMBER
				|| sc.state == SCE_CSS_UNIT
				|| sc.state == SCE_CSS_COLOR
				|| sc.state == SCE_CSS_HEXACOLOR
		)
			 && (sc.ch == '\"' || sc.ch == '\''))
		{
			sc.SetState ((sc.ch ==
				      '\"' ? SCE_CSS_DOUBLESTRING :
				      SCE_CSS_SINGLESTRING));
		}	
		else if (IsCssOperator (static_cast < char >(sc.ch))
			 && (sc.state != SCE_CSS_VALUE
			     || sc.state != SCE_CSS_VALID_VALUE
			     || sc.ch == ';' || sc.ch == '}' || sc.ch == '!')
			 && (sc.state != SCE_CSS_DIRECTIVE || sc.ch == ';'
			     || sc.ch == '{'))
		{
			if (sc.state != SCE_CSS_OPERATOR)
				lastState = sc.state;

			if (sc.state != SCE_CSS_NUMBER)
				sc.SetState (SCE_CSS_OPERATOR);
			else
				sc.ChangeState (SCE_CSS_NUMBER);
			op = sc.ch;
		}
	}
	sc.Complete ();
}

/* FOLDING */
static void
FoldCSSDoc (unsigned int startPos, int length, int, WordList *[],
	    Accessor & styler)
{
	bool foldComment = styler.GetPropertyInt ("fold.comment") != 0;
	bool foldCompact = styler.GetPropertyInt ("fold.compact", 1) != 0;
	unsigned int endPos = startPos + length;
	int visibleChars = 0;
	int lineCurrent = styler.GetLine (startPos);
	int levelPrev = styler.LevelAt (lineCurrent) & SC_FOLDLEVELNUMBERMASK;
	int levelCurrent = levelPrev;
	char chNext = styler[startPos];
	bool inComment = (styler.StyleAt (startPos - 1) == SCE_CSS_COMMENT);
	
	for (unsigned int i = startPos; i < endPos; i++)
	{
		char ch = chNext;
		chNext = styler.SafeGetCharAt (i + 1);
		int style = styler.StyleAt (i);
		bool atEOL = (ch == '\r' && chNext != '\n') || (ch == '\n');
		if (foldComment)
		{
			if (!inComment && (style == SCE_CSS_COMMENT))
				levelCurrent++;
			else if (inComment && (style != SCE_CSS_COMMENT))
				levelCurrent--;
			inComment = (style == SCE_CSS_COMMENT);
		}
		if (style == SCE_CSS_OPERATOR)
		{
			if (ch == '{')
			{
				levelCurrent++;
			}
			else if (ch == '}')
			{
				levelCurrent--;
			}
		}
		
		if (atEOL)
		{
			int lev = levelPrev;
			if (visibleChars == 0 && foldCompact)
				lev |= SC_FOLDLEVELWHITEFLAG;
			if ((levelCurrent > levelPrev) && (visibleChars > 0))
				lev |= SC_FOLDLEVELHEADERFLAG;
			if (lev != styler.LevelAt (lineCurrent))
			{
				styler.SetLevel (lineCurrent, lev);
			}
			lineCurrent++;
			levelPrev = levelCurrent;
			visibleChars = 0;
		}
		if (!isspacechar (ch))
			visibleChars++;
	}
	// Fill in the real level of the next line, keeping the current flags as they will be filled in later
	int flagsNext =
		styler.LevelAt (lineCurrent) & ~SC_FOLDLEVELNUMBERMASK;
	styler.SetLevel (lineCurrent, levelPrev | flagsNext);
}

static const char *const cssWordListDesc[] = {
	"Valid properties",
	"Pseudo classes",
	"Valid fixed values",
	"Well known functions",
	"Valid units",
	"Named colours",
	0
};

LexerModule lmCss (SCLEX_CSS, ColouriseCssDoc, "css", FoldCSSDoc,
		   cssWordListDesc);
