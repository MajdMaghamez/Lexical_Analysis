#include <cctype>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

// the lexer implements a DFA with these states
// each state is covered by a case in the lexer function
enum Lexstate {
	SEEKING_STARTTAG,
	FOUND_STARTTAG,
	FINDING_ELEMENT,
	FOUND_SLASH_SEEKING_CHAR,
	FOUND_SLASH_CHAR_SEEKING_ID,
	FOUND_CHAR_SEEKING_ID,
	FOUND_BANG,
	FOUND_BANG_DASH,
	IN_COMMENT,
	FOUND_DASH,
	FOUND_DASHDASH,
	FOUND_TAG,
	FOUND_ENDTAG,
	SEEKING_ATTRNAME,
	FOUND_ATTR_EQUAL,
	FOUND_ATTR_EQUAL_QUOTE,
	ERROR_CONSUME_TO_ENDTAG,
	ERROR_CONSUME_ATTR,
};

// these are the specific characters for the lexer
const char TAGSTART = '<';
const char TAGEND = '>';
const char SLASH = '/';
const char BANG = '!';
const char DASH = '-';
const char EQUAL = '=';
const char QUOTE = '"';

// there's actually only a couple of tokens in this machine!
enum Tokentype {
		TAG,
		ENDTAG,
		ATTRNAME,
		ERRORTAG,
		BADATTR,
};

// this counter keeps track of the line number on input
int	input_line_num = 1;

// this class is used to represent the token
class Token {
	Tokentype tokentype;
	string Tokenval;		// the specific string that is the TAG, ENDTAG, etc
	int		linenum;		// keep the line number where the token was found

public:
	Token(Tokentype t, string v = "") {
		tokentype = t;
		Tokenval = v;
		linenum = input_line_num;
	}

	bool operator==(int tt) { return tt == tokentype; }

	Tokentype gettype() { return tokentype; }
	string getid() { return Tokenval; }

	string toString() {
		stringstream s;
		s << "(";
		switch( tokentype ) {
		case TAG:
			s << "TAG";
			break;
		case ENDTAG:
			s << "ENDTAG";
			break;
		case ATTRNAME:
			s << "ATTRNAME";
			break;
		case ERRORTAG:
			s << "ERRORTAG";
			break;
		case BADATTR:
			s << "BADATTR";
			break;
		}
		s << ":" << Tokenval << " line: " << linenum << ")";
		return s.str();
	}
};

// this type is used to keep statistics on words
// basically it's a string and a counter for how many times it was found
// for an 'end tag', that is, /tag, I use foundclose to count end occurrences
class WordStats {
	string	id;
	int		found;
	int		foundclose;

public:
	WordStats( string s ) {
		id = s;
		found = foundclose = 0;
	}

