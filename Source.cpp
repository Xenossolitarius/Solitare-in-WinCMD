#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>
#include <conio.h>
#include <strsafe.h>
#include <iostream>
#include <locale.h>
#include <wchar.h>

#define SPADES "\xE2\x99\xA0" //SPADES

#define CLUBS "\xE2\x99\xA3" //CLUBS

#define HEARTS "\xE2\x99\xA5" //HEARTS

#define DIAMONDS "\xE2\x99\xA6" //DIAMONDS

#define BLOCK "\xE2\x96\x88" //BLOCK

#define COLONS 44 
#define LINES 18

/*
printf("\xE2\x99\xA0"); //SPADES

printf("\xE2\x99\xA3"); //CLUBS

printf("\xE2\x99\xA5"); //HEARTS

printf("\xE2\x99\xA6"); //DIAMONDS

printf("\xE2\x86\x91"); //LONGARROW

printf("\xE2\x96\xB2"); //ARROW

printf(SPADES);
*/

using namespace std; //malo c++ sintakse jer neke funkcionalnosti..lakse je



/*Ovo je vježba kao prvo iz vezanih lista. Kreirano je kao i vježba algoritama te vježba u optimizaciji. 
Solitare se može isprogramirat iznimno traljavo bez da se to primjeti u perfomansama, no ovdje ćemo obraditi neke 
koncepte u optimizaciji i nacine programiranja uglavnom birajući prosječna riješenja. Naranvo korištenje vezanih lista 
nije potrebno za ovu igru te se zasigurno može riješit običnim poljem i nizovima no nadam se da će program koristiti profesoru 
za demonstraciju što je moguće napravit na 3 godini fakulteta u 2-3 dana. Jer kao šta on zna ja nikad ne komentiram kod :P
*/

HANDLE wHnd;    // Handle za pisat
HANDLE rHnd;    // Handle za citat



typedef struct karta * Position; 



int timegen = 0; //globalna varijabla za problem sa randomom dosta interesentan problem XD
//struktura karta
struct karta {
	int broj;//1-13
	char boja;// S-srce K-karo P-pik T-tref 0-nista
	char stanje;// V-vidljivo N-nevidljivo O- vidljivo odabrano
	Position Next;
	Position Back;//mozda iskoristim

};

//struktura za petljanje po velicini konzole


//listafunkcija
struct karta * generate(struct karta *,Position );
void randomize(int * );
void Printkarte(Position);
int random4();
void place(struct karta*, struct karta *, Position, Position);
void showprototip();
void setFontSize(int);
void changecolor(char);
void blankmatrix(char [][COLONS]);
void printmatrix(char[][COLONS]);
void fillmatrix(char[][COLONS], struct karta *, struct karta *, Position, int);
void setupmatrix(char[][COLONS], struct karta *, struct karta *, Position, int);
void cardrw(char[][COLONS], Position , int , int );
void boardrw(char[][COLONS], Position , int , int );
void stashrw(char[][COLONS], Position, int, int);
void printcolorkarta(char[][COLONS], int, int);


