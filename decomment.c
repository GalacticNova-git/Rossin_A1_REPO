#include <stdio.h>
#include <stdlib.h>

/* Represents if the char is in normal text, 
a string literal, char literal, or comment */
enum Statetype {
	NORMAL,
	SLASH,
	COMMENT,
	STAR,
	STRINGLITERAL,
	STRINGLITERALESCAPE,
	CHARLITERAL,
	CHARLITERALESCAPE};

/* Function prototypes for each state of code,
 and what to do for certain characters in each state*/
enum Statetype handleNormal(int c);
enum Statetype handleSlash(int c);
enum Statetype handleComment(int c);
enum Statetype handleStar(int c);
enum Statetype handleStringLiteral(int c);
enum Statetype handleStringLiteralEscape(int c);
enum Statetype handleCharLiteral(int c);
enum Statetype handleCharLiteralEscape(int c);

/* Handles characters when in the NORMAL state */
enum Statetype handleNormal(int c) {
    if (c == '/') {
	return SLASH;
    }
    if (c == '\'') {
	putchar(c);
	return CHARLITERAL;
	}
    if (c == '"') {
	putchar(c);
	return STRINGLITERAL;
	}
	putchar(c);
	return NORMAL;
}

/* Handles characters when in the SLASH state. */
enum Statetype handleSlash(int c) {
    if (c == '*') {
        putchar(' '); /* Because we are in a comment, we want to replace it with a space */
        return COMMENT;
   	}
    putchar('/'); /* Because we aren't in a comment, we want to keep the slash */
    if (c == '/') {
	return SLASH;
    }
    if (c == '"') {
        putchar(c);
        return STRINGLITERAL;
    }
    if (c == '\'') {
        putchar(c);
        return CHARLITERAL;
    }
	putchar(c);
	return NORMAL;
}

/* Handles characters when in the COMMENT state. */
enum Statetype handleComment(int c) {
    if (c == '*') {
        return STAR;
    }
    if (c == '\n') {
        putchar(c); /* So we can track when we enter a new line and continue on the new line */
    }
    return COMMENT;
}

/* Handles characters when in the STAR state. */
enum Statetype handleStar(int c) {
    if (c == '/') {
        return NORMAL;
    }
    if (c == '*') {
        return STAR;
    }
if (c == '\n') { /* So we can track a new line and continue on the next line */
        putchar(c);
    }
    return COMMENT;
}

/* Handles characters when in the STRINGLITERAL state. */
enum Statetype handleStringLiteral(int c) {
    putchar(c);
    if (c == '\\') { /* I initially had an error here, and had to check notes to see I needed two backslashes */
        return STRINGLITERALESCAPE;
    }
    if (c == '"') {
        return NORMAL;
    }
    return STRINGLITERAL;
}

/* Handles characters when in the STRINGLITERALESCAPE state. */
enum Statetype handleStringLiteralEscape(int c) {
    putchar(c);
    return STRINGLITERAL;
}

/* Handles characters when in the CHARLITERAL state. */
enum Statetype handleCharLiteral(int c) {
    putchar(c);
    if (c == '\\') {
        return CHARLITERALESCAPE;
    }
    if (c == '\'') {
        return NORMAL;
    }
    return CHARLITERAL;
}

/* Handles characters when in the CHARLITERALESCAPE state. */
enum Statetype handleCharLiteralEscape(int c) {
    putchar(c);
    return CHARLITERAL;
}

int main(void) {
	int c;
	enum Statetype state = NORMAL;
	int currentLine = 1;
	int commentStartLine = 0;

	while ((c = getchar()) != EOF) {
	/* Used to route state transitions and capture state updates */
	enum Statetype nextState = state;

	switch (state) {
	    case NORMAL:
		nextState = handleNormal(c);
		break;
            case SLASH:
                nextState = handleSlash(c);
                break;
            case COMMENT:
                nextState = handleComment(c);
                break;
            case STAR:
                nextState = handleStar(c);
                break;
            case STRINGLITERAL:
                nextState = handleStringLiteral(c);
                break;
            case STRINGLITERALESCAPE:
                nextState = handleStringLiteralEscape(c);
                break;
            case CHARLITERAL:
                nextState = handleCharLiteral(c);
                break;
            case CHARLITERALESCAPE:
                nextState = handleCharLiteralEscape(c);
                break;
        }

/* Record the exact line the comment begins */
	if (state != COMMENT && state != STAR && 
	(nextState == COMMENT || nextState == STAR)) {
	commentStartLine = currentLine;
	}

/* update line count if text goes accross multiple lines */
	if ( c == '\n') {
		currentLine++;
	}

	state = nextState;
	} /* End of while loop */

    /* If the entire input ends on a slash, prints slash */
    if (state == SLASH) {
        putchar('/');
    }

    /* Error statement for if program detects EOF before comment terminates */
    if (state == COMMENT || state == STAR) {
        fprintf(stderr, "Error: line %d: unterminated comment\n",
        commentStartLine);
        return EXIT_FAILURE;
    }

	return 0;
}