	string getid() { return id; }
	int sawopen() { return ++found; }
	int sawclose() { return ++foundclose; }
	int getopen() { return found; }
	int getclose() { return foundclose; }
};
// this function returns a token stream on successive calls
// note that the function does requirement a and d itself,
// and leaves it to the caller to suss out the rest
Token *
lex() {
	// these variables live across invocations of the lex function
	// they keep track of the state of the lexer state machine
	// and the pushback character, if any...
	static Lexstate lexstate = SEEKING_STARTTAG;

	// we need to keep a Pushback character: we look ahead one character
	// to determine that we have reached the end of a token... then we need
	// to save that character and re-examine it when we are in a new state
	static char Pushback = 0;

	char ch;

	string tokenval = "";

	while( !cin.eof() || Pushback ) {
		if( Pushback ) {
			ch = Pushback;
			Pushback = 0;
		}
		else {
			ch = cin.get();
			if( ch == '\n' ) ++input_line_num;
		}

		//cout <<"received a " <<ch <<" in state " <<lexstate <<endl;

		switch (lexstate) {
		case SEEKING_STARTTAG:
			if (ch == TAGSTART) {
				lexstate = FOUND_STARTTAG;
			}
			continue;

		case ERROR_CONSUME_TO_ENDTAG:
			if( ch == TAGEND ) {
				lexstate = SEEKING_STARTTAG;
				return new Token(ERRORTAG, tokenval);
			}
			continue;

		// in this state, I have found a <
		// I expect to find:
		//    a ! to begin something (maybe a comment?)
		//    a / to begin an endtag
		//    a starting character to begin a tagname
		//    anything else is an error
		case FOUND_STARTTAG:
			if (ch == BANG) {
				lexstate = FOUND_BANG;
				continue;
			} else if( ch == SLASH ) {
				lexstate = FOUND_SLASH_SEEKING_CHAR;
			} else if( isalpha(ch) ) {
				lexstate = FOUND_CHAR_SEEKING_ID;
				tokenval = ch;
			} else {
				lexstate = ERROR_CONSUME_TO_ENDTAG;
			}
			continue;

		case FOUND_CHAR_SEEKING_ID:
			if( isalnum(ch) ) {
				tokenval += ch;
			} else {
				Pushback = ch;
				lexstate = FOUND_TAG;
				return new Token(TAG, tokenval);
			}
			continue;

		case FOUND_SLASH_SEEKING_CHAR:
			if( isalpha(ch) ) {
				lexstate = FOUND_SLASH_CHAR_SEEKING_ID;
				tokenval = ch;
			} else {
				lexstate = ERROR_CONSUME_TO_ENDTAG;
			}
			continue;

		case FOUND_SLASH_CHAR_SEEKING_ID:
			if( isalnum(ch) ) {
				tokenval += ch;
			} else {
				Pushback = ch;
				lexstate = FOUND_ENDTAG;
				return new Token(ENDTAG, tokenval);
			}
			continue;

		case FOUND_BANG:
			if (ch == DASH) {
				lexstate = FOUND_BANG_DASH;
				continue;
			} else {
				lexstate = ERROR_CONSUME_TO_ENDTAG;
				continue;
			}
			break;

		case FOUND_BANG_DASH:
			if (ch == DASH) {
				lexstate = IN_COMMENT;
				continue;
			} else {
				lexstate = ERROR_CONSUME_TO_ENDTAG;
				continue;
			}
			break;

		case IN_COMMENT:
			if (ch == DASH)
				lexstate = FOUND_DASH;
			continue;

		case FOUND_DASH:
			if (ch == DASH)
				lexstate = FOUND_DASHDASH;
			else
				lexstate = IN_COMMENT;
			continue;

		case FOUND_DASHDASH:
			if (ch == TAGEND)
				lexstate = SEEKING_STARTTAG;
			else
				lexstate = IN_COMMENT;
			continue;

		case FOUND_TAG:
			// looking for attribute
			if( isalpha(ch) ) {
				lexstate = SEEKING_ATTRNAME;
				tokenval = ch;
			} else if( ch == TAGEND )
				lexstate = SEEKING_STARTTAG;
			else if( !isspace(ch) ){
				tokenval = ch;
				lexstate = ERROR_CONSUME_ATTR;
			}
			continue;

		case SEEKING_ATTRNAME:
			if( isalnum(ch) )
				tokenval += ch;
			else if( ch == EQUAL ) {
				lexstate = FOUND_ATTR_EQUAL;
			}
			else {
				Pushback = ch;
				lexstate = ERROR_CONSUME_ATTR;
			}
			continue;

		case FOUND_ATTR_EQUAL:
			if( ch == QUOTE )
				lexstate = FOUND_ATTR_EQUAL_QUOTE;
			else {
				Pushback = ch;
				lexstate = ERROR_CONSUME_ATTR;
			}
			continue;

		case FOUND_ATTR_EQUAL_QUOTE:
			if( ch == QUOTE ) {
				lexstate = FOUND_TAG;
				return new Token(ATTRNAME, tokenval);
			}
			continue;

		case ERROR_CONSUME_ATTR:
			// interesting bug used to exist below: I compared ch to ENDTAG,
			// the token value, not TAGEND, the character. Havoc ensued.
			if( ch == TAGEND ) {
				lexstate = SEEKING_STARTTAG;
				return new Token(BADATTR, tokenval);
			}
			else if( isspace(ch) ) {
				lexstate = FOUND_TAG;
				return new Token(BADATTR, tokenval);
			}
			else tokenval += ch;
			continue;

		case FOUND_ENDTAG:
			// NOTE this code silently ignores whitespace between endtag and >
			if( ch == TAGEND )
				lexstate = SEEKING_STARTTAG;
			continue;

		default:
			break;
		}
	}

	return 0;
}