//med i maslac
void main() { //dobra filozofija je return-at!
	
	//varijablegeneratebegin
	karta boja[4];
	karta plik;

	//varijablegenerateend

	//varijableplacebegin
	karta board[7];
	karta stash[4];
	karta rotate;
	
	//varijableplaceend

	//varijabla char matrix
	char screenmatrix[LINES][COLONS];
	int rotatestate = 2; // ne izlistan
	//varijabla char matrix end
	
	
	
	



	//promjena char kodiranja

	//setlocale(LC_ALL, "");
	system("chcp 65001"); //UTF-8
	//system("chcp 1200"); //UTF-16
	
	//wideoutput redirect na standardni
	fwide(stdout, 1);
	
	//za promjenu imena konzole

	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	rHnd = GetStdHandle(STD_INPUT_HANDLE);


	TCHAR NewTitle[MAX_PATH]; 
	


	
		// Build new console title string.

		StringCchPrintf(NewTitle, MAX_PATH, TEXT("Solitare"));

		// Set console title to new title
		SetConsoleTitle(NewTitle);


	
		setFontSize(32);
		
		//system("mode 650"); //fullscreen mode

		// parsirat string da moze minjat kasnije....jep ovo bi bila dobra ideja, ali moram odma pazit na "spacing"....god...
		
		system("mode con:cols=44 lines=22"); // ok ovo su bila 3 sata googlanja i jebem ja mater i microsoftu i svim retardiranim programerima isusovim
		
		//za ovo gori napravit funkciju koja parsira string
	
											  	


		//kraj petljanja s konzolom




	plik.Next=generate(boja, &plik); // za primjetit šaljemo niz struktura, a jednom sam slao dvostruki pointer na polje struktura te je to razlog zašto se C izbjegava nakon faxa
	
	//Printkarte(plik.Next); //provjera
	/*Ovo sa plikom je potpuno nepotrebno već se može direktno izgenerirat u unutar gameimportant vezanih lista, ali ovo je prototip koji simulira 
	ponašanje čovjeka i njegovo mješanje karata, misli se možda i na animaciju koja bi se mogla puno lakše programirat sljedno iz jednog velikog spremnika*/

	//nastavit vamo sa Place funkcijom osigurat nulto stanje za board i stash, razradit na boardu da zadnja bude visible ..moguce
	//dupli pointer, više pointera...zatim obradit rotate i njegovu ideju..nutlo stanje sa hidden...i napravit funkciju show jer testiranje ce bit nepregledno!
	place(board, stash, &rotate, &plik);
	//sve je postavljeno kako treba, plik je razasut sad napravit funkciju "show" to bi tribalo bit pregledno.Postavit array na ekran ? Good question...

	

	blankmatrix(screenmatrix); // all blank

	fillmatrix(screenmatrix, board, stash, &rotate, rotatestate); // test funkcija za popunjavanje 
	
	setupmatrix(screenmatrix, board, stash, &rotate, rotatestate);

	printmatrix(screenmatrix);
	

	
	//showprototip(); // testiranje boja i znakova
	
	


	//system("pause"); // prije kraja za test
	return;
}




void setupmatrix(char(*matrix)[COLONS], struct karta * board, struct karta * stash, Position Rotate, int rotatestate) {
	//rotate logika
	char rotatechar;
	int x, y, k;

	x = y = k = 0;
	
	if (rotatestate == 1 || rotatestate == 2 ){
		rotatechar = 'Y';
	}
	else if (rotatechar == 0) {
		rotatechar = '0';
	}
	for (int i = 0; i < 3; i++) {
		matrix[1][4 + i] = rotatechar;
	}

	//rotate logika // fali puno


	//board logika


	
	k = 21;  //pocetna pozicija
	for (int j = 0; j < 7; j++) {
		boardrw(matrix, &board[j], k, 4);
		k = k + 3; //moze se stavit u head for petlje, ali nema potrebe

	}
	//cardrw(matrix, board, x, y);
	
	//stash logika

	k = 30;
	for (int j = 0; j < 4; j++) {
		stashrw(matrix, &stash[j], k, 1);
		k = k + 3;
	}

	return;

}
void stashrw(char(*matrix)[COLONS], Position P, int x, int y) {
	if (P == NULL) {
		printf("\nWTF saljes nule\n");
	}
	while (P->Next != NULL) {
		P = P->Next;
	
	}
	cardrw(matrix, P, x, y);


}


void boardrw(char(*matrix)[COLONS], Position P, int x, int y) {
	Position Q;
	int i = 0;
	if (P == NULL) { // ako se dogodi exception da bar vidim koji je
		printf("\nWTF saljes nule\n");
	}
	if(P->Next == NULL) { // ako je board prazan
		cardrw(matrix, P, x, y);
		i++;
		return;
	}
	else { //ako ima nesto aj na sljedeci
		P = P->Next;
	}
	while (P != NULL) { //ispisuj karte na nove pozicije dok ne dode do 0
		cardrw(matrix, P, x, y);
		y = y + 2;
		i++;
		P = P->Next;

	}
	for (int k=i; k < 7; k++) { //ispisuj praznine koje su ti ostale
		matrix[y][x] = ' ';
		matrix[y][x + 1] = ' ';
		y = y + 2;
	}

	//printf("%d", i);




}


