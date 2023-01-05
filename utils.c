/*
 * >0 if parens are not closed
 * 0  if all parens closed
 * -1 if unexpected close paren
 */
int checkParensParity(char* input) {
	int parity = 0;
	while(*input) {
		if(*input == '(')
			parity++;
		else if(*input == ')') {
			parity--;
			if(parity < 0) 
				return parity;
		}
		input++;
	}
	return parity;
}
