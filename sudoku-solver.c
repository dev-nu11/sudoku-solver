/*####################################*/
/*##                                ##*/
/*##                                ##*/
/*####################################*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define G 9
//static int feld[G][G];
typedef struct {
	int feld[G][G];
	int loesung;
	char *pfad;
} sudoku;


	
int erzeugeSudoku(sudoku *feld); /* Einlesen des Sudokus */
int check(sudoku *feld, int x, int y, int wert); /* Aufruf der anderen Check Funktionen*/
int Box(sudoku *feld, int x, int y, int wert); /* Prüfen in der Box */
int Horizontal(sudoku *feld, int y, int wert); /* Prüfen in der Horizontalen */
int Vertical(sudoku *feld, int x, int wert); /* Prüfen in der Vertikalen */
int einsetzen(sudoku *feld, int x, int y); /* Die eigendliche Rekursions Funktion */
void Anzeigen(sudoku *feld); /* Anzeigen auf commandline */
void dateischreiben(sudoku *feld); /* Datei ausgabe*/
void datei_error(); /* Fehler ausgabe */

int main(int argc, char *argv[]) {	
 sudoku feld;
 feld.loesung =0;
 if (argc > 1){
	feld.pfad = (char *) malloc (strlen(argv[1]) * sizeof(char));
	feld.pfad[0]='\0';
	feld.pfad = argv[1];
//	strcat(feld.pfad ,argv[1]); //-.-
 }else{
	char tmp[100];
	tmp[0]='\0';
	printf("Bitte geben sie den Pfad zu dem Sudoku an:\n");
	scanf("%s", tmp);
	feld.pfad = (char *) malloc (strlen(tmp) * sizeof(char));
	feld.pfad[0]='\0';
	feld.pfad = tmp;
 }
 	  
  if(erzeugeSudoku(&feld) < 0)
        return -1;
  printf("\n### Eingelesenes Sudoku: ###\n");
  Anzeigen(&feld);
 
  printf("_____________________________\n\n");
  einsetzen(&feld, 0, 0);
  printf("Loesungen: %d\n", feld.loesung);
  if (feld.loesung <= 0)
	datei_error();
  return 0;
}
	
int erzeugeSudoku(sudoku *feld) { /* Einlesen des Sudokus */
  FILE *datei;
  int i, j;
  char temp[G+3] = {0};
 
  if((datei = fopen(feld->pfad, "r")) == NULL) {
    printf("Datei %s nicht gefunden!\n", feld->pfad);
    return -1;
  }
 
  for(i = 0; i < G; i++) {
    if(fgets(temp, G + 3, datei) == NULL) {  /* Prüfe auf vollständigkeit der Datei */
      printf("Daten nicht vollstaendig!\n");
      fclose(datei);
	  datei_error();
      return -1;
    }
    for(j = 0; j < G; j++) {
	if (temp[j] == 32){ 	/* Das Leerzeichen durch 0 ersetzen */
		feld->feld[i][j] = 0;
	}else{
		feld->feld[i][j] = temp[j] - '0';
		if ((feld->feld[i][j] > G)||(feld->feld[i][j] < 0)){	/* Prüfen ob ein Falsches zeichen sich in der Datei befindet */
			printf("Falsches Zeichen!\n");
			fclose(datei);
			datei_error();
			return -1;
			}
	  }
    }
	

  }
 if (fgets(temp, G + 3, datei) != NULL){
		fclose(datei);
		printf("Zuviel!\n");
		datei_error();
		return -1;
	}
	
  fclose(datei);
 
  return 0;
}

void Anzeigen(sudoku *feld) { /* Anzeigen des Sudokus in der Comandline nicht für die Funktion des Programms nötig */
  int x, y;
  for(x = 0; x < G; x++) {
    printf("%d",(x+1));
    for(y = 0; y < G; y++) {
      if ((y%3)== 0)
		printf("  ");
		printf("%d", feld->feld[x][y]);
    }
    if (((x+1)%3)== 0)
	printf("\n");
    printf("\n");
  }
}

int einsetzen(sudoku *feld, int x, int y) {
  int i;
  if(x == G) {                 /* Ende der Zeile */
    y++;
    x = 0;
    if(y == G)                 /* Ende der Zeile und er Spalte */
      return 1;
  }
 
  if(feld->feld[y][x] > 0)           /* Feld schon gesetzt */
    return einsetzen(feld, x+1, y);      /* Naechstes Feld */
 
  for(i = 1; i <= G; i++) {    /* Keine Zahl vorhanden */
    if(!check(feld, x, y, i)) {      /* Alle Zahlen durchgehen */
      feld->feld[y][x] = i;          /* Wenn Zahl passt, setzen */
        if(einsetzen(feld,x+1, y)) {    /* Naechstes Feld pruefen */
        feld->loesung++;                /* Loesung gefunden, ausgeben */
        printf("Loesung %d:\n", feld->loesung);
            Anzeigen(feld);
			dateischreiben(feld);
            printf("\n");
            //return 1;   /*Falls nur eine Loesung ausgeben werden soll*/
        }
    }
  }
 
  feld->feld[y][x] = 0;              /* Keine Zahl hat gepasst, wieder 0 setzen */
  return 0;
}

int check(sudoku *feld, int x, int y, int wert) { /* Aufruf der Einzelnen Checkfunktionen */
  if(Horizontal(feld, y, wert))
    return 1;
  if(Vertical(feld, x, wert))
    return 1;
  if(Box(feld, x, y, wert))
    return 1;
  return 0;
}

int Box(sudoku *feld, int x, int y, int wert) {	/* Prüfen ob der Wert in der Box schon vorhanden ist */
  int x_box, y_box, i, j;
  x_box = (int)(x / 3) * 3;				/* Ermitteln der Box */
  y_box = (int)(y / 3) * 3;
  for(i = y_box; i < y_box + 3; i++)	/* Prüfen der Box */
    for(j = x_box; j < x_box + 3; j++)
      if(feld->feld[i][j] == wert)
        return 1;
  return 0;
}

int Vertical(sudoku *feld, int x, int wert) { 	/* Prüfen ob der Wert in Vertikaler richtung schon vorhanden ist */
  int i;
  for(i = 0; i < G; i++)
    if(feld->feld[i][x] == wert)
      return 1;
  return 0;
}

int Horizontal(sudoku *feld, int y, int wert) { 	/* Prüfen ob der Wert in Horizontaler richtung schon vorhanden ist */
  int i;
  for(i = 0; i < G; i++)
    if(feld->feld[y][i] == wert)
      return 1;
  return 0;
}

void dateischreiben(sudoku *feld){ 	/* Ausgeben der sudoku_geloest.txt */
FILE *datei;
int x,y;
datei = fopen ("sudoku_geloest.txt","w");
//fprintf(datei, "Das Sudoku ist geloest.\nDie Anzahl der moeglichen Loesungen ist %d \nDie letzte Moeglichkeit ist:\n\n",loesung);
  for(x = 0; x < G; x++) {
    for(y = 0; y < G; y++) {
		fprintf(datei, "%c",(char)(feld->feld[x][y])+ '0');
    }
    fprintf(datei, "\n");
  }
fclose (datei);
}

void datei_error(){ 	/* Falls ein Fehler in der Datei vorliegt wird nur "Eingabefehler" in die Datei geschrieben */
FILE *datei;
	datei=fopen ("sudoku_geloest.txt","w");
	fprintf(datei, "Eingabefehler");
	fclose (datei);
}