void cardrw(char(*matrix)[COLONS], Position P, int x, int y) { // fun fact ovo je rađeno od kraja prema pocetku
	
	if (P->broj == 0) {
		matrix[y][x] = '_';
		matrix[y][x+1] = '_';
		return;
	}

	if (P->stanje == 'V') {

		switch (P->broj) {
		case 13:
			matrix[y][x] = 'K';
			break;
		case 12:
			matrix[y][x] = 'D';
			break;
		case 11:
			matrix[y][x] = 'J';
			break;
		case 10:
			matrix[y][x] = 'X'; // hvala rimljanima!
			break;
		default:
			matrix[y][x] = P->broj + '0';
			break;

		}

		matrix[y][x + 1] = P->boja;
	}
	else if (P->stanje == 'N') {
		matrix[y][x] = 'N';
		matrix[y][x+1] = 'N';

	}//dodat Odabrano možda kasnije


	return;


}





void fillmatrix(char(*matrix)[COLONS], struct karta * board, struct karta * stash, Position Rotate,int rotatestate) {
	//dali ovo funkcionizirat nadalje ili ostavit logiku vamo ???? //DA!
	//prvo napravit sva polja da vidimo di smo
	//zapis bitnih pozicija
	//rotate logika vamo
	matrix[1][4] = '_';
	matrix[1][5] = '_';
	matrix[1][6] = '_';

	matrix[1][8] = '_';
	matrix[1][9] = '_';
	
	matrix[1][11] = '_';
	matrix[1][12] = '_';

	matrix[1][14] = '_';
	matrix[1][15] = '_';

	//stash logika vamo
	matrix[1][30] = '_';
	matrix[1][31] = '_';

	matrix[1][33] = '_';
	matrix[1][34] = '_';

	matrix[1][36] = '_';
	matrix[1][37] = '_';

	matrix[1][39] = '_';
	matrix[1][40] = '_';

	//board logika vamo
	for (int k = 4; k < 18; k = k + 2) {
		matrix[k][21] = '_';
		matrix[k][22] = '_';

		matrix[k][24] = '_';
		matrix[k][25] = '_';

		matrix[k][27] = '_';
		matrix[k][28] = '_';

		matrix[k][30] = '_';
		matrix[k][31] = '_';

		matrix[k][33] = '_';
		matrix[k][34] = '_';

		matrix[k][36] = '_';
		matrix[k][37] = '_';

		matrix[k][39] = '_';
		matrix[k][40] = '_';
	}



	return;
}






void printmatrix(char(*matrix)[COLONS]) { //this is surely getting changed  //and it didn't ... i wrote a new function yaaaay....rip 
	for (int i = 0; i < LINES; i++) {
		for (int j = 0; j < COLONS; j++) {
			
			switch (matrix[i][j]) {
			
			case 'Y':
				changecolor('G');
				printf(" ");
				changecolor('W');
				break;
			case '0':
				changecolor('X');
				printf(" ");
				changecolor('W');
				break;
			case 'S':
				changecolor('R');
				printf(HEARTS);
				changecolor('W');
				break;
			case 'C':
				changecolor('R');
				printf(DIAMONDS);
				changecolor('W');
				break;
			case 'P':
				changecolor('B');
				printf(SPADES);
				changecolor('W');
				break;
			case 'T':
				changecolor('B');
				printf(CLUBS);
				changecolor('W');
				break;		
			case 'N':
				changecolor('N');
				printf(BLOCK);
				changecolor('W');
				break;
			case '_':
				changecolor('N');
				printf("_");
				changecolor('W');
				break;
			default:	
				printf("%c", matrix[i][j]);
				break;

			}
		}
		//system("pause");
		printf("\n");
	}

}