// I use a map from a string to a pointer-to-WordStats, AND
// I use a vector.

// Each time I find a new word (a tag or an attr), I enter it into
// the map AND the vector

// I am using vectors because they're easier to sort with the standard library stuff
map<string, WordStats*> Tags;
vector<WordStats*> Tagsvec;

map<string, WordStats*> Attrs;
vector<WordStats*> Attrsvec;

// this function is passed to the sort routine to put wordstats items in descending order
bool WordStatsDescendComp(WordStats *i, WordStats *j) {
	return j->getopen() < i->getopen();
}

int main() {
	Token *t;
	WordStats *wd;
	int	badtags = 0;
	int badattrs = 0;

	while ( (t = lex()) ) {
		cout << t->toString() << endl;

		switch( t->gettype() ) {
		case TAG:
		case ENDTAG:
			// if I have seen this word before, it will be in the map, so fetch it
			if( Tags.count( t->getid() ) )
				wd = Tags[ t-> getid() ];
			else {
				// here, this is the first time for this word. Get a new one,
				// and put it in the map and in the vector
				Tags[ t->getid() ] = wd = new WordStats( t->getid() );
				Tagsvec.push_back(wd);
			}

			// here, do the actual counting...
			if( t->gettype() == TAG )
				wd->sawopen();
			else
				wd->sawclose();
			break;

		case ATTRNAME:
			if( Attrs.count( t->getid() ) )
				wd = Attrs[ t-> getid() ];
			else {
				Attrs[ t->getid() ] = wd = new WordStats( t->getid() );
				Attrsvec.push_back(wd);
			}

			wd->sawopen();
			break;

		case ERRORTAG:
			badtags++;
			break;

		case BADATTR:
			badattrs++;
			break;
		}
	}

	// now generate the output
	// dump all tags for now

	cout << "BAD TAGS:" << badtags << endl;
	cout << "BAD ATTRS:" << badattrs << endl;

	// use the library sort function to sort the vectors
	sort(Tagsvec.begin(), Tagsvec.end(), WordStatsDescendComp);
	sort(Attrsvec.begin(), Attrsvec.end(), WordStatsDescendComp);

	vector<WordStats*>::iterator iter;
	int	outcount, maxout = 10;

	for( iter = Tagsvec.begin(), outcount = 0;
			iter != Tagsvec.end() && outcount < maxout;
			iter++,outcount++ ) {
		wd = *iter;
		cout << wd->getid() << ":";
		cout << wd->getopen() << endl;
	}

	for( iter = Attrsvec.begin(), outcount = 0;
			iter != Attrsvec.end() && outcount < maxout;
			iter++,outcount++ ) {
		wd = *iter;
		cout << wd->getid() << ":" << wd->getopen() << endl;
	}

	// here I look for all the places where there's not the same number
	// of opens and closes. I call that a mismatch
	for( iter = Tagsvec.begin(); iter != Tagsvec.end(); iter++ ) {
		wd = *iter;
		if( wd->getopen() == wd->getclose() )
			continue;
		cout << "MISMATCH:" << wd->getid() << ":";
		cout << wd->getopen() - wd->getclose()<< endl;
	}
	return 0;
}
