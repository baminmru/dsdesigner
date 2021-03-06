/****************************************************
 *    NUM2STR.CPP                                   *
 *  Copyright (C) ������ �. �������                 *
 *	 (����������� ������ ����� �. ������)            *
 * ����������� ��� ���������������� �������� ������ *
 * ����� ��������������� ��� ��������� ����������   *
 * � �������������� �����                           *
 * ������: 21 �������� 1995, �������                *
 ****************************************************/
#include "bamilib.h"

int hund2string( int h, char *s )
{

 char *hundreds[] = { "",
				"���",
				"������",
				"������",
				"���������",
				"�������",
				"��������",
				"�������",
				"���������",
				"���������"
			  };

 strcpy( s, hundreds[h] );
 return 1;
}

int dec2string( int d, char *s )
{
 char *dec[] = { "",
		 "������",
		 "��������",
		 "��������",
		 "�����",
		 "���������",
		 "����������",
		 "���������",
		 "�����������",
		 "���������"
		 };

 strcpy( s, dec[d] );
 return 1;
}

int decdig2string( int n, char *s )
{
 char * decdig[]  = { "������",
			"�����������",
			"����������",
			"����������",
			"������������",
			"����������",
			"�����������",
			"����������",
			"������������",
			"������������"
		  };

 strcpy( s, decdig[n] );
 return 1;
}

int dig2string( int d, char *s, int sex )
{
char *digMale[] = { "",
		 "����",
		 "���",
		 "���",
		 "������",
		 "����",
		 "�����",
		 "����",
		 "������",
		 "������"
			 };

 char *digFem[] = {"",
			"����",
			"���"
		  };

 if( d < 3 && sex == FEMALE )
	{
	  strcpy( s, digFem[d] );
	}
 else
	  strcpy( s, digMale[d] );
 return 1;
}


int thou2string( int n, char *s )
{
  char *thou[] = { " �����",
			" ������",
			" ������",
			" ������",
			" ������",
			" �����",
			" �����",
			" �����",
			" �����",
      " �����"
		  };

 if( n >= 10 && n < 20 ) n = 0;
 else if( n>=20 ) n%= 10;

 strcpy( s, thou[n] );
 return 1;
}

//

int male2string( long double d, char *root, char *s )
{
 char *tmp;
 int n = (long)d%100;

 if( strncmp( CharUpper(root), "����", 3 ) == 0 )
	{
		if( n == 1 )
	  tmp = "�";
		else if( n >1 && n < 5 )
	  tmp = "�";
		else tmp = "��";
		strncpy( s, root, 4 );
		s[4] = 0;
	}
 else
	{
	 if( n>=20 )
	n%= 10;

		if( n == 1 )
	  tmp = "";
		else if( n < 5 && n> 1 )
	  tmp = "�";
		else tmp = "��";
      strcpy( s, root );
  }

 strcat( s, tmp );

 return 1;
}

// ������������ �� ��� ������ ������

int num2string( int num, char *dest, int gender )
{
 char  tmp[80];
 int hund, dec;

 dest[0] = ' ';
 dest[1] = 0x0;

 if( (hund = num / 100) >= 1 )
	{
	  num -= hund*100;
	  hund2string( hund, tmp );
	  strcat( dest, tmp );
	}
 if( ( dec = num / 10) >=1 )
	{
	  num -= dec*10;
	  if( dec == 1 )
		 {
	 decdig2string( num, tmp );
	 num = 0;
		 }
	  else
	 dec2string( dec, tmp );
	  strcat( dest, " " );
	  strcat( dest, tmp );
	}
  if( num > 0 )
	{
	  dig2string( num, tmp, gender );
	  strcat( dest, " " );
	  strcat( dest, tmp );
	}
 return 1;
}


int parseNumber( long double numb, char *dest, int gender )
{
 long double trl=((long double)1000000l*1000000l);
 long int rem, tail;
 char name[80];
 char tmp[80];

 dest[0] = ' ';
 dest[1] = 0x0;

 if( (rem = ((numb / 1000000000L)/1000)) >= 1 )
	{

     numb -= trl*rem;
	  num2string( rem, tmp, MALE );
     male2string( rem, " ��������", name );
     strcat( dest, tmp );
     strcat( dest, name );
   }


 if( (rem = (numb / 1000000000L)) >= 1 )
	{

     numb -= (long double)1000000000L*rem;
	  num2string( rem, tmp, MALE );
	  male2string( rem, " ��������", name );
	  strcat( dest, tmp );
	  strcat( dest, name );
	}



 if( (rem = numb / 1000000L) >= 1 )
	{
     tail = rem*1000000L;
     numb -= tail;
     num2string( rem, tmp, MALE );
     male2string( rem, " �������", name );
	  strcat( dest, tmp );
     strcat( dest, name );
   }

 if( (rem = numb / 1000) >=1 )
	{
	 tail = rem*1000;
	 numb -= tail;
	  num2string( rem, tmp, FEMALE );
	 thou2string( rem%100, name );
	  strcat( dest, tmp );
	 strcat( dest, name );
	}

  if( (rem = numb ) >=1 )
   {
    num2string( rem, tmp, gender );
	  strcat( dest, tmp );
	}
 return 1;
}