void blankmatrix(char (*matrix)[COLONS]) { //puni spejsevima matricu //RESET
	int i, j;
	for (i = 0; i < LINES; i++) {
		for (j = 0; j < COLONS; j++) {
			matrix[i][j] = ' ';
		}
			
	}
	return;

}


void showprototip() {
	
	int max = 30;
	char c = '1';
	
	printf("\n");
	printf("   ");//3
	printf("___");//3
    printf(" ");//1
	printf("__");//2
	printf(" ");//1
	printf("__");//2
	printf(" ");//1
	printf("__");//2
	printf("              ");//15
	printf("__");//2
	printf(" ");//1
	printf("__");//2
	printf(" ");//1
	printf("__");//2
	printf(" ");//1
	printf("__");//2
	printf("    ");//3
	printf("\n\n");
	//change font vjezba
	printf("%c1234567890QWERTZUIOPASDFGHJKLYXCVBNM", c);

	printf("\n\n");
	
	changecolor('R');
	printf("\xE2\x99\xA0"); //SPADES
	changecolor('B');
	printf("\xE2\x99\xA3"); //CLUBS
	
	printf("\xE2\x99\xA5"); //HEARTS

	printf("\xE2\x99\xA6"); //DIAMONDS

	printf("\xE2\x86\x91"); //LONGARROW

	printf("\xE2\x96\xB2"); //ARROW

	printf("\xE2\x96\x88"); //BLOCK
	
	printf(SPADES);


	
	printf("\n\n");
	
	
	

   
}








void place(struct karta * board, struct karta * stash, Position Rotate, Position Plik) {
	Position Last;
	Position P;
	int max = 0;
	
	
	P = Plik;
	P = P->Next;
	
	
	Plik->broj = 0;
	
	
	


	for (int i = 0; i < 7; i++) { // postavljanje nultih na ništa
		board[i].broj = 0;
		board[i].boja = '0';
		board[i].stanje = 'N';
		board[i].Next = NULL;
		board[i].Back = NULL;
	}
	
	for (int i = 0; i < 4; i++) { // postavljanje stash na ništa, to je sve ne mora se više petljat
		stash[i].broj = 0;
		stash[i].boja = '0';
		stash[i].stanje = 'N';
		stash[i].Next = NULL;
		stash[i].Back = NULL;
	}


	for (int k = 0; k < 7; k++) {// za sve boardove
		board[k].Next = P;
		for (int j = 0; j < max; j++) { //prebaci plik
			P = P->Next;
		}
		P->stanje = 'V'; // ucini zadnjeg vidljivog
		Last = P;
		max++;
		P = P->Next;
		Last->Next = NULL;



	}


/*  //provjera za board
	for (int k = 0; k < 7; k++) { 
		Printkarte(&board[k]);
		printf("\n");
		printf("\n");
	}
	*/

	//Printkarte(P); //provjera ostatka plika
	
	//Rotate head na nulu
	Rotate->broj = 0;
	Rotate->boja = '0';
	Rotate->stanje = 'N';
	Rotate->Next = NULL;
	Rotate->Back = NULL;
	//rotate na ostatak plika
	Rotate->Next = P;
	
	//Printkarte(Rotate); //provjera rotate
	





}








struct karta * generate(struct karta * boja, Position Plik) {
	int randomnum[13]; //ovo bi sve bilo bolje dinamicki napravit, ali ovo je studentsko rješenje ^^
	Position P; //šetač
	Position R[4];
	int random = 0;//random var
	int gotovo[4] = { 0,0,0,0 };


	
	
	for (int k = 0; k < 13; k++) {//sve nula
		randomnum[k] = 0;
	}
	
	for (int i = 0; i < 4; i++) { // postavljanje nultih na ništa
		boja[i].broj = 0;
		boja[i].boja = '0';
		boja[i].stanje = 'N';
		boja[i].Next = NULL;
		boja[i].Back = NULL;
	}


	


