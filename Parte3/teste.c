#include <ctype.h>
#include <stdio.h>

int main(void)
{
    char buf[BUFSIZ];
    int value, end;
    
    printf("Enter an integer: ");
    fflush(stdout);
    
    while (fgets(buf, sizeof buf, stdin) == NULL ||
           sscanf(buf, "%d%n", &value, &end) != 1 || 
           !isspace(buf[end]))
    {
        printf("Invalid integer. Please try again: ");
        fflush(stdout);
    }
    
    printf("You entered %d\n", value);
    
    return 0;
}