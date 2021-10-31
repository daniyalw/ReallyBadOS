unsigned char mouse_cycle=0;     //unsigned char
unsigned char mouse_byte[3];    //signed char
char mouse_x=0;         //signed char
char mouse_y=0;         //signed char
bool mouse_ready = false;
void mouse_install();
int mouse_handler();
unsigned char mouse_read();
void mouse_write(unsigned char a_write);
void mouse_wait(unsigned char a_type);