	for (int i = 0; i < 4; i++) {//kreiranje prvih i izjednacavanje na pointere kako bi mogli šetat
		
		boja[i].Next = (Position)malloc(sizeof(struct karta));
		R[i] = boja[i].Next;

	}


							
	//popunjavanje boja ( vezanih lista) izbjec cemo stvaranje nove funkcije jer one koje stvorimo vamo ce bit posljednje dodane
	
	//postavljanje 
	
	
	
	for (int i = 0; i < 4; i++) {
		//printf("%d. ulazak \n", i+1); provjera
		randomize(&randomnum[0]);//randomiziraj karte  //ruzno baratanje arrayom no nmvz
		
		R[i]->broj = randomnum[0]; //postavljanje prvih
		R[i]->stanje = 'N';
		R[i]->Next = NULL;
		P = R[i]; //Namistanje šetaća na pojedine liste
		switch (i) { //svaka lista svoju boju
			case 0:
				R[i]->boja = 'S';
				break;
			case 1:
				R[i]->boja = 'C';
				break;
			case 2:
				R[i]->boja = 'P';
				break;
			case 3:
				R[i]->boja = 'T';
				break;
			}
	
		for (int k = 1; k < 13; k++) {// sljedecih 12

			if (P->Next == NULL) {
				P->Next= (Position)malloc(sizeof(struct karta));
				P = P->Next; //++
				P->boja = R[i]->boja;  //izjednacavanje boje i stanja
				P->stanje = R[i]->stanje;
				P->broj = randomnum[k]; //broj iz random niza
				P->Next = NULL;

			}
		
		}
		
		//Printkarte(R[i]); //provjera I suck

		
		

}
	//system("pause");
	
	//sad ovo bi se moglo stavit u novu funkciju i to bi savršeno imalo smisla sa preglednoscu, ali za optimizaciju bi imalo 0 smisla. 
	//Nismo gotovi sekvencijalnu paradigmu.

	// Random multipleksor u plik
	//ovo su neke teže i smiješne akrobacije

	random = random4();
	Plik = R[random]; //postavi na prvog nekog u nizu
	R[random] = R[random]->Next; //tog makni dalje
	P = Plik; //plik je na 1. p ide dalje
	while(gotovo[0] == 0 || gotovo [1] == 0 ||  gotovo[2] == 0 || gotovo[3] == 0) { //akrobacije citaj ako je bar jedan 0 vrti petlju, moglo se stavit pointer == NULL, ali ne čitljivo osim toga bolje da uspoređuje int
		random = random4();
		//printf("\nRandom je: %d", random);// testiranje random ispod 1 sec
		if (R[random] != NULL) { //ako nije taj odredeni null
			P->Next = R[random]; //prebaci pointer na njega
			P = P->Next;         // pointer P ide isto na nejga direktno
			R[random] = R[random]->Next; //random R ide dalje
			if (R[random] == NULL) { //ako jedan od njih uspije doc do kraja digni zastavicu na 1
				gotovo[random] = 1;
			}
			
		}
		
		//Printkarte(Plik); //provjera jer sam retard i stavljam 4 u R[]
		
	}
	//ovo sam zakomentira jer ovakve akrobacije rade programeri ljevaci, ali šta radi radi


	//Printkarte(Plik);

	//Programirajte funkcije koje vam testiraju funkcije. Breakpointovi su super tko ih zna koristit, ali lakše je sam ubacit svoj
	//printf ili funkciju i vidit što je zblokalo pogotovo u veznaih lista!
	//Printkarte čini točno to sa minimalnim trudom
	
	

	



	return Plik;
}



