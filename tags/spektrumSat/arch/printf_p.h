#ifndef PRINTF_P_H
#define PRINTF_P_H




extern void PRINT(const char *, unsigned int);
extern void PRINTP(const char *, unsigned int);
extern void PAD_SP(signed char);
extern void PAD_0(signed char);
extern void __printf_P (char const *fmt0, ...);

#define print_uart0(format, args...)   __printf_P(format , ## args)

#endif
