/* File: csvfunctions.c

  V00812068 Saige Liu

   IMPORTANT:  Replace the line above with your student number and your name.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csvfunctions.h"

#define MAXINPUTLINELEN     256

static SPREADSHEET theSS;   // the spreadsheet to work on
static int debug = 0;
static int tooWide = 0;


// forward declaration
static int extractItems(char *line, char row[][MAXLEN]);

void SS_SetDebug(int dbg) {
    debug = dbg;
}

SPREADSHEET *SS_ReadCSV(char *fileName) {
    char line[MAXINPUTLINELEN];
    SPREADSHEET result;
    int i;

    result.fileName = fileName;
    result.numRows = result.numCols = 0;

    FILE *f = fopen(fileName, "r");
    if (f == NULL) {
        fprintf(stderr, "Unable to read from file %s\n", fileName);
        perror(fileName);
        return NULL;
    }
    for( i = 0; i < MAXROWS; i++) {
        if (fgets(line, MAXINPUTLINELEN, f) == NULL) {
            // i--;    <=== this was a mistake!!
            break;
        }
        int k = extractItems(line, result.contents[i]);
        if (result.numCols == 0) {
            result.numCols = k;
        } else
        if (result.numCols != k) {
            fprintf(stderr, "Row %d has different number of columns from first row\n", i);
        }
    }
    result.numRows = i;
    fclose(f);
    memcpy(&theSS, &result, sizeof(SPREADSHEET));
    if (tooWide)
        fprintf(stderr, "Warning, number of columns exceeded the max of %d\n", MAXCOLS);
    return &theSS;
}

static int extractItems(char *line, char row[][MAXLEN]) {
	int col = 0;
	int state =1;
		int ix =0;
		int k = 0;
		
		char curr;
		curr = line[ix++];
		while(state !=4){
			switch(state){
			case (1):
			if(curr =='\"'){
				curr =line[ix++];
				state = 2;
			}else if(curr ==','){
				curr = line[ix++];
			row[col][MAXLEN] = '\0';
			col++;
				k=0;
				state = 1;
				
			}else if(curr =='\0'||curr =='\r'||curr =='\n'){
			row[col][MAXLEN] = '\0';
			col++;
				state = 4;
			}else {
				row[col][k++]= curr;
				curr =line[ix++];
			}
			break;
		case (2):
			if(curr =='\"'){
				curr =line[ix++];
				state = 3;
			}else if(curr ==','){
				row[col][k++]= curr;
				curr =line[ix++];
			}else if(curr =='\0'||curr =='\r'||curr =='\n'){
				printf("The input have an error!");
			row[col][MAXLEN] = '\0';
			col++;
				state = 4;
			}else {
				row[col][k++]= curr;
				curr =line[ix++];
			}
			break;
		case (3):
			if(curr =='\"'){
				row[col][k++]= curr;
				state = 2;
			}else if(curr ==','){
				curr =line[ix++];
			row[col][MAXLEN] = '\0';
			col++;
				k=0;
				state = 1;
				
			}else if(curr =='\0'||curr =='\r'||curr =='\n'){
				curr =line[ix++];
			row[col][MAXLEN] = '\0';
			col++;
				state = 4;
			}else {
				row[col][k++]= curr;
				state = 1;
			}
			break;
		case (4):
			break;
			
			}
		}
			
	 // force null termination
        
    //    line = NULL;  // required by strtok function
    
    return col;
    
	}
    
	
	

// Searches down the specified column for a row which contains text.
// The search starts at row number rowNum;
// The result is the row number (where the first row is numbered 0).
// If the text is not found, the result is -1.
int SS_FindRow(SPREADSHEET *ss, int colNum, char *text, int startNum) {
    if (debug)
        fprintf(stderr, "DEBUG: Call to SS_FindRow(--,%d,%s,%d)\n",
            colNum, text, startNum);
		
		if(colNum>ss->numRows-1){
			fprintf(stdout,"out of range");
			return -1;
		}
			
		int i,j,k;
		for(i= startNum;i<ss->numRows;i++){
			for(j=0;j<MAXLEN;j++){
				if(ss->contents[i][colNum][j]==text[0]){
					int bool =1;
					for( k=1;k<strlen(text);k++){
						j++;
						if(ss->contents[i][colNum][j]!=text[k]){
							
								bool=0;
						}
					}
				if(bool==1) return i;
				}
		
			}
		
		}

		



    return -1;
}

// Outputs the specified row of the spreadsheet.
// It is printed as one line on standard output.
void SS_PrintRow(SPREADSHEET *ss, int rowNum) {
    if (debug)
        fprintf(stderr, "DEBUG: Call to SS_PrintRow(--,%d)\n", rowNum);
		int i;
		for(i=0;i<ss->numCols;i++){
		fprintf(stdout,"%s  ",ss->contents[rowNum][i]);
		
		}
		fprintf(stdout,"\n",ss->contents[rowNum][i]);
    return;
}

// The specified column must contain the textual representations of numbers
// (either integer or floating point). The sum of the numbers in the column
// is returned as a floating point number.
double SS_ColumnSum(SPREADSHEET *ss, int colNum) {
    if (debug)
        fprintf(stderr, "DEBUG: Call to SS_ColumnSum(--,%d)\n", colNum);
    int j;
	float temp;
	float sum;
	sum=0;
	for(j=0;j<ss->numRows;j++){
		temp=atof(ss->contents[j][colNum]);
		
		sum= sum +temp;
		
	}
		//printf( "The sum of that column is %f", sum);
		
	
    // Note: atof(s) converts a string s to a float value
    return sum;
}

double SS_ColumnAvg(SPREADSHEET *ss, int colNum) {
    if (debug)
        fprintf(stderr, "DEBUG: Call to SS_ColumnAvg(--,%d)\n", colNum);
    int j;
	float temp;
	float avg;
	float sum;
	avg=0;
	sum=0;
	int num = ss->numRows-1;
	for(j=0;j<ss->numRows;j++){
		temp=atof(ss->contents[j][colNum]);
		
		sum= sum +temp;
		
	}
	
	avg=sum/j;
    // Note: atof(s) converts a string s to a float value
    return avg;
}