void randomize(int * randomnum) {
	int i = 0;
	int j = 0;
	int temp = 0;
	int flag = 0;
	int MAX = 13;
	int MIN = 1;
	
	for (int k = 0; k < 13; k++) {
		*(randomnum + k) = 0;
	}

	

	while ( i != 13) {
		srand(clock() + timegen);//again
		timegen += 1;
		temp = (rand() % MAX) + MIN;
		
		for (j = 0; j < 13; j++) {
			
		
			if ( *(randomnum + j) == temp ) {
				
				flag = 1; //1 ako ima clana vec u nizu

				break;
			}
		}
		
		
		
		if (flag == 1) {//ako ima clana resetiraj
			flag = 0;
		}else {
			*(randomnum + i) = temp; //ako nema ubaci aj dalje
			i++;
			if (temp == ((MAX-1)+MIN)) {//optimizacija za gornju granicu
				MAX--;
			}
			else if (temp == MIN) {//optimizacija za donju granicu
				MIN++;
				MAX--;
			}
		

			/*Zanimljivo je dali ova optimizacija uopce funkcionira kao optimizacija jer tribala bi pomagat oko 15%,
			a uvodi 2 provjere no dobro ako izbjegne jednu repeticiju izbjec ce 13 naredbi. Tako da je to optimizacija od 11 naredbi.
			Mogao se koristit randomize obrnuti quicksort te iz niza od 1 do 13 samo izmisat znamenke. To bi bio najbrzi pristup.
			Nadalje ova optimizacija bi se mogla ubrzat ako provjerava imali jos koji broj ito rekurzivno od krajeva no to bi ubrzalo na ~25%
			No dobro da moramo generirat 20+ karata sigurno ne bi ovako programira */

		}

	}

	
	
}


void Printkarte(Position P) {
	int suma = 0;
	while (P != NULL) {
		suma++;
		printf("\n%d. \nBroj karte: %d \nVrsta karte: %c \nBoja karte: %c \n", suma, P->broj, P->stanje, P->boja);
		P = P->Next;

	}



}

int random4() {
	
	timegen += 1; //koja je ovo bila akrobacija, a sve zbog problema < 1 sec
	srand(clock()+timegen);
	return rand() % 4 ;

	
}


void setFontSize(int FontSize)
{
	HANDLE outcon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX info = { 0 };
	info.cbSize = sizeof(info);
	info.dwFontSize.Y = FontSize; // leave X as zero
	info.dwFontSize.X = 26;// leave X as zero
	info.FontWeight = FW_NORMAL;
	wcscpy(info.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
	SetConsoleTextAttribute(outcon, 0x2F);

}

void changecolor(char Color) {
	HANDLE outcon = GetStdHandle(STD_OUTPUT_HANDLE);
	
	//FONTCOLOR 
	//0X00 - BLACK
	//0X01 - DARK BLUE
	//0x02 - GREEN
	//0X03 - LIGHT BLUE
	//0X04 - DARK RED
	//0X05 - DARK PINK
	//0X06 - DARK YELLOW
	//0X07 - WHITE
	//0X08 - GRAY
	//0X09 - BLUE
	//0x0A - GREENYELLOW
	//0x0B - LIGHT BLUE
	//0X0C - RED
	//0X0D - PINK
	//0X0E - YELLOW
	//0X0F - WHITE

	//BACKGROUND
	//0X10 - BLUE
	//0X20 - DARK GREEN
	//0X30 - DARK TIRQUIZE
	//0X40 - DARK RED
	//0X50 - DARK PINK
	//0X60 - DARK YELLOW
	//0X70 - GRAY
	//0X80 - DARK GRAY
	//0X90 - DARK BLUE
	//0XA0 - GREEN
	//0XB0 - LIGHT TIRQUIZE
	//0XC0 - RED
	//0XD0 - PINK
	//0XE0 - YELLOW
	//0XF0 - WHITE
	switch (Color) {
	case 'W': //white ili ti ga standard
		SetConsoleTextAttribute(outcon, 0x2F);
		break;
	case 'R': //red
		SetConsoleTextAttribute(outcon, 0x2C);
		break;
	case 'B': //black
		SetConsoleTextAttribute(outcon, 0x20);
		break;
	case 'P': //plavo...jea i suck
		SetConsoleTextAttribute(outcon, 0x21);
		break;
	case 'X':
		SetConsoleTextAttribute(outcon, 0xFF);
		break;
	case 'G':
		SetConsoleTextAttribute(outcon, 0xAF);
		break;
	case 'N':
		SetConsoleTextAttribute(outcon, 0x2A);
		break;

	default:
		break;
	}

	//define more colors later!!!!
	



}

