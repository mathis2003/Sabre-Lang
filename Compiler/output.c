
#include <stdio.h>

#include <stdint.h>
uint32_t a = (6);
uint32_t *b = &(uint32_t ){0};
uint32_t *c;
uint32_t  anon0 (uint32_t  n) {
	uint32_t  x1;
	(x1 = (((n)>(1)) ? ((n)*((anon0)(((n)-(1))))) : (1)));

	return x1;

}
uint32_t (*factorial)(uint32_t ) = &anon0;
uint32_t  anon1 (uint32_t  n) {
	uint32_t  x2;
	(x2 = (((n)>(1)) ? (((anon1)(((n)-(1))))+((anon1)(((n)-(2))))) : (((n)==(1)) ? (1) : (0))));

	return x2;

}
uint32_t (*fibonacci)(uint32_t ) = &anon1;
uint32_t  anon2 (uint32_t  f1 , uint32_t  f2 , uint32_t  n) {
	uint32_t  x3;
	(x3 = (((n)<(f2)) ? ((f1)+((anon2)((f1) , (f2) , ((n)+(1))))) : (0)));

	return x3;

}
uint32_t (*mult)(uint32_t  , uint32_t  , uint32_t ) = &anon2;
uint32_t  anon3 (uint32_t  base , uint32_t  e , uint32_t  n) {
	uint32_t  x4;
	(x4 = (((n)<(e)) ? ((base)*((anon3)((base) , (e) , ((n)+(1))))) : (1)));

	return x4;

}
uint32_t (*exponent)(uint32_t  , uint32_t  , uint32_t ) = &anon3;
uint32_t  anon5 () {
	uint32_t  x6;
	(x6 = (5));

	return x6;

}
uint32_t (*ch)() = &anon5;
uint8_t  anon4 () {
	uint8_t  x5;
	(x5 = (((ch)())+(1)));

	return x5;

}
uint8_t (*p)() = &anon4;
int main() {
	*b = (5);
	c = b;
	(*c = ((*c)+(8)));
	((printf)(("a = %d | b = %d | c = %d\n") , (a) , (*b) , (*c)));
	((printf)(("Hello, World!\n")));
	((printf)(("factorial: %d\n") , ((factorial)((a)))));
	((printf)(("fibonacci: %d\n") , ((fibonacci)((a)))));
	((printf)(("mult (%d, %d): %d\n") , (4) , (5) , ((mult)((4) , (5) , (0)))));
	((printf)(("exponent (%d, %d): %d\n") , (2) , (4) , ((exponent)((2) , (4) , (0)))));
	((printf)(("p: %d\n") , ((p)())));

	return 0;
}