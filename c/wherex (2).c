#include <conio.h>  

int main(void) 
{ 
   clrscr(); 
   gotoxy(10,10); 
   cprintf("Current location is X: %d  Y: %d\r\n", wherex(), wherey()); 
   getch();  

   return 0; 
} 
