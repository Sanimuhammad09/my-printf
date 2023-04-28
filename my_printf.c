#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
/* function for collecting the char to be printed in the output */
int ftn_putchar(int z) {
    char y[1];
    y[0] = (char)z;
    return write(1, y, 1);
}

/* Function to ctrl format and returns the total size of arguments with the total size of the base string */
int ftn_printf_aux(char *str, va_list list, int len);

/*  */
int ftn_putnum(unsigned long long n, unsigned int base, const char *digits) {
    int res = 1;
    if (n >= base)
        res += ftn_putnum(n / base, base, digits);
    ftn_putchar(digits[n % base]);
    return res;
}

/* Function for int (or listpropriate variant) argument convertion to signed decimal (d) va_arg() fetches the 
next arg in the para list of the function with the var type returns the next additional argument as an expression 
of the type of var. va_start() must be called first to initialize the list or object bf  using the va_arg()*/
int ftn_print_d(char *str, va_list list, int len) {
    int t = va_arg(list, int);
    unsigned long long a;
    if (t < 0) {
        ftn_putchar('-');
        len++;
        a = -(unsigned)t;
    } else {
        a = t;
    }
    len += ftn_putnum(a, 10, "0123456789");
    return ftn_printf_aux(str, list, len);
}

/* Function for int (or listpropriate variant) argument convertion to unsigned octal (o) */
int ftn_print_o(char *str, va_list list, int len) {
    unsigned int n = va_arg(list, unsigned int);
    len += ftn_putnum(n, 8, "01234567");
    return ftn_printf_aux(str, list, len);
}

/* Function for int (or listpropriate variant) argument convertion to unsigned decimal (u) */
int ftn_print_u(char *str, va_list list, int len) {
    int n = va_arg(list, int);
    if(n<0){
        n = -n;
    }
    len += ftn_putnum(n, 10, "0123456789");
    return ftn_printf_aux(str, list, len);
}

/* Function for int (or listpropriate variant) argument convertion to unsigned hexadecimal (x) */
int ftn_print_x(char *str, va_list list, int len) {
    unsigned int n = va_arg(list, unsigned int);
    len += ftn_putnum(n, 16, "0123456789abcdef");
    return ftn_printf_aux(str, list, len);
}

/* Function for int argument convertion to an unsigned char (c) */
int ftn_print_c(char *str, va_list list, int len) {
    int c = va_arg(list, int);
    ftn_putchar(c);
    return ftn_printf_aux(str, list, len + 1);
}

/* Function for int char* argument expected to be a pointer to an array of character type (p) (pointer to 
a string). Characters from the array are written up to (but not including) a terminating NUL character */
int ftn_print_p(char *str, va_list list, int len) {
    unsigned int n = va_arg(list, unsigned int);
    len += ftn_putnum(n, 16, "0123456789abcdef");
    return ftn_printf_aux(str, list, len);
}

/* Function void * pointer argument (s) printed in hexadecimal */
int ftn_print_s(char *str, va_list list, int len) {
    const char *s = va_arg(list, const char *);
    if (s == NULL) {
        s = "(null)";
    }
    while (*s) {
        ftn_putchar(*s++);
        len++;
    }
    return ftn_printf_aux(str, list, len);
}

/*  */
typedef int (*ftn_print_dispatch_f)(char *str, va_list list, int len);

/*Function to handle the percentile. It controls percent format. */
ftn_print_dispatch_f const ftn_print_dispatch[256] = {
    ['d'] = ftn_print_d,
    ['o'] = ftn_print_o,
    ['u'] = ftn_print_u,
    ['x'] = ftn_print_x,
    ['c'] = ftn_print_c,
    ['s'] = ftn_print_s,
    ['p'] = ftn_print_p,
};

/* Function for the size of the elements of the specifier to be printed, where; char *str 
is the string format, va_list list is the list of arguements and int len is the 
iteration/positions/length/size etc parameter*/
int ftn_printf_aux(char *str, va_list list, int len) {
    int q;

    while (*str) {
        q = (unsigned char)*str++;
        if (q != '%') {
            ftn_putchar(q);
            len++;
        } else {
            q = (unsigned char)*str++;
            if (ftn_print_dispatch[q] == NULL) {
                if (q == '\0')
                    break;
                ftn_putchar(q);
                len++;
            } else {
                return ftn_print_dispatch[q](str, list, len);
            }
        }
    }
    return len;
}

/* Function to return the  */
int ft_vprintf(char *str, va_list list) {
    return ftn_printf_aux(str, list, 0);
}

/* Function for my_printf i.e custom printf() */
int my_printf(char *str, ...) {
    va_list list;
    int r;
    va_start(list, str);
    r = ftn_printf_aux(str, list, 0);
    va_end(list);
    return r;
}

/* Main function to test the functionality of the program locally */
int main(void) {
    int total = my_printf("%d\n", 123);
    my_printf("%d\n", total);
    my_printf("%cello %s\n", 'H', "World!");
    my_printf("%d == 0%o == 0x%x == 0x%X\n", 1, 1, 1, 1);
    my_printf("%d == 0%o == 0x%x == 0x%X\n", 0xdead, 0xdead, 0xdead, 0xdead);
    return 0;
}