#include "matrix.h"
#include <math.h>

#define _DEBUG_

void ReadData(FILE* fin, double**pMatrix, double* b, int nDim);

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf( "Nieprawid³owe wywo³anie programu!" );
		return 1;
	}

	FILE *pFile = fopen(argv[1], "r");	// r - plik do odczytu
	if (pFile == NULL)
	{
		printf( "Nie udalo sie otworzyc pliku %s", argv[1] );
		return 1;
	}

	int nDim = 0;
	fscanf(pFile, "%d", &nDim);	// Pobranie wymiaru macierzy
	if (nDim < 1)
	{
		perror( "Bledne podanie wymiaru macierzy!\n" );
		return 1;
	}

	double** pMx = NULL;
	if (!CreateMatrix(&pMx, nDim))
	{
		perror("Blad alokacji pamieci! -> Main");
		return 1;
	}

	double* pB = ( double* )malloc( nDim * sizeof( double ) );
	if ( !pB )
	{
		perror( "Blad alokacji pamieci! -> Main" );
		return 1;
	}
	memset( pB, 0, nDim * sizeof( double ) );

	
	ReadData( pFile, pMx, pB, nDim );
	double fDet = Det( pMx, nDim );
	if ( fabs( fDet ) < 1e-10 )
	{
		printf( "Blad, nie mozna odwrocic macierzy o wyznacziku = 0!" );
		return 1;
	}

	double **pInvMx = NULL;
	if ( !CreateMatrix( &pInvMx, nDim ) )
	{
		perror( "Blad alokacji pamieci!" );
		return 1;
	}
	InverseMatrix( pInvMx, pMx, nDim, fDet );

	double *pRes = (double*)malloc(nDim * sizeof(double));
	if (!pRes)
	{
		perror( "Blad alokacji pamieci!" );
		return 1;
	}
	memset( pRes, 0, nDim * sizeof( double ) );
	LayoutEqu( pInvMx, pB, pRes, nDim );


#ifdef _DEBUG_
	printf( "wymiar macierzy: %d\n", nDim );
	printf( "\nmacierz:\n" );
	PrintMatrix( pMx, nDim );
	printf( "\nwyrazy wolne:\n" );
	double *pPom = pB;
	for (int i = 0; i < nDim; i++)
		printf( "%lf  ", *pPom++ );
	printf( "\n\nwyznacznik: %lf\n", fDet );
	printf( "\nmacierz odwrotna:\n" );
	PrintMatrix( pInvMx, nDim );
	printf( "\nWyniki\n" );
	pPom = pRes;
	for ( int i = 0; i < nDim; i++ )
		printf( "x%d = %lf\n", i, *pPom++ );
	printf( "\n" );
#endif // _DEBUG_

	DeleteMatrix( &pMx, nDim );
	DeleteMatrix( &pInvMx, nDim );
	free( pB );
	free( pRes );

	return 0;
}

void ReadData(FILE* fin, double**pMatrix, double* b, int nDim)
{
	for ( int i = 0; i < nDim; i++ )
	{
		double *pPom = *pMatrix++;
		for ( int j = 0; j < nDim; j++ )
		{
			fscanf( fin, "%lf", pPom++ );
		}
		fscanf( fin, "%lf", b++ );
	}
}



