void split(char *string, char *target[], char *separator);
void showInfo();
void isExit(char *args[]);
void showEnvironment();
void append(char *str, char c);
char *getData(char *str);
/// @brief To replace the target in the source
/// @param source
/// @param target
/// @return
char *replaceFirst(char *source, char *target, char *replacement);
int isContain(char *source, char *target, int *startPosition, int *endPosition);
void identify(char *);
int cd(char **);
void output_redirect(char *input);
void input_redirect(char *input);
void exec_pipe(char *input);
void process( char *args[]);
void showHello();