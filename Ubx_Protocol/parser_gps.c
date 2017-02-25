#include <stdio.h>
#include <stdint.h>

struct Ubx_protocol{
  unsigned char sync1;
  unsigned char sync2;
  unsigned char class;
  unsigned char id;
  unsigned short length;
  char payload[200];
  unsigned char check_A;
  unsigned char check_B;
};

struct Message{
  unsigned int itow;
	unsigned int tAcc;
	unsigned int nano;
	short year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
	unsigned char valid;
};

int main(int argc, char const *argv[]){

  // example of data
  unsigned char data[] = {
    0xb5, // sync1
    0x62, // sync2
    0x01, // class
    0x21, // id
    0x14, 0x00, // length
    0x60, 0x12, 0xe7, 0x1e, 0x0d, 0x00, 0x00, 0x00, 0x6c, 0x77, 0x00, 0x00, 0xe0, 0x07, 0x07, 0x17, 0x00, 0x00, 0x2b, 0x07, // payload
    0xd4, // check_A
    0x01 // check_B
  };

  // Filling of the structure with the data
  struct Ubx_protocol ubx = *(struct Ubx_protocol *) data; // data as adress
  // Beware of check_A and check_B => filling by hand
  ubx.check_A = ubx.payload[ubx.length];
  ubx.check_B = ubx.payload[ubx.length+1];

  printf("Protocole: \n \
            \t Sync numero 1: %.2x \n \
            \t Sync numero 2: %.2x \n \
            \t Class        : %.2x \n \
            \t Id           : %.2x \n ", ubx.sync1, ubx.sync2, ubx.class, ubx.id);

  struct Message msg;
  // The structure message is only for :
  if (ubx.sync1 == 0xb5 && ubx.sync2 == 0x62){
    if (ubx.class == 0x01 && ubx.id == 0x21){

      // Filling the message
      msg = *(struct Message *) ubx.payload; // ubx.payload as adress

      printf("Message : \n \
            \t iTow : %d \n \
            \t tAcc : %d \n \
            \t nano : %d \n \
            \t year : %d \n \
            \t month: %d \n \
            \t day  : %d \n \
            \t hour : %d \n \
            \t min  : %d \n \
            \t sec  : %d \n \
            \t valid: %d \n", msg.itow, msg.tAcc, msg.nano, msg.year, msg.month, msg.day, msg.hour, msg.min, msg.sec, msg.valid);
    }
  }

  // Check the check sum A and B
  unsigned char check_check_A = 0;
  unsigned char check_check_B = 0;  // initialisation of the calculated check sum
  // Calculation of the check sum
  int i;
  for ( i = 2; i < sizeof(data) - 2; i++) {
    check_check_A += data[i];
    check_check_B += check_check_A;
    printf("%d, %d, %d, %d \n ", i, data[i], check_check_A,check_check_B);
  }

  printf("Verification: \n \
          \t Check Sum A: \t given: %d \t calculated: %d \n \
          \t Check Sum B: \t given: %d \t calculated: %d \n", ubx.check_A, check_check_A, ubx.check_B, check_check_B);

  if (check_check_A == ubx.check_A && check_check_B == ubx.check_B){
    printf("\n \t Everything gonna be alright ! \n ");
  }else{
    printf("\n \t There is a fly in the ointment ! \n");
  }

}
